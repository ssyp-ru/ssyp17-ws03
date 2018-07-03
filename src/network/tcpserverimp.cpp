#include "networkimp.h"

using namespace asio::ip;
 
#include <stdio.h>

namespace re
{

void TCPServerImpl::send( int id, std::vector<char> data ){
    MsgHeader header;
    header.type = DataType::custom;
    header.data_size = data.size();

    std::shared_ptr<tcp::socket> sock = this->clients[id];
    char buffer[max_size_of_package];
    
    encode_header( buffer, header );
    memcpy( buffer + msg_header_size, data.data(), data.size() );

    asio::error_code error;
    asio::write( 
        *sock,
        asio::buffer( buffer ),
        asio::transfer_exactly( msg_header_size + header.data_size ),
        error
    );

    if(error)
    {
        return;
    }
}

void TCPServerImpl::listen( int id ) {
    std::shared_ptr<tcp::socket> sock = this->clients[id];
    char buffer[max_size_of_package];
    asio::error_code error;

    asio::read( 
        *sock, 
        asio::buffer(buffer),
        asio::transfer_exactly( 2 ),
        error 
    );

    if( buffer[0] != 'o' ||
        buffer[1] != 'k' )
    {
        return;
    }

    if( this->callback )
    {
        this->callback( Callback_event::connect, id, std::vector<char>() );
    }

    for(;;) {
        asio::read( 
            *sock, 
            asio::buffer(buffer),
            asio::transfer_exactly( msg_header_size ),
            error 
        );

        if(error)
        {
            break;
        }

        MsgHeader header = decode_header( buffer );

        asio::read(
            *sock,
            asio::buffer(buffer),
            asio::transfer_exactly(header.data_size),
            error
        );

        if(error)
        {
            break;
        }

        std::vector<char> data( buffer, buffer + header.data_size );

        if(this->callback)
        {
            this->callback( Callback_event::msg_recive, id, data );
        }
    }

    if( this->callback )
    {
        this->callback( Callback_event::disconnect, id, std::vector<char>() );
    }
}

void TCPServerImpl::accept() {
    char header_buff[32];
    MsgHeader header;
    header.ver = version_of_our_cool_protocol_over_tcp;
    header.data_size = 0;
    header.type = 0;
    encode_header( header_buff, header );
    for(;;) {
        std::shared_ptr<tcp::socket> new_client = std::make_shared<tcp::socket>(io_service);
        this->tcp_acceptor.accept( *new_client );

        asio::write(
            *new_client,
            asio::buffer(header_buff),
            asio::transfer_exactly(4)
        );

        int client_id = this->clients.size();
        this->clients.push_back(new_client);

        std::thread th = std::thread( std::bind(&TCPServerImpl::listen, this, std::placeholders::_1), client_id );
        th.detach();
    }
}

void TCPServerImpl::setup( int port ) {
    tcp_acceptor = tcp::acceptor(io_service, tcp::endpoint(tcp::v4(), 11999));
    std::thread acceptor(std::bind(&TCPServerImpl::accept, this));
    acceptor.detach();
}

int TCPServerImpl::get_client_count()
{
    return this->clients.size();
}

void TCPServerImpl::set_callback( Callback callback ) {
    this->callback = callback;
}

TCPServerImpl::TCPServerImpl()
    : tcp_acceptor(io_service)
{
    

}

re::TCPServerPtr re::TCPServer::create()
{
    re::TCPServerPtr server = std::make_shared<TCPServerImpl>();
    return server;
}

}