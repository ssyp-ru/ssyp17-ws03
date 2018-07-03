#include <RealEngine/baseApp.h>
#include <RealEngine/graphic.h>
#include <RealEngine/network.h>

#include <iostream>

class MainApp : public re::IBaseApp{
public:
    re::TCPClientPtr tcpClient;
    re::TCPServerPtr tcpServer;
    std::string lastMsg;

    void setup() override {
        re::set_fps(60);
        re::background(re::WHITE);
    }

    void reciveMsgClient(std::vector<char> data ) {
        for( auto ch : data )
        {
            this->lastMsg += ch;
        }
    }

    void reciveMsgServer( re::TCPServer::Callback_event event_type, int id, std::vector<char> data ) {
        if(event_type != re::TCPServer::Callback_event::msg_recive)
        {
            return;
        }
        for( auto ch : data )
        {
            this->lastMsg += ch;
        }
    }

    void update() override {
    }
    void display() override {
        re::draw_text( 
            re::Point2f(100,100),
            this->lastMsg,
            re::Color(0,0,0)
        );
    }

    void on_key_pressed(re::Key key){
        std::string test_string("test from client");
        switch(key)
        {
        case re::Key::A:
            this->tcpClient = re::TCPClient::get();
            this->tcpClient->connect( "127.0.0.1", 11999 );
            this->tcpClient->set_recive_callback( std::bind( &MainApp::reciveMsgClient, this, std::placeholders::_1 ) );
            lastMsg = "client";
            break;
        case re::Key::S:
            this->tcpClient->send( std::vector<char>( test_string.c_str(), test_string.c_str() + test_string.size() ) );
            break;
        case re::Key::Z:
            this->tcpServer = re::TCPServer::get();
            this->tcpServer->setup( 11999 );
            lastMsg = "server";
            this->tcpServer->set_callback( std::bind( &MainApp::reciveMsgServer, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 ) );
            break;
        case re::Key::X:
            std::string msg("test from server");
            this->tcpServer->send( 0, std::vector<char>( msg.c_str(), msg.c_str() + msg.size() ) );
            break;
        }
    }

    void on_mouse_move( int x0, int y0 )
    {
    }
};

int main(){
    re::setWindowName( "RealEngine" );
    re::runApp( 640, 480, std::make_shared<MainApp>() );
    return 0;
}
