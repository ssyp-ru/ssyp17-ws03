#pragma once

#include "RealEngine/network.h"
#include <asio.hpp>
#include <thread>

enum DataType
{
    event,
    custom
};

struct MsgHeader
{
    unsigned short int size;
    unsigned char type;
};

class TCPClientImpl : public re::TCPClient {
public:
    TCPClientImpl();
    virtual bool connect( std::string, int ) override;
    virtual bool isConnected() override;
    void listen();
    virtual void send( std::string ) override;
private:
    bool connected = false;
    asio::io_service io_service;
    asio::ip::tcp::socket sock;
    char bufferIn[re::networkMaxPackage];
    char bufferOut[re::networkMaxPackage]; 
    std::thread listner;
};

class TCPServerImpl : public re::TCPServer {
public:
    TCPServerImpl();
    virtual void setup( int port ) override;
    virtual int getClientCount() override;

    virtual void send( int id, std::string data ) override;

    void accept();
    void listen( int id );
private:
    std::vector<asio::ip::tcp::socket*> clients;
    asio::io_service io_service;
    asio::ip::tcp::acceptor tcp_acceptor;
};