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

    void reciveMsgClient(std::string data ) {
        this->lastMsg = data;
    }

    void reciveMsgServer( std::string data, int id ) {
        this->lastMsg = data;
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
        switch(key)
        {
        case re::Key::A:
            this->tcpClient = re::TCPClient::get();
            this->tcpClient->connect( "127.0.0.1", 11999 );
            this->tcpClient->onRecive = std::bind( &MainApp::reciveMsgClient, this, std::placeholders::_1 );
            lastMsg = "client";
            break;
        case re::Key::S:
            this->tcpClient->send("test client");
            break;
        case re::Key::Z:
            this->tcpServer = re::TCPServer::get();
            this->tcpServer->setup( 11999 );
            lastMsg = "server";
            //this->tcpServer->onRecive = std::bind( &MainApp::reciveMsgServer, this, 1, std::placeholders::_1 );
            break;
        case re::Key::X:
            this->tcpServer->send( 0, "test server" );
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
