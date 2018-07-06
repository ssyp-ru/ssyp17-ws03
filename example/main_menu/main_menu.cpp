//#pragma once
#include <RealEngine/baseApp.h>
#include <RealEngine/graphic.h>
#include <RealEngine/math.h>
#include <RealEngine/math.h>
#include <RealEngine/gui/base_button.h>
#include <RealEngine/gui/gui_manager.h>

#include <memory>

#include <iostream>
int counter;

void CounterPlus()
{
    counter++;
    //std::cout << "g\n";
}
class MainApp : public re::IBaseApp{
    private:
    void Join(){
        std::cout << "Join Game" << std::endl;
        connectButton->set_active(true);
        joinGame->set_active(false);
        ipButton->set_active(true);
    }

    void Create(){
        std::cout << "Create Game" << std::endl;
        guiManager.layer_set_active("main_menu", false);
        guiManager.layer_set_active("select_side", true);
    }

    void Exit(){
        std::cout << "Exit Game" << std::endl;
        re::exitApp();
    }

    void Empty(){
        std::cout << "empty button" << std::endl;
        changeNick->set_active(true);
    }
    void Empty2(){
        std::cout << "empty button" << std::endl;
    }

     void Change(){
        std::cout << "change nick" << std::endl;
        changeNick->set_active(false);
    }
     void Connect(){
        std::cout << "connect to server" << std::endl;
        connectButton->set_active(false);
        ipButton->set_active(false);
        joinGame->set_active(true);
        guiManager.layer_set_active("main_menu", false);
        guiManager.layer_set_active("select_side", true);
        goButton->set_active(false);
    }

    void Go(){
        std::cout << "GO! GO! GO!" << std::endl;
    }


    void Dark(){
        std::cout << "dark side" << std::endl;
    }

    void Bright(){
        std::cout << "bright side" << std::endl;
    }
public:
    void setup() override {
        menuBackground = std::make_shared<re::Image>("menu.png");
        players = std::make_shared<re::Image>("players.png");




        re::ImagePtr joinIcon = std::make_shared<re::Image>("join.png");
        joinGame = std::make_shared<re::BaseButton>(20, 220, "join", joinIcon, joinIcon);
        joinGame->register_action(std::bind(&MainApp::Join, this));
        guiManager.register_button(joinGame, "main_menu");

        re::ImagePtr createIcon = std::make_shared<re::Image>("create.png");
        createGame = std::make_shared<re::BaseButton>(20, 520, "create", createIcon, createIcon);
        createGame->register_action(std::bind(&MainApp::Create, this));
        guiManager.register_button(createGame, "main_menu");

        re::ImagePtr exitIcon = std::make_shared<re::Image>("exit.png");
        exitGame = std::make_shared<re::BaseButton>(20, 720, "exit", exitIcon, exitIcon);
        exitGame->register_action(std::bind(&MainApp::Exit, this));
        guiManager.register_button(exitGame, "main_menu");

        re::ImagePtr emptyIcon = std::make_shared<re::Image>("empty.png");
        emptyGame = std::make_shared<re::BaseButton>(20, 20, "empty", emptyIcon, emptyIcon);
        emptyGame->register_action(std::bind(&MainApp::Empty, this));
        guiManager.register_button(emptyGame, "main_menu");

        re::ImagePtr changeIcon = std::make_shared<re::Image>("change.png");
        changeNick = std::make_shared<re::BaseButton>(20, 120, "change", changeIcon, changeIcon);
        changeNick->register_action(std::bind(&MainApp::Change, this));
        guiManager.register_button(changeNick, "main_menu");
        changeNick->set_active(false);
        

        re::ImagePtr connectIcon = std::make_shared<re::Image>("connect.png");
        connectButton = std::make_shared<re::BaseButton>(20, 320, "connect", connectIcon, connectIcon);
        connectButton->register_action(std::bind(&MainApp::Connect, this));
        guiManager.register_button(connectButton, "main_menu");
        connectButton->set_active(false);

        re::ImagePtr ipIcon = std::make_shared<re::Image>("empty.png");
        ipButton = std::make_shared<re::BaseButton>(20, 220, "ip", ipIcon, ipIcon);
        ipButton->register_action(std::bind(&MainApp::Empty2, this));
        guiManager.register_button(ipButton, "main_menu");
        ipButton->set_active(false);


        re::ImagePtr darkIcon = std::make_shared<re::Image>("dark.png");
        dark = std::make_shared<re::BaseButton>(20, 760, "dark", darkIcon, darkIcon);
        dark->register_action(std::bind(&MainApp::Dark, this));
        guiManager.register_button(dark, "select_side");


        re::ImagePtr brightIcon = std::make_shared<re::Image>("bright.png");
        bright = std::make_shared<re::BaseButton>(20, 360, "bright", brightIcon, brightIcon);
        bright->register_action(std::bind(&MainApp::Bright, this));
        guiManager.register_button(bright, "select_side");


        re::ImagePtr goIcon = std::make_shared<re::Image>("go.png");
        goButton = std::make_shared<re::BaseButton>(420, 360, "go", goIcon, goIcon);
        goButton->register_action(std::bind(&MainApp::Go, this));
        guiManager.register_button(goButton, "select_side");

        guiManager.layer_set_active("select_side", false);

    }

    void update() override {
        
    }
    void display() override { 
        re::draw_image(0, 0, menuBackground);
        guiManager.display(mouseX, mouseY);  
        if(dark->is_active()){
            re::draw_image(20, 20, players);
            re::draw_image(20, 420, players);
        }      
    }

    
    void on_mouse_move( int x0, int y0 ){
        mouseX = x0;
        mouseY = y0;
   }

    void on_button_pressed(int button){
        guiManager.on_click(button, mouseX, mouseY);

    }

private:
re::GuiManager guiManager;
re::BaseButtonPtr joinGame;
re::BaseButtonPtr createGame;
re::BaseButtonPtr exitGame;
re::BaseButtonPtr emptyGame;
re::BaseButtonPtr changeNick;
re::BaseButtonPtr connectButton;
re::BaseButtonPtr ipButton;
re::BaseButtonPtr dark;
re::BaseButtonPtr bright;
re::BaseButtonPtr goButton;
int mouseX, mouseY;
re::ImagePtr menuBackground;
re::ImagePtr players;
    
};

int main(){ 
    re::setWindowName( "RealEngine" );
    re::runApp( 1600, 900, std::make_shared<MainApp>() );
    return 0;
}
