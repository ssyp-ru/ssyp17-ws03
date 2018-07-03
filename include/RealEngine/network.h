#pragma once

#include <string>
#include <memory>
#include <vector>
#include <functional>

namespace re
{

typedef std::shared_ptr<class TCPClient> TCPClientPtr;
typedef std::shared_ptr<class TCPServer> TCPServerPtr;

class TCPClient
{
public:
    typedef std::function<void(std::vector<char>)> Callback;

    virtual bool connect( std::string addr, int port ) = 0;
    virtual bool is_connected() = 0;

    virtual void send( std::vector<char> data ) = 0;
    virtual void set_recive_callback( Callback on_recive ) = 0;

    static TCPClientPtr create();
};

class TCPServer
{
public:
    enum Callback_event {
        connect,
        disconnect,
        msg_recive
    };

    typedef std::function<void(Callback_event,int,std::vector<char>)> Callback;

    virtual void setup( int port ) = 0;

    virtual int get_client_count() = 0;
    virtual void set_callback( Callback ) = 0;
    virtual void send( int id, std::vector<char> data ) = 0;

    static TCPServerPtr create();
};

}