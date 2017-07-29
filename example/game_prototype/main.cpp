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
#include <cmath>
#include "RealEngine/physic_core.h"
#include "platform.h"

class MainApp : public re::IBaseApp{
public:
    re::Game mainGame;
    int k = 16; // scale
    //Setup:

    re::GameObjectPtr getGameObject(re::Vector2f pos, re::Vector2f size)
    {
        return std::make_shared<Platform>(pos, size);
    }

    void setup() override {
        re::Animation testanimCustom(0, true);
        re::Image buttonsource("test.png");
        re::Image spritelist("spritelist.png");
        for (int i = 0; i < 9; i++){
            testanimCustom.add_frame(spritelist.get_subimage(3 + 17*i, 5, 16, 25));
        }
        testPlayer = std::make_shared<Player>(re::Vector2f(5, 15), re::Vector2f(1, 1.5));
        testPlayer->movingAnim = testanimCustom;
        testPlayer->setRigidbodySimulated(true);
        //testPlayer->addImpulse(re::Vector2f(4, 0));
        testPlayer->setFriction(-1.0);
        testPlayer->setBounciness(0.0);
        testPlayer->addCollisionCallback(testPlayer->getCallback());
        mainGame.addObject(testPlayer);

        re::GameObjectPtr plat = std::make_shared<Platform>(re::Vector2f(0, 20), re::Vector2f(20, 2));
        plat->setRigidbodySimulated(false);
        plat->setFriction(1.0);
        plat->setBounciness(0.0);
        mainGame.addObject(plat);

        re::GameObjectPtr plat2 = std::make_shared<Platform>(re::Vector2f(15, 16), re::Vector2f(2, 2));
        plat2->setRigidbodySimulated(false);
        plat2->setFriction(1.0);
        plat2->setBounciness(0.0);
        mainGame.addObject(plat2);

        curState = AppState::Ingame;
    }

    void update() override {
        switch(curState){
            case AppState::Ingame:
                mainGame.getWorld()[0]->addForce(re::Vector2f(0, 20 * mainGame.getWorld()[0]->getMass()));
                break;
            case AppState::Exit:
                exit(0);
        }  
    }

    void display() override 
    {
        if (re::get_key_state(re::Key::D))
            testPlayer->setVelocity(re::Vector2f(5 * testPlayer->getMass(), testPlayer->getVelocity().Y));
        if (re::get_key_state(re::Key::A))
            testPlayer->setVelocity(re::Vector2f(-5 * testPlayer->getMass(), testPlayer->getVelocity().Y));

        if ((testPlayer->isGrounded) && ((re::get_key_state(re::Key::D)) || (re::get_key_state(re::Key::A))))
            testPlayer->movingAnim.setSpeed(0.5);
        else
            testPlayer->movingAnim.setSpeed(0);

        if (re::get_key_state(re::Key::W))
        {
            if (testPlayer->isGrounded)
            {
                testPlayer->addImpulse(re::Vector2f(0, -15 * testPlayer->getMass()));
                testPlayer->isGrounded = false;
            }
        }
        if ((testPlayer->getVelocity().Y > 0.1) && (testPlayer->isGrounded)) testPlayer->isGrounded = false;

        mainGame.updateTick();

        testPlayer->display(k);
        for (auto curObject : mainGame.getWorld())
        {
            if (curObject != testPlayer)
                (std::dynamic_pointer_cast<DrawableGameObject>(curObject))->display(k);
        }
    }
    //Events:
    void on_key_released(re::Key key){
        //std::cout << "Key release\n";
        if(key == re::Key::Right || key == re::Key::Left){
            
        }
    }

    void on_key_pressed(re::Key key){
        //std::cout << "Key pressed\n";
        if (key == re::Key::Escape){
            exit(0);
        }
        if (key == re::Key::Down){
            
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
    std::shared_ptr<Player> testPlayer;
    AppState curState;
    std::vector<re::BaseButton> buttonList;
};

int main(){
    re::runApp( 400, 400, std::make_shared<MainApp>() );
    return 0;
}