#pragma once

#include "RealEngine/network.h"
#include <asio.hpp>
#include <thread>

namespace re
{

const size_t max_size_of_package = 4096;

enum DataType
{
    event,
    custom
};

struct MsgHeader
{
    uint16_t size;
    uint8_t type;
};

void encode_header( char *buffer, MsgHeader header );
MsgHeader decode_header( char *buffer );

static size_t msg_header_size = sizeof(uint16_t) + sizeof(uint8_t);

class TCPClientImpl : public TCPClient {
public:
    TCPClientImpl();
    virtual bool connect( std::string addr, int port ) override;
    virtual bool is_connected() override;
    void listen();
    virtual void send( std::vector<char> data ) override;
    virtual void set_recive_callback( Callback on_recive ) override;
private:
    bool connected = false;
    asio::io_service io_service;
    asio::ip::tcp::socket sock;
    char buffer_in[max_size_of_package];
    char buffer_out[max_size_of_package]; 
    std::thread listner;
    Callback on_recive;
};

class TCPServerImpl : public TCPServer {
public:
    TCPServerImpl();
    virtual void setup( int port ) override;
    virtual int get_client_count() override;

    virtual void send( int id, std::vector<char> data ) override;
    virtual void set_callback( Callback ) override;

    void accept();
    void listen( int id );
private:
    Callback callback;

    std::vector<std::shared_ptr<asio::ip::tcp::socket>> clients;
    asio::io_service io_service;
    asio::ip::tcp::acceptor tcp_acceptor;
};

}