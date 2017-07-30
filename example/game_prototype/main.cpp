#include <RealEngine/baseApp.h>
#include <RealEngine/graphic.h>
#include <RealEngine/graphic/image.h>
#include <RealEngine/graphic/animation.h>
#include <RealEngine/tiled_parser.h>
#include <RealEngine/xml_parser.h>
#include <RealEngine/math.h>
#include "RealEngine/physic_core.h"
//#include "RealEngine/resource_manager.h"
#include "hud.h"

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
#include "ability_damageBoost.h"
#include "ability_heal.h"
#include "ability_invincibility.h"

const double SCALE_COEFF = 0.0625;

class MainApp : public re::IBaseApp{
public:
    re::Game mainGame;
    int k = 16; // scale
    re::Map map;
    HUD *curHUD;
    //Setup:

    re::GameObjectPtr getGameObject(re::Vector2f pos, re::Vector2f size)
    {
        return std::make_shared<Platform>(pos, size);
    }

    void setup() override {
        resource_manager.load_file("resources.xml");
        re::AnimationPtr testanimCustom = resource_manager.get_animation("mario_sprite");

        re::AnimationPtr playerAnim = std::make_shared<re::Animation>(0, true);
        re::AnimationPtr jumpAnim = std::make_shared<re::Animation>(0.5, true);
        re::AnimationPtr atkAnim = std::make_shared<re::Animation>(1, true);
        re::AnimationPtr onIceAnim = std::make_shared<re::Animation>(0.5, true);
        
        re::Image spritelist( "data/spritelist.png" );

        playerAnim->add_frame(spritelist.get_subimage(29, 12, 224, 344));
        playerAnim->add_frame(spritelist.get_subimage(298, 11, 224, 344));
        playerAnim->add_frame(spritelist.get_subimage(556, 15, 224, 344));
        playerAnim->add_frame(spritelist.get_subimage(840, 8, 224, 344));
        playerAnim->add_frame(spritelist.get_subimage(17, 381, 224, 344));
        playerAnim->add_frame(spritelist.get_subimage(289, 376, 224, 344));
        playerAnim->add_frame(spritelist.get_subimage(569, 378, 224, 344));
        playerAnim->add_frame(spritelist.get_subimage(854, 374, 224, 344));

        atkAnim->add_frame(spritelist.get_subimage(578, 739, 227, 344)); 
        atkAnim->add_frame(spritelist.get_subimage(307, 735, 244, 344));
        atkAnim->add_frame(spritelist.get_subimage(16, 744, 260, 344));
        atkAnim->add_frame(spritelist.get_subimage(307, 735, 244, 344));
        atkAnim->add_frame(spritelist.get_subimage(578, 739, 227, 344)); 

        onIceAnim->add_frame(spritelist.get_subimage(862, 741, 224, 344));

        jumpAnim->add_frame(spritelist.get_subimage(30, 1139, 224, 344));
        jumpAnim->add_frame(spritelist.get_subimage(315, 1130, 224, 344));
        jumpAnim->add_frame(spritelist.get_subimage(605, 1136, 224, 344));
        jumpAnim->add_frame(spritelist.get_subimage(892, 1143, 224, 344));
                                       

        re::ImagePtr buttonsource = resource_manager.get_image("button_test");
        pauseImage = std::make_shared<re::Image>("whilepaused.png");
        re::BaseButton startgame_btn(50, 50, buttonsource->get_subimage(10, 10, 200, 70));
        re::BaseButton exit_btn(50, 250, buttonsource->get_subimage(10, 200, 200, 70));
        buttonList.push_back(startgame_btn);
        buttonList.push_back(exit_btn);
        //buttonList.push_back(respawn_btn);
        buttonList[0].register_action(std::bind(&MainApp::setState_ingame, this));
        buttonList[1].register_action(std::bind(&MainApp::setState_exit, this));

        map = (re::parse_tiled( re::parse_xml( "map/map_level1.tmx" ) ))[0];

        for( auto object : map.objectgroup[0].group )
        {
            if( object.name == "grass" )
            {
                mainGame.addObject( std::make_shared<Platform>(
                        re::Vector2f(object.x * SCALE_COEFF - 4.1, object.y * SCALE_COEFF), 
                        re::Vector2f((float)object.width * SCALE_COEFF + 0.2, (float)object.height * SCALE_COEFF)));
            } else if( object.name == "ground" )            {
                mainGame.addObject( std::make_shared<Platform>(
                        re::Vector2f(object.x * SCALE_COEFF - 4.1, object.y * SCALE_COEFF), 
                        re::Vector2f((float)object.width * SCALE_COEFF + 0.2, (float)object.height * SCALE_COEFF)));

            } else if ( object.name == "yojus" ) {
                testPlayer = std::make_shared<Player>(re::Vector2f(object.x * SCALE_COEFF, object.y * SCALE_COEFF));
                testPlayer->movingAnim = playerAnim;//testanimCustom;
                testPlayer->slidingAnim = onIceAnim;
                testPlayer->jumpingAnim = jumpAnim;
                testPlayer->attackAnim = atkAnim;
                testPlayer->setFriction(-1.0);
                testPlayer->setBounciness(0.0);
            } else if ( object.name == "ice" ) {
                re::GameObjectPtr platice = std::make_shared<IcePlatform>(
                        re::Vector2f(object.x * SCALE_COEFF - 4.1, object.y * SCALE_COEFF), 
                        re::Vector2f((float)object.width * SCALE_COEFF + 0.2, (float)object.height * SCALE_COEFF));
                mainGame.addObject(platice);
            } else if ( object.name == "corr" ) {
                for(size_t i = 0; i < object.width / 64; i++ )
                {
                    re::GameObjectPtr weplat = std::make_shared<WeakPlatform>( 
                        re::Vector2f(object.x * SCALE_COEFF + ( (i-1) * 4 ), object.y * SCALE_COEFF), 
                        re::Vector2f(4, 4), 0.5);
                    mainGame.addObject(weplat);
                }
            } else if ( object.name == "metal" ) {
                re::GameObjectPtr movplat = std::make_shared<MovingPlatform>(
                        re::Vector2f(object.x * SCALE_COEFF - 4, object.y * SCALE_COEFF + 4), 
                        re::Vector2f(4, 1), 
                        3.0);
                mainGame.addObject(movplat);
                (std::dynamic_pointer_cast<MovingPlatform>(movplat))->path->addWaypoint(re::Vector2f(
                    (object.x * SCALE_COEFF) + 4 + (object.width / 128) * 4,// - 1) * 4,
                    (object.y * SCALE_COEFF) + 4// + ((object.height / 128) - 1) * 4
                ));
                (std::dynamic_pointer_cast<MovingPlatform>(movplat))->path->setCycled(true);
                (std::dynamic_pointer_cast<MovingPlatform>(movplat))->path->setActivated(true);
            } else if ( object.name == "deth" ) {
                re::GameObjectPtr deathTrig = std::make_shared<DeathTrigger>(
                                re::Vector2f(object.x * SCALE_COEFF - 4, object.y * SCALE_COEFF), 
                                re::Vector2f((float)object.width * SCALE_COEFF + 0.1, (float)object.height * SCALE_COEFF));
                mainGame.addObject(deathTrig);
            }
        }

        mainGame.addObject(testPlayer);
        testPlayer->addAbility(new Ability_DamageBoost(5, 2));
        testPlayer->addAbility(new Ability_Heal(10));
        testPlayer->addAbility(new Ability_Invincibility(5));

        curHUD = new HUD(std::dynamic_pointer_cast<Player>(testPlayer).get(), &resource_manager);

        /*re::GameObjectPtr plat = std::make_shared<Platform>(re::Vector2f(0, 20), re::Vector2f(20, 2));
        mainGame.addObject(plat);

        re::GameObjectPtr plat2 = std::make_shared<MovingPlatform>(re::Vector2f(15, 16), re::Vector2f(3, 0.5), 3.0);
        mainGame.addObject(plat2);
        (std::dynamic_pointer_cast<MovingPlatform>(plat2))->path->addWaypoint(re::Vector2f(20, 16));
        (std::dynamic_pointer_cast<MovingPlatform>(plat2))->path->setCycled(true);
        (std::dynamic_pointer_cast<MovingPlatform>(plat2))->path->setActivated(true);

        re::GameObjectPtr plat3 = std::make_shared<WeakPlatform>(re::Vector2f(10, 16), re::Vector2f(5, 0.5), 1.0);
        mainGame.addObject(plat3);

        re::GameObjectPtr plat4 = std::make_shared<Platform>(re::Vector2f(48, 48), re::Vector2f(27, 2));
        plat4->setRigidbodySimulated(false);
        plat4->setFriction(1.0);
        plat4->setBounciness(0.0);
        mainGame.addObject(plat4);

        re::GameObjectPtr plat5 = std::make_shared<Platform>(re::Vector2f(48, 48), re::Vector2f(27, 2));
        plat5->setRigidbodySimulated(false);
        plat5->setFriction(1.0);
        plat5->setBounciness(0.0);
        mainGame.addObject(plat5);
        */
        curState = AppState::MainMenu;

        //re::scale(0.5);
        //re::view_at(0,0);
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

            //re::view_at( (int)((testPlayer->getPosition().X * 16 + 64 * 16) / (64 * 16) ) * 64 * 16 - 64 * 17,//(testPlayer->getPosition().X * 16 / 16) * 1000,
            //             (int)((testPlayer->getPosition().Y * 16) / (64 * 9) ) * 64 * 9 );//(testPlayer->getPosition().Y * 16 / 9 ) * 1000);

            int cam_x, cam_y;
            cam_x = testPlayer->getPosition().X * 16;
            cam_y = testPlayer->getPosition().Y * 16;
            cam_x+= 64 * 17;
            cam_x/= 64 * 16;
            cam_x*= 64 * 16;
            cam_x-= 64 * 17;

            cam_y/= 64 * 9;
            cam_y*= 64 * 9;
            //don't show it to Kolya, srsly.
            //Kolya, if you saw it, pls, don't ask us to do pushups.

            //re::Vector2f pos = doBlackMagic( re::Vector2f(cam_x,cam_y) );

            re::view_at( cam_x, cam_y );

            //re::view_at(  );

            re::draw_image_part(0,0, 
                            3072,
                            1728, 
                            0,0, 1,1, 
                            map.layer[0].background);

            for (auto curObject : mainGame.getWorld())
                (std::dynamic_pointer_cast<DrawableGameObject>(curObject))->update();

            for (auto curObject : mainGame.getWorld())
                (std::dynamic_pointer_cast<DrawableGameObject>(curObject))->display(k);

            curHUD->display();
            break;
        case AppState::MainMenu:
            re::background(re::WHITE); 
            /*for (auto& btn : buttonList){
                btn.display();
            }*/
            for(int i = 0; i < 2; i++){
                buttonList[i].display();
            }
            break;
        case AppState::Dead:
            re::background(re::WHITE);
            buttonList[2].display();
            break;
        case AppState::Pause:
            re::draw_image(-64, 0, pauseImage);
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
                setState_pause();
            }
            else{
            exit(0);
            }
        }
        else if (curState == AppState::Pause){
            setState_ingame();
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
         *//*
        if(button == 0 && (curState == AppState::MainMenu || curState==AppState::Pause)){
            for (auto& btn : buttonList){
                if(btn.check_if_mouse_over(curX, curY)){
                    btn.action(button);
                }
            }
        }*/
        if(button == 0){
            switch(curState){
                case AppState::MainMenu:
                for(int i = 0; i < 2; i++){
                    if(buttonList[i].check_if_mouse_over(curX, curY)){
                        buttonList[i].action(button);
                    }
                }
                break;
                case AppState::Pause:
                    setState_ingame();
                break;
                case AppState::Dead:
                if(buttonList[2].check_if_mouse_over(curX, curY)){
                    buttonList[2].action(button);
                }
                break;
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

    void setState_dead(){
        curState = AppState::Dead;
    }

    void respawn(){
        //testPlayer->respawn();
        curState = AppState::Ingame;
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
    re::ImagePtr pauseImage;
    re::ResourceManager resource_manager;
};

int main(){
    re::runApp( 1024, 576, std::make_shared<MainApp>() );
    return 0;
}