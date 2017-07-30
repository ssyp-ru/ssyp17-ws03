#include <RealEngine/baseApp.h>
#include <RealEngine/graphic.h>
#include <RealEngine/graphic/image.h>
#include <RealEngine/graphic/animation.h>
#include <RealEngine/tiled_parser.h>
#include <RealEngine/xml_parser.h>
#include <RealEngine/math.h>
#include "RealEngine/physic_core.h"
#include "RealEngine/resource_manager.h"

#include <functional>
#include <string>
#include <iostream>
#include <chrono>
#include <vector>
#include <cmath>

#include "player.h"
#include "app_state.h"
#include "base_button.h"
#include "platform.h"
#include "movingPlatform.h"
#include "weakPlatform.h"
#include "evilBird.h"
#include "icePlatform.h"
#include "spikes.h"
#include "deathTrigger.h"
#include "jumpPlatform.h"


const double SCALE_COEFF = 0.031275;

class MainApp : public re::IBaseApp{
public:
    re::Game mainGame;
    int k = 16; // scale
    re::Map map;
    //Setup:

    re::GameObjectPtr getGameObject(re::Vector2f pos, re::Vector2f size)
    {
        return std::make_shared<Platform>(pos, size);
    }

    void setup() override {
        resource_manager.load_file("resources.xml");
        re::AnimationPtr testanimCustom = resource_manager.get_animation("mario_sprite");

        re::AnimationPtr playerAnim = std::make_shared<re::Animation>(0, true);
        re::Image spritelist( "data/spritelist.png" );

        playerAnim->add_frame(spritelist.get_subimage(29, 13, 224, 343));
        playerAnim->add_frame(spritelist.get_subimage(298, 12, 224, 343));
        playerAnim->add_frame(spritelist.get_subimage(556, 16, 224, 343));
        playerAnim->add_frame(spritelist.get_subimage(840, 9, 224, 343));
        playerAnim->add_frame(spritelist.get_subimage(17, 382, 224, 343));
        playerAnim->add_frame(spritelist.get_subimage(289, 377, 224, 343));
        playerAnim->add_frame(spritelist.get_subimage(569, 379, 224, 343));
        playerAnim->add_frame(spritelist.get_subimage(854, 375, 224, 343));
        re::ImagePtr buttonsource = resource_manager.get_image("button_test");
        re::BaseButton startgame_btn(50, 50, buttonsource->get_subimage(10, 10, 200, 70));
        re::BaseButton exit_btn(50, 250, buttonsource->get_subimage(10, 200, 200, 70));
        buttonList.push_back(startgame_btn);
        buttonList.push_back(exit_btn);
        buttonList[0].register_action(std::bind(&MainApp::setState_ingame, this));
        buttonList[1].register_action(std::bind(&MainApp::setState_exit, this));

        map = (re::parse_tiled( re::parse_xml( "map/untitled.tmx" ) ))[0];

        for( auto object : map.objectgroup[0].group )
        {
            if( object.name == "grass" )
            {
                mainGame.addObject( std::make_shared<Platform>(
                        re::Vector2f(object.x * SCALE_COEFF, object.y * SCALE_COEFF), 
                        re::Vector2f((float)object.width * SCALE_COEFF, (float)object.height * SCALE_COEFF)));
            } else if( object.name == "ground" )            {
                mainGame.addObject( std::make_shared<Platform>(
                        re::Vector2f(object.x * SCALE_COEFF, object.y * SCALE_COEFF), 
                        re::Vector2f((float)object.width * SCALE_COEFF, (float)object.height * SCALE_COEFF)));

            } else if ( object.name == "yojus" ) {
                testPlayer = std::make_shared<Player>(re::Vector2f(object.x * SCALE_COEFF, object.y * SCALE_COEFF));
                testPlayer->movingAnim = playerAnim;//testanimCustom;
                testPlayer->setFriction(-1.0);
                testPlayer->setBounciness(0.0);
            } else if ( object.name == "ice" ) {
                re::GameObjectPtr platice = std::make_shared<IcePlatform>(
                        re::Vector2f(object.x * SCALE_COEFF, object.y * SCALE_COEFF), 
                        re::Vector2f((float)object.width * SCALE_COEFF, (float)object.height * SCALE_COEFF));
                mainGame.addObject(platice);
            } else if ( object.name == "corr" ) {
                for( int i = 0; i < object.width / 128; i++ )
                {
                    re::GameObjectPtr weplat = std::make_shared<WeakPlatform>( 
                        re::Vector2f(object.x * SCALE_COEFF + ( i * 4 ), object.y * SCALE_COEFF), 
                        re::Vector2f(4, 4), 0.5);
                    mainGame.addObject(weplat);
                }
            } else if ( object.name == "metal" ) {
                re::GameObjectPtr movplat = std::make_shared<MovingPlatform>(
                        re::Vector2f(object.x * SCALE_COEFF, object.y * SCALE_COEFF), 
                        re::Vector2f(4, 1), 
                        3.0);
                mainGame.addObject(movplat);
                (std::dynamic_pointer_cast<MovingPlatform>(movplat))->path->addWaypoint(re::Vector2f(
                    (object.x * SCALE_COEFF) + ((object.width / 128) - 1) * 4,
                    (object.y * SCALE_COEFF) + ((object.height / 128) - 1) * 4
                ));
                (std::dynamic_pointer_cast<MovingPlatform>(movplat))->path->setCycled(true);
                (std::dynamic_pointer_cast<MovingPlatform>(movplat))->path->setActivated(true);
            }
        }

        mainGame.addObject(testPlayer);

        /*re::GameObjectPtr plat = std::make_shared<Platform>(re::Vector2f(0, 20), re::Vector2f(20, 2));
        mainGame.addObject(plat);

        re::GameObjectPtr plat2 = std::make_shared<MovingPlatform>(re::Vector2f(15, 16), re::Vector2f(3, 0.5), 3.0);
        mainGame.addObject(plat2);
        (std::dynamic_pointer_cast<MovingPlatform>(plat2))->path->addWaypoint(re::Vector2f(20, 16));
        (std::dynamic_pointer_cast<MovingPlatform>(plat2))->path->setCycled(true);
        (std::dynamic_pointer_cast<MovingPlatform>(plat2))->path->setActivated(true);

        re::GameObjectPtr plat3 = std::make_shared<WeakPlatform>(re::Vector2f(10, 16), re::Vector2f(3, 0.5), 1.0);
        mainGame.addObject(plat3);

        re::GameObjectPtr plat4 = std::make_shared<IcePlatform>(re::Vector2f(20, 21), re::Vector2f(10, 1));
        mainGame.addObject(plat4);

        re::GameObjectPtr plat5 = std::make_shared<Platform>(re::Vector2f(30, 21), re::Vector2f(5, 1));
        mainGame.addObject(plat5);

        re::GameObjectPtr plat6 = std::make_shared<JumpPlatform>(re::Vector2f(-6, 20), re::Vector2f(5, 1), 20);
        mainGame.addObject(plat6);

        re::GameObjectPtr bird = std::make_shared<EvilBird>(re::Vector2f(10, 17), 2);
        mainGame.addObject(bird);
        (std::dynamic_pointer_cast<EvilBird>(bird))->path->addWaypoint(re::Vector2f(12, 18));
        (std::dynamic_pointer_cast<EvilBird>(bird))->path->addWaypoint(re::Vector2f(14, 17));
        (std::dynamic_pointer_cast<EvilBird>(bird))->path->setCycled(false);
        (std::dynamic_pointer_cast<EvilBird>(bird))->path->setActivated(true);

        re::GameObjectPtr spikes = std::make_shared<Spikes>(re::Vector2f(32, 20), re::Vector2f(1, 1), 50.0, 1.0);
        mainGame.addObject(spikes);
        */
        //re::GameObjectPtr deathTrig = std::make_shared<DeathTrigger>(re::Vector2f(-5, 30), re::Vector2f(50, 1));
        //mainGame.addObject(deathTrig);

        curState = AppState::Ingame;
    }

    void update() override {
        switch(curState){
            case AppState::Ingame:
                break;
            case AppState::Exit:
                exit(0);
        }  
    }

    void display() override 
    {   
        switch(curState){
        case AppState::Ingame:
            mainGame.updateTick();

            re::view_at( testPlayer->getPosition().X * 16 - 500,
                         testPlayer->getPosition().Y * 16 - 300 );
            re::draw_image_part(0,0, 
                            1280,
                            1280, 
                            0,0, 1,1, 
                            map.layer[0].background);

            for (auto curObject : mainGame.getWorld())
                (std::dynamic_pointer_cast<DrawableGameObject>(curObject))->update();

            for (auto curObject : mainGame.getWorld())
                (std::dynamic_pointer_cast<DrawableGameObject>(curObject))->display(k);
            break;
        case AppState::MainMenu:
            re::background(re::WHITE); 
            for (auto& btn : buttonList){
                btn.display();
            }
            break;
        case AppState::Pause:
            re::background(re::WHITE);         
            for (auto& btn : buttonList){
                btn.display();
            }
            break;
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
        if (key == re::Key::Escape){ //255 = escape, and also "delete", dat opengl :(
            if(curState == AppState::Ingame){
                std::cout << "pause setter triggered" << std::endl;
                setState_pause();
            }
            /*else{
            exit(0);
            }*/
        }
        if (key == re::Key::Down){
            
        }
    }

    void on_button_pressed(int button){
        /*
         * magic-shmagic button constants:
         * 0: left-mouse click,
         * 1: mid button click,
         * 2: right-mouse click,
         * 3: scroll up,
         * 4: scroll down.
         */
        if(button == 0 && curState == AppState::MainMenu || curState==AppState::Pause){
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

    void setState_pause(){
        curState = AppState::Pause;
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

    re::ResourceManager resource_manager;
};

int main(){
    re::runApp( 1024, 576, std::make_shared<MainApp>() );
    return 0;
}