#include <RealEngine/baseApp.h>
#include <RealEngine/graphic.h>
#include <RealEngine/graphic/image.h>
#include <RealEngine/physic_core.h>
#include "basic_platform.h"

#include <iostream>
#include <vector>
#include <memory>

///////////////////////////////////////////////////////////////////////////////
re::Game mainGame;

class MainApp : public re::IBaseApp {
public:
	re::Game mainGame;
    re::ImagePtr imgptr;

    void setup() override 
    {
		
        imgptr = std::make_shared<re::Image>("map.png");
        re::background(re::WHITE);
        //20 PIXELS - 1 METER
	/*	re::GameObjectPtr platform1 = std::make_shared<re::BasicPlatform>(re::Vector2f(2, 5));
		platform1->setRigidbodySimulated(false);
		platform1->setMass(10);
		platform1->setFriction(0.1);
		platform1->setBounciness(0);
        mainGame.addObject(platform1);*/
    }

    void update() override {
		/*std::vector<re::GameObjectPtr> curWorld = mainGame.getWorld();
        curWorld[0]->addForce(re::Vector2f(0, 10 * curWorld[0]->getMass()));
        mainGame.updateTick();*/
    }

    void display() override {
        re::scale(2);
        re::view_at( x, 0 );
        re::draw_image(0,0, imgptr);
        x = (x+1) % 100;
        // for (uint i = 0; i < curWorld.size(); i++)
        // {
        //     for (uint j = 0; j < curWorld[i]->getEdges()->size(); j++)
        //     {
        //         re::draw_line(((*curWorld[i]->getEdges())[j].P1->X + curWorld[i]->getPosition().X) * 25, 
        //                 ((*curWorld[i]->getEdges())[j].P1->Y + curWorld[i]->getPosition().Y) * 25,
        //                 ((*curWorld[i]->getEdges())[j].P2->X + curWorld[i]->getPosition().X) * 25, 
        //                 ((*curWorld[i]->getEdges())[j].P2->Y + curWorld[i]->getPosition().Y) * 25,
        //                 re::BLACK);
        //     }
        // }
    }

    void on_key_pressed(re::Key key){
        //std::cout << (int)key;
        if (key == re::Key::Escape){
            exit(0);
        }
    }

    void on_mouse_move( int x0, int y0 )
    {
        
    }
private:
    int x;
    int y;
};

int main(){
    re::setWindowName( "RealEngine" );
    re::runApp( 640, 480, std::make_shared<MainApp>() );
    return 0;
}
