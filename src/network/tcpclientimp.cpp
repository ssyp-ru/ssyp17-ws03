#include "networkimp.h"

using namespace asio::ip; 

namespace re
{

void encode1byte( char *ch, uint8_t n)
{
    *ch = 0;
    for( int i = 0; i < 8; i++ )
    {
        *ch^= ( n%2 << i );
        n /= 2;
    }
}

uint8_t decode1byte( char *ch )
{
    uint8_t n = 0;
    for( int i = 7; i >= 0; i-- )
    {
        n *= 2;
        n += ( *ch >> i ) & 1;
    }
    return n;
}

void encode2byte( char *ch, uint16_t n)
{
    encode1byte( ch, n % 256 );
    encode1byte( ch + 1, n / 256 );
}

uint16_t decode2byte( char *ch)
{
    uint16_t n;
    n = decode1byte( ch );
    n+= decode1byte( ch + 1 ) * 256;
    return n;
}

void encode_header( char *buffer, MsgHeader header )
{
    encode1byte( buffer, version_of_our_cool_protocol_over_tcp );
    encode2byte( buffer + 1, header.data_size );
    encode1byte( buffer + 3, header.type );
}

MsgHeader decode_header( char *buffer )
{
    MsgHeader header;
    header.ver = decode1byte( buffer );
    header.data_size = decode2byte( buffer + 1 );
    header.type = decode1byte( buffer + 3 );
    return header;
}

void TCPClientImpl::listen()
{
    for(;;)
    {
        asio::error_code error;
        asio::read(
            this->sock,
            asio::buffer(buffer_in),
            asio::transfer_exactly( msg_header_size ),
            error );

        if(error)
        {
            break;
        }

        MsgHeader header = decode_header( buffer_in );

        asio::read(
            this->sock,
            asio::buffer(this->buffer_in),
            asio::transfer_exactly( header.data_size ),
            error );

        if(error)
        {
            break;
        }

        std::vector<char> data(buffer_in, buffer_in + header.data_size);

        if( this->on_recive )
        {
            this->on_recive( data );
        }
    }

    //event connection closed
    this->connected = false;
}

void TCPClientImpl::send( std::vector<char> data )
{
    MsgHeader header;
    header.data_size = data.size();
    header.type = DataType::custom;

    encode_header( buffer_out, header );

    if( data.size() > max_size_of_package )
    {
        return;
    }

    memcpy( (buffer_out + msg_header_size), data.data(), data.size() );

    asio::error_code error;
    asio::write( 
        sock,
        asio::buffer( buffer_out ),
        asio::transfer_exactly( msg_header_size + data.size() ),
        error
    );

    if(error)
    {
        return;
    }
}

bool TCPClientImpl::is_connected()
{
    return this->connected;
}

bool TCPClientImpl::connect( std::string addr, int port )
{
    asio::error_code error;
    tcp::resolver resolver(io_service);
    
    tcp::resolver::query query( addr, std::to_string(port) );
    tcp::resolver::iterator endpoint_iterator
        = resolver.resolve(query, error);

    if(error)
    {
        return false;
    }

    asio::connect( this->sock, endpoint_iterator, error );

    if(error)
    {
        return false;
    }

    asio::read(
        sock,
        asio::buffer(buffer_in),
        asio::transfer_exactly(4),
        error );

    MsgHeader header = decode_header( buffer_in );

    if( header.ver != version_of_our_cool_protocol_over_tcp )
    {
        strcpy(buffer_out,"er");
        asio::write(
            sock,
            asio::buffer(buffer_out),
            asio::transfer_exactly(2),
            error );
        return false;
    }

    strcpy(buffer_out,"ok");
    asio::write(
        sock,
        asio::buffer(buffer_out),
        asio::transfer_exactly(2),
        error );

    if(error)
    {
        return false;
    }

    listner = std::thread( std::bind(&TCPClientImpl::listen, this) );
    listner.detach();

    this->connected = true;
    return true;
}

void TCPClientImpl::set_recive_callback( TCPClientImpl::Callback on_recive ) {
    this->on_recive = on_recive;
}

TCPClientImpl::TCPClientImpl()
    : sock( io_service )
{

}

re::TCPClientPtr re::TCPClient::create()
{
    re::TCPClientPtr client = std::make_shared<TCPClientImpl>();
    return client;
}

}