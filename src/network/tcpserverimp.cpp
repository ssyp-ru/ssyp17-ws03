#include "networkimp.h"

using namespace asio::ip;

#include <stdio.h>

void TCPServerImpl::send( int id, std::string data ){
    MsgHeader header;
    header.type = DataType::custom;
    header.size = data.size() + 1;

    tcp::socket *sock = this->clients[id];
    char buffer[re::networkMaxPackage];
    
    memcpy( buffer, &header, sizeof(MsgHeader) );

    asio::error_code error;
    asio::write( 
        *sock,
        asio::buffer( buffer ),
        asio::transfer_exactly( sizeof(MsgHeader) ),
        error
    );

    if(error)
    {
        return;
    }

    memcpy( buffer, data.c_str(), data.size() );
    buffer[data.size()] = '\0';

    asio::write(
        *sock,
        asio::buffer(buffer),
        asio::transfer_exactly( header.size ),
        error
    );
}

void TCPServerImpl::listen( int id ) {
    tcp::socket *sock = this->clients[id];
    char buffer[re::networkMaxPackage];
    for(;;) {
        asio::error_code error;
        asio::read( 
            *sock, 
            asio::buffer(buffer),
            asio::transfer_exactly( sizeof(MsgHeader) ),
            error 
        );

        if(error)
        {
            //event disconect;
            break;
        }

        MsgHeader header = *(MsgHeader*)buffer;

        asio::read(
            *sock,
            asio::buffer(buffer),
            asio::transfer_exactly(header.size),
            error
        );

        if(error)
        {
            break;
        }

        if(this->onRecive)
        {
            this->onRecive( id, std::string(buffer) );
        }
    }
}

void TCPServerImpl::accept() {
    char buffer[5] = "conn";
    for(;;) {
        tcp::socket *new_client = new tcp::socket(io_service);
        this->tcp_acceptor.accept( *new_client );
        
        new_client->non_blocking(true);

        asio::write(
            *new_client,
            asio::buffer(buffer),
            asio::transfer_exactly(4)
        );

        int client_id = this->clients.size();
        this->clients.push_back(new_client);

        std::thread th = std::thread( std::bind(&TCPServerImpl::listen, this, std::placeholders::_1), client_id );
        th.detach();

        if( this->onClientConnect != nullptr )
        {
            this->onClientConnect( client_id );
        }
    }
}

void TCPServerImpl::setup( int port ) {
    std::thread acceptor(std::bind(&TCPServerImpl::accept, this));
    acceptor.detach();
}

int TCPServerImpl::getClientCount()
{
    return this->clients.size();
}

TCPServerImpl::TCPServerImpl()
    : tcp_acceptor(io_service, tcp::endpoint(tcp::v4(), 11999))
{
    

}

re::TCPServerPtr re::TCPServer::get()
{
    re::TCPServerPtr server = std::make_shared<TCPServerImpl>();
    return server;
}