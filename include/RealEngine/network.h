#pragma once

#include <string>
#include <memory>

namespace re
{

typedef std::shared_ptr<class TCPClient> TCPClientPtr;
typedef std::shared_ptr<class TCPServer> TCPServerPtr;

const size_t networkMaxPackage = 4096;

class TCPClient
{
public:
    virtual bool connect( std::string addr, int port ) = 0;
    virtual bool isConnected() = 0;

    virtual void send( std::string data ) = 0;
    std::function<void(std::string)> onRecive;

    static TCPClientPtr get();
};

class TCPServer
{
public:
    virtual void setup( int port ) = 0;

    virtual int getClientCount() = 0;
    std::function<void(int)> onClientConnect;

    virtual void send( int id, std::string data ) = 0;
    std::function<void(int, std::string)> onRecive;

    static TCPServerPtr get();
};

}