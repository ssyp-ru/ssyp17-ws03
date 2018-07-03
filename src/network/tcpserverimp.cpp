#include "networkimp.h"

using namespace asio::ip;
 
#include <stdio.h>

using namespace re;

void TCPServerImpl::send( int id, std::vector<char> data ){
    MsgHeader header;
    header.type = DataType::custom;
    header.size = data.size();

    std::shared_ptr<tcp::socket> sock = this->clients[id];
    char buffer[max_size_of_package];
    
    encode_header( buffer, header );
    memcpy( buffer + msg_header_size, data.data(), data.size() );

    asio::error_code error;
    asio::write( 
        *sock,
        asio::buffer( buffer ),
        asio::transfer_exactly( msg_header_size + header.size ),
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
    for(;;) {
        asio::error_code error;
        asio::read( 
            *sock, 
            asio::buffer(buffer),
            asio::transfer_exactly( msg_header_size ),
            error 
        );

        if(error)
        {
            //event disconect;
            break;
        }

        MsgHeader header = decode_header( buffer );

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

        std::vector<char> data( buffer, buffer + header.size );

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
    char buffer[5] = "conn";
    for(;;) {
        std::shared_ptr<tcp::socket> new_client = std::make_shared<tcp::socket>(io_service);
        this->tcp_acceptor.accept( *new_client );
        
        //new_client->non_blocking(true);

        asio::write(
            *new_client,
            asio::buffer(buffer),
            asio::transfer_exactly(4)
        );

        int client_id = this->clients.size();
        this->clients.push_back(new_client);

        std::thread th = std::thread( std::bind(&TCPServerImpl::listen, this, std::placeholders::_1), client_id );
        th.detach();

        if( this->callback )
        {
            this->callback( Callback_event::connect, client_id, std::vector<char>() );
        }
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

re::TCPServerPtr re::TCPServer::get()
{
    re::TCPServerPtr server = std::make_shared<TCPServerImpl>();
    return server;
}