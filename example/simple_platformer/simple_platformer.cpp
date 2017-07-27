#include <RealEngine/base_app.h>
#include <RealEngine/graphic.h>
#include <RealEngine/graphic/image.h>
#include <RealEngine/graphic/animation.h>
#include "player.h"
#include "app_state.h"
#include "base_button.h"
#include <RealEngine/math.h>
#include <functional>
#include <string>
#include <iostream>
#include <chrono>
#include <vector>

class MainApp : public re::IBaseApp{
public:

    //Setup:
    void setup() override {
        re::set_fps(60);
        re::Animation testanimCustom(0, true);
        re::Image buttonsource("test.png");
        re::Image spritelist("spritelist.png");
        for (int i = 0; i < 9; i++){
            testanimCustom.add_frame(spritelist.get_subimage(3 + 17*i, 5, 16, 25));
            //imgptr = std::make_shared<re::Image>("test.png")
            //testanimCustom.add_frame(std::make_shared<re::Image>("anim" + std::to_string(i) + ".png"));
        }
        re::BaseButton startgame_btn(50, 50, buttonsource.get_subimage(10, 10, 200, 70));
        re::BaseButton exit_btn(50, 250, buttonsource.get_subimage(10, 200, 200, 70));
        buttonList.push_back(startgame_btn);
        buttonList.push_back(exit_btn);
        buttonList[0].register_action(std::bind(&MainApp::setState_ingame, this/*, std::placeholders::_1*/));
        buttonList[1].register_action(std::bind(&MainApp::setState_exit, this/*, std::placeholders::_1*/));
        /*for (auto& btn : buttonList){
            btn.register_action(std::bind(&MainApp::state_change, this, std::placeholders::_1));
        }*/
        Player newPlayer(100, 100, 10, testanimCustom);
        testPlayer = newPlayer;   
        curState = AppState::MainMenu;
        curX = 0;
        curY = 0; 
    }

    void update() override {
      //  x += 0;
       // y += 5;
        //if (isMovingForward) x += testanimCustom.getSpeed()*10;
        //else x -= testanimCustom.getSpeed()*10;
        //customAnimSpeed += 0.05;
        //if (customAnimSpeed > 3) customAnimSpeed = 0;
        //if (y > 380) y = 0;
        //if (x > 380) x = -100;
      //  if (x < -100) x = 380;
        switch(curState){
            case AppState::Ingame:
            testPlayer.updatePosition();
            break;
            case AppState::Exit:
            exit(0);
            break;
        }  
    }
    void display() override {
        switch(curState){
            //re::draw_image(100+x, 100, testanimCustom.getNextFrame());
            case AppState::Ingame:
            re::background(re::WHITE); // Clears all, each drawn pic is placed forever.
            re::draw_rectangle(0, 126, 400, 2, re::BLACK);
            re::draw_rectangle(curX, curY, 5, 5, re::BLACK);
            testPlayer.display();
            break;
            case AppState::MainMenu:
            for (auto& btn : buttonList){
                btn.display();
            }
            break;
        }
    }
    //Events:
    void on_key_released(re::Key key){
        std::cout << "Key release\n";
        if(key == re::Key::Right || key == re::Key::Left){
            testPlayer.stop();
        }
    }

    void on_key_pressed(re::Key key){
        std::cout << "Key pressed\n";
        if (key == re::Key::Escape){
            exit(0);
        }
        if (key == re::Key::Right){
            isMovingForward = true;
            testPlayer.move(isMovingForward);
            re::Point2f curCoords = testPlayer.getCurPosition();
            std::cout << "x: " << curCoords.x << " y: " << curCoords.y << '\n';
        }
        if (key == re::Key::Left){
            isMovingForward = false;
            testPlayer.move(isMovingForward);
            re::Point2f curCoords = testPlayer.getCurPosition();
            std::cout << "x: " << curCoords.x << " y: " << curCoords.y << '\n';
        }
    }

    void on_button_pressed(int button){
        /*magic-shmagic button constants:
        1: left-mouse click,
        2: mid button click,
        3: right-mouse click,
        4: scroll up,
        5: scroll down.*/
        if(button == 1){
            for (auto& btn : buttonList){
                if(btn.check_if_mouse_over(curX, curY)){
                    btn.action(button);
                }
            }
        }
    }

    void on_mouse_move(int mouseX, int mouseY){
        curX = mouseX;
        curY = mouseY;
    }

    //Callbacks:
    void setState_ingame(){
        curState = AppState::Ingame;
        std::cout << "main app state change" << std::endl;
    }

    void setState_exit(){
        curState = AppState::Exit;
        std::cout << "main app state change" << std::endl;
    }

private:
    int x;
    int y;
    bool isMovingForward = true;
    int curX;
    int curY;
    Player testPlayer;
    AppState curState;
    std::vector<re::BaseButton> buttonList;
};

int main(){
    re::init_graphics(re::GraphicalBackend::XCB, 400, 400);
    re::run_base_app(new MainApp());
    return 0;
}