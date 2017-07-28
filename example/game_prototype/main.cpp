#include <RealEngine/baseApp.h>
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
        re::Animation testanimCustom(0, true);
        re::Image buttonsource("test.png");
        re::Image spritelist("spritelist.png");
        for (int i = 0; i < 9; i++){
            testanimCustom.add_frame(spritelist.get_subimage(3 + 17*i, 5, 16, 25));
            //imgptr = std::make_shared<re::Image>("test.png")
            //testanimCustom.add_frame(std::make_shared<re::Image>("anim" + std::to_string(i) + ".png"));
        }
        Player newPlayer(100, 100, 10, testanimCustom);
        testPlayer = newPlayer;   
        curState = AppState::Ingame;
        curX = 0;
        curY = 0; 
    }

    void update() override {
        switch(curState){
            case AppState::Ingame:
                testPlayer.updatePosition();
                break;
            case AppState::Exit:
                exit(0);
        }  
    }
    void display() override {
        switch(curState){
            //re::draw_image(100+x, 100, testanimCustom.getNextFrame());
            case AppState::Ingame:
                background(re::WHITE); // Clears all, each drawn pic is placed forever.
                draw_rectangle(0, 126, 400, 2, re::BLACK);
                draw_rectangle(curX, curY, 5, 5, re::BLACK);
                testPlayer.display();
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
    re::runApp( 400, 400, std::make_shared<MainApp>() );
    return 0;
}