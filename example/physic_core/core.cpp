#include <RealEngine/baseApp.h>
#include <RealEngine/graphic.h>
#include "RealEngine/physic_core.h"

#include <iostream>
#include <vector>
#include <memory>

///////////////////////////////////////////////////////////////////////////////
re::Game mainGame;

class MainApp : public re::IBaseApp{
public:
	re::Game mainGame;
    re::ImagePtr imgptr;

    void setup() override 
    {
		re::GameObjectPtr obj1 = std::make_shared<re::GameObject>(re::Vector2f(2, 4));
    	obj1->setMass(10);
    	obj1->setFriction(0.0);
    	obj1->setBounciness(0.5);
    	obj1->addImpulse(re::Vector2f(10, 0));
		obj1->addPoint(re::Vector2f(1, 1));
		obj1->addPoint(re::Vector2f(1, -1));
		obj1->addPoint(re::Vector2f(-1, -1));
		obj1->addPoint(re::Vector2f(-1, 1));
		obj1->addEdge(0, 1);
		obj1->addEdge(1, 2);
		obj1->addEdge(2, 3);
		obj1->addEdge(3, 0);
		mainGame.addObject(obj1);

		re::GameObjectPtr obj2 = std::make_shared<re::GameObject>(re::Vector2f(10, 15));
		obj2->setRigidbodySimulated(false);
    	obj2->setMass(10);
    	obj2->setFriction(0.2);
    	obj2->setBounciness(0.5);
    	obj2->addImpulse(re::Vector2f(10, 0));
		obj2->addPoint(re::Vector2f(-10, 1));
		obj2->addPoint(re::Vector2f(-10, -1));
		obj2->addPoint(re::Vector2f(10, -1));
		obj2->addPoint(re::Vector2f(10, 1));
		obj2->addEdge(0, 1);
		obj2->addEdge(1, 2);
		obj2->addEdge(2, 3);
		obj2->addEdge(3, 0);
		mainGame.addObject(obj2);
    }

    void update() override {
    }

    void display() override {
		std::vector<re::GameObjectPtr> curWorld = mainGame.getWorld();
        curWorld[0]->addForce(re::Vector2f(0, 10 * curWorld[0]->getMass()));

        mainGame.updateTick();

		curWorld = mainGame.getWorld();


        re::graphic::background(re::WHITE);
        for (uint i = 0; i < curWorld.size(); i++)
        {
            for (uint j = 0; j < curWorld[i]->getEdges()->size(); j++)
            {
                re::graphic::draw_line(((*curWorld[i]->getEdges())[j].P1->X + curWorld[i]->getPosition().X) * 25, 
                                       ((*curWorld[i]->getEdges())[j].P1->Y + curWorld[i]->getPosition().Y) * 25,
                                       ((*curWorld[i]->getEdges())[j].P2->X + curWorld[i]->getPosition().X) * 25, 
                                       ((*curWorld[i]->getEdges())[j].P2->Y + curWorld[i]->getPosition().Y) * 25,
                                       re::BLACK);
            }
        }
    }

    void on_key_pressed(re::Key key){
        //std::cout << (int)key;
        if (key == re::Key::Escape){
            exit(0);
        }
    }

    void on_mouse_move( int x0, int y0 )
    {
        x = x0 - 25;
        y = y0 + 25;
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
