#include <RealEngine/baseApp.h>
#include <RealEngine/graphic.h>
#include <RealEngine/graphic/image.h>
#include <RealEngine/graphic/animation.h>
#include <RealEngine/tiled_parser.h>
#include <RealEngine/xml_parser.h>
#include <RealEngine/math.h>
#include "RealEngine/physic_core.h"
#include "RealEngine/gui/base_button.h"
#include "RealEngine/gui/gui_manager.h"
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

    void map_objects_init(){
        map = re::get_tmx_map("map/map_level1.tmx")[0];

        for( auto object : map.objectgroup[0].group )
        {
            if( object.name == "grass" )
            {
                mainGame.addObject( std::make_shared<Platform>(
                        re::Vector2f(object.x * SCALE_COEFF - 4.05, object.y * SCALE_COEFF - 0.05), 
                        re::Vector2f((float)object.width.value() * SCALE_COEFF + 0.2, (float)object.height.value() * SCALE_COEFF + 0.2)));

            } else if( object.name == "ground" )            {
                mainGame.addObject( std::make_shared<Platform>(
                        re::Vector2f(object.x * SCALE_COEFF - 4.05, object.y * SCALE_COEFF - 0.05), 
                        re::Vector2f((float)object.width.value() * SCALE_COEFF + 0.2, (float)object.height.value() * SCALE_COEFF + 0.2)));


            } else if ( object.name == "yojus" ) {
                player = std::make_shared<Player>(re::Vector2f(object.x * SCALE_COEFF, object.y * SCALE_COEFF));
                player->movingAnim = resource_manager.get_animation("playerAnim");
                player->slidingAnim = resource_manager.get_animation("onIceAnim");
                player->jumpingAnim = resource_manager.get_animation("jumpAnim");
                player->attackAnim = resource_manager.get_animation("atkAnim");
                player->setFriction(-1.0);
                player->setBounciness(0.0);

            } else if ( object.name == "ice" ) {
                re::GameObjectPtr platice = std::make_shared<IcePlatform>(
                        re::Vector2f(object.x * SCALE_COEFF - 4, object.y * SCALE_COEFF), 
                        re::Vector2f((float)object.width.value() * SCALE_COEFF + 0.1, (float)object.height.value() * SCALE_COEFF + 0.1));
                mainGame.addObject(platice);
            } else if ( object.name == "corr" ) {
                for(size_t i = 0; i < object.width.value() / 64; i++ )
                {
                    re::GameObjectPtr weplat = std::make_shared<WeakPlatform>( 
                        re::Vector2f(object.x * SCALE_COEFF + ( (i) * 4 ) - 4, object.y * SCALE_COEFF), 
                        re::Vector2f(4, 4), 0.75);
                    mainGame.addObject(weplat);
                }
            } else if ( object.name == "metal" ) {
                auto movplat = std::make_shared<MovingPlatform>(
                        re::Vector2f(object.x * SCALE_COEFF - 4, object.y * SCALE_COEFF + 4), 
                        re::Vector2f(4, 1), 
                        3.0);
                mainGame.addObject(movplat);
                movplat->path->addWaypoint(re::Vector2f(
                    (object.x * SCALE_COEFF) + 4 + (object.width.value() / 128) * 4,// - 1) * 4,
                    (object.y * SCALE_COEFF) + 4// + ((object.height / 128) - 1) * 4
                ));
                movplat->path->setCycled(true);
                movplat->path->setActivated(true);
            } else if ( object.name == "deth" ) {
                re::GameObjectPtr deathTrig = std::make_shared<DeathTrigger>(
                                re::Vector2f(object.x * SCALE_COEFF - 3.6, object.y * SCALE_COEFF + (float)object.height.value() * SCALE_COEFF * 0.1), 
                                re::Vector2f((float)object.width.value() * SCALE_COEFF - 0.8, (float)object.height.value() * SCALE_COEFF * 0.8));
                mainGame.addObject(deathTrig);
            } else if (object.name == "EvilBird"){
                auto evilbird = std::make_shared<EvilBird>(re::Vector2f(object.x * 64 * SCALE_COEFF,
                object.y * 64 * SCALE_COEFF + 1), 5.0);
                mainGame.addObject(evilbird);
                evilbird->path->addWaypoint(re::Vector2f(
                    evilbird->getPosition().X + object.width.value() * 4,
                    evilbird->getPosition().Y + object.height.value() * 4
                ));
                evilbird->path->setActivated(true);
                evilbird->movingAnim = resource_manager.get_animation("birdMovingAnim");
            }
        }

        mainGame.addObject(player);
        player->addAbility(new Ability_DamageBoost(15, 3));
        player->addAbility(new Ability_Heal(10));
        player->addAbility(new Ability_Invincibility(5));

        curHUD = new HUD(std::dynamic_pointer_cast<Player>(player).get(), &resource_manager);

        mainGame.addObject(std::make_shared<Platform>(re::Vector2f(-7, -4), re::Vector2f(4, 9 * 4 * 3 + 8)));
        mainGame.addObject(std::make_shared<Platform>(re::Vector2f(-4, -7), re::Vector2f(16 * 4 * 3 + 8, 4)));
        mainGame.addObject(std::make_shared<Platform>(re::Vector2f(16 * 4 * 3 - 5, -4), re::Vector2f(4, 9 * 4 * 3 + 8)));
        mainGame.addObject(std::make_shared<DeathTrigger>(re::Vector2f(-4, 9 * 4 * 3 - 1), re::Vector2f(16 * 4 * 3 + 8, 4)));
    }

    void setup() override {
        resource_manager.load_file("resources.xml");

                                       
        pause_image = resource_manager.get_image("whilepaused");
        main_menu_image = resource_manager.get_image("mainmenu");

        
        map_objects_init();

        re::background( re::Color(200,200,255) );

        re::ImagePtr bs = resource_manager.get_image("buttonsource");
 
        re::ImagePtr im_small_button = bs->get_subimage(0, 225, 50, 50);
        re::ImagePtr im_small_button_hover = bs->get_subimage(50, 225, 50, 50);
        re::XmlElemPtr gui = re::parse_xml("menu.xml")->get_children("gui")[0];

        re::BaseButton button1(341, 125, 289, 65, "new_game");
        re::BaseButton button2(341, 387, 292, 65, "load_game");
        re::BaseButton button3(341, 500, 291, 65, "settings");
        re::BaseButton button4(960, 517, 64, 59, "quit");
        gui_manager.register_button(std::make_shared<re::BaseButton>(button1), "main_menu");
        gui_manager.register_button(std::make_shared<re::BaseButton>(button2), "main_menu");
        gui_manager.register_button(std::make_shared<re::BaseButton>(button3), "main_menu");
        gui_manager.register_button(std::make_shared<re::BaseButton>(button4), "main_menu");

        for (auto small_button : gui->get_children("small_button")) {
            int x = std::stoi(small_button->field["x"]);
            int y = std::stoi(small_button->field["y"]);
            std::string id = small_button->field["id"];
            re::BaseButton button(x, y, id, im_small_button, im_small_button_hover);
            // button.register_action(std::bind(&MainApp::set_changing_control, this, id));
            gui_manager.register_button(std::make_shared<re::BaseButton>(button), "controls");
        }

        gui_manager.get_button_by_id("new_game")->register_action(std::bind(&MainApp::setState_ingame, this));
        gui_manager.get_button_by_id("settings")->register_action(std::bind(&MainApp::toggle_state_controls, this));
        gui_manager.get_button_by_id("quit")->register_action(std::bind(&MainApp::setState_exit, this));

        gui_manager.layer_set_active("controls", false);

        curState = AppState::MainMenu;
    }

    void update() override {
        
        switch(curState){
            case AppState::Ingame:{
                mainGame.updateTick();
                break;
            }
            case AppState::Exit:{
                exit(0);
            }
        }  
    }

    void display() override 
    {   
        re::Vector2f campos;
        switch(curState){
        case AppState::Ingame:
            //re::view_at( (int)((player->getPosition().X * 16 + 64 * 16) / (64 * 16) ) * 64 * 16 - 64 * 17,//(player->getPosition().X * 16 / 16) * 1000,
            //             (int)((player->getPosition().Y * 16) / (64 * 9) ) * 64 * 9 );//(player->getPosition().Y * 16 / 9 ) * 1000);

            static re::Vector2f location = re::Vector2f( 1,0 );

            int cam_x, cam_y;
            cam_x = player->getPosition().X * 16;
            cam_y = player->getPosition().Y * 16;
            cam_x+= 64 * 17;
            cam_x/= 64 * 16;

            if( cam_x > location.X)
            {
                mainGame.addObject( std::make_shared<Platform>(
                        re::Vector2f(((location.X-1) * 4 * 16) - 0.5, location.Y * 4 * 9 - 0.5), 
                        re::Vector2f(4 * 15 + 1, 4 * 9 + 1) ) );
                mainGame.addObject( std::make_shared<DeathTrigger>(
                        re::Vector2f(((location.X-1) * 4 * 16) - 0.5 + 4, location.Y * 4 * 9 - 0.5 + 4), 
                        re::Vector2f(4 * 15 + 1 - 8, 4 * 9 + 1 - 8) ) );
                player->reduceCooldowns();
            }

            if( cam_x < location.X)
            {
                mainGame.addObject( std::make_shared<Platform>(
                        re::Vector2f(((location.X+1) * 4 * 16) - 0.5, location.Y * 4 * 9 - 0.5), 
                        re::Vector2f(4 * 15 + 1, 4 * 9 + 1) ) );
                mainGame.addObject( std::make_shared<DeathTrigger>(
                        re::Vector2f(((location.X+1) * 4 * 16) - 0.5 + 4, location.Y * 4 * 9 - 0.5 + 4), 
                        re::Vector2f(4 * 15 + 1 - 8, 4 * 9 + 1 - 8) ) );
                player->reduceCooldowns();
            }


            location.X = cam_x;
            cam_x*= 64 * 16;
            cam_x-= 64 * 17;

            cam_y/= 64 * 9;

            if( cam_y != location.Y )
            {
                mainGame.addObject( std::make_shared<Platform>(
                        re::Vector2f(location.X * 64 * 16, (location.Y - 1) * 64 * 9 ), 
                        re::Vector2f(4 * 15, 4 * 9) ) );
                mainGame.addObject( std::make_shared<DeathTrigger>(
                        re::Vector2f(location.X * 4 * 16 - 0.5 + 4, (location.Y - 1) * 4 * 9 - 0.5 + 4), 
                        re::Vector2f(4 * 15 + 1 - 8, 4 * 9 + 1 - 8) ) );
                player->reduceCooldowns();
            }
            location.Y = cam_y;
            
            cam_y*= 64 * 9;
            //don't show it to Kolya, srsly.
            //Kolya, if you saw it, pls, don't ask us to do pushups.

            re::view_at( cam_x, cam_y );
            //re::view_at(  );

            re::draw_image_part( re::get_cam_x(), re::get_cam_y(),
                                 re::get_cam_x() + 1024, re::get_cam_y() + 576,
                                 0,0, 1,1,
                                 resource_manager.get_image("bg") );

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
            //re::background(re::WHITE); 
            /*for (auto& btn : buttonList){
                btn.display();
            }*/
            re::draw_image(0, 0, main_menu_image);
            for(int i = 0; i < 2; i++){
                // buttonList[i].display();
            }
            break;
        case AppState::Dead:
            //re::background(re::WHITE);
            // buttonList[2].display();
            break;
        case AppState::Pause:
            re::draw_image(re::get_cam_x(), re::get_cam_y(), pause_image);
            break;
        }

        gui_manager.display(curX, curY);
        re::draw_text(50, 70, std::to_string(re::get_fps()), re::RED);
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
         */
        gui_manager.on_click(button, curX, curY);
    }

    void on_mouse_move(int mouseX, int mouseY){
        curX = mouseX;
        curY = mouseY;
    }

    //Callbacks:
    void setState_ingame(){
        curState = AppState::Ingame;
        gui_manager.layer_set_active("main_menu", false);
        gui_manager.layer_set_active("controls", false);
        std::cout << "main app state change" << std::endl;
    }

    void toggle_state_controls() {
        if (gui_manager.get_button_by_id("right")->is_active()) {
            gui_manager.layer_set_active("main_menu", true);
            gui_manager.layer_set_active("controls", false);
            curState = AppState::MainMenu;
        } else {
            gui_manager.get_button_by_id("new_game")->set_active(false);
            gui_manager.get_button_by_id("load_game")->set_active(false);
            gui_manager.layer_set_active("controls", true);
            // curState = AppState::Controls;
        }
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
        //player->respawn();
        curState = AppState::Ingame;
    }

private:
    int x, y;
    int curX, curY;
    bool isMovingForward = true;
    std::shared_ptr<Player> player;
    AppState curState;
    re::ImagePtr pause_image, main_menu_image;
    re::ResourceManager resource_manager;
    re::GuiManager gui_manager;
};

int main(){
    re::runApp( 1024, 576, std::make_shared<MainApp>() );
    return 0;
}