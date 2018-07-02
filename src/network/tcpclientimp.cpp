#include "networkimp.h"

using namespace asio::ip;

void TCPClientImpl::listen()
{
    for(;;)
    {
        char header[sizeof(MsgHeader)];
        asio::error_code error;
        asio::read(
            this->sock,
            asio::buffer(header),
            asio::transfer_exactly( sizeof(MsgHeader) ),
            error );

        if(error)
        {
            break;
        }

        asio::read(
            this->sock,
            asio::buffer(this->bufferIn),
            asio::transfer_exactly( ((MsgHeader*)header)->size ),
            error );

        if(error)
        {
            break;
        }

        if( this->onRecive != nullptr )
        {
            this->onRecive( std::string(bufferIn) );
        }
    }

    //event connection closed
    this->connected = false;
}

void TCPClientImpl::send( std::string data )
{
    MsgHeader header;
    header.type = DataType::custom;
    header.size = data.size() + 1;
    
    memcpy( bufferOut, &header, sizeof(MsgHeader) );

    asio::error_code error;
    asio::write( 
        sock,
        asio::buffer( bufferOut ),
        asio::transfer_exactly( sizeof(MsgHeader) ),
        error
    );

    if(error)
    {
        return;
    }

    memset( bufferOut, 0, re::networkMaxPackage );
    memcpy( bufferOut, data.c_str(), data.size() );

    asio::write(
        sock,
        asio::buffer(bufferOut),
        asio::transfer_exactly( header.size ),
        error
    );
}

bool TCPClientImpl::isConnected()
{
    return this->connected;
}

bool TCPClientImpl::connect( std::string addr, int port )
{
    tcp::resolver resolver(io_service);
    tcp::resolver::query query( addr, std::to_string(port) );
    tcp::resolver::iterator endpoint_iterator
        = resolver.resolve(query);

    asio::connect( this->sock, endpoint_iterator );

    asio::error_code error;
    asio::read(
        sock,
        asio::buffer(bufferIn),
        asio::transfer_exactly(4),
        error );

    if(error)
    {
        return false;
    }

    listner = std::thread( std::bind(&TCPClientImpl::listen, this) );
    listner.detach();

    this->connected = false;
    return true;
}

TCPClientImpl::TCPClientImpl()
    : sock( io_service )
{

}

re::TCPClientPtr re::TCPClient::get()
{
    re::TCPClientPtr client = std::make_shared<TCPClientImpl>();
    return client;
}
