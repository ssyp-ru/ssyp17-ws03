#pragma once

#include <string>
#include <memory>
#include <vector>

namespace re
{

typedef std::shared_ptr<class TCPClient> TCPClientPtr;
typedef std::shared_ptr<class TCPServer> TCPServerPtr;

class TCPClient
{
public:
    virtual bool connect( std::string addr, int port ) = 0;
    virtual bool is_connected() = 0;

    virtual void send( std::vector<char> data ) = 0;
    virtual void set_recive_callback( std::function<void(std::vector<char>)> on_recive ) = 0;

    static TCPClientPtr get();
};

class TCPServer
{
public:
    virtual void setup( int port ) = 0;

    virtual int get_client_count() = 0;
    virtual void set_connect_callback( std::function<void(int)> on_client_connect ) = 0;

    virtual void send( int id, std::vector<char> data ) = 0;
    virtual void set_recive_callback( std::function<void(int, std::vector<char>)> on_recive ) = 0;

    static TCPServerPtr get();
};

}