#include <RealEngine/baseApp.h>
#include <RealEngine/graphic.h>
#include "RealEngine/physic_core.h"

#include <iostream>
#include <vector>
#include <memory>

#include <map>

///////////////////////////////////////////////////////////////////////////////
re::Game mainGame;

class MainApp : public re::IBaseApp{
public:
	re::Game mainGame;
    re::ImagePtr imgptr;

    static void onTrigger(re::GameObjectPtr trigger, re::GameObjectPtr object)
    {
        std::cout << "YAY! trigger!\n";
    }

    static void onCollision(re::GameObjectPtr trigger, re::GameObjectPtr object, re::Vector2f vec)
    {
        std::cout << "YAY! collision!\n";
    }

    void setup() override 
    {
        auto obj = mainGame.addTriangle(re::Vector2f(10, 10), re::Vector2f(0, 1), re::Vector2f(1, -1), re::Vector2f(-1, -1));
        //obj->setRigidbodySimulated(false);
        //obj->setIsTrigger(true);
        obj->setVelocity(re::Vector2f(3, 0));
        auto obj2 = mainGame.addQuadrangle(re::Vector2f(10, 15), re::Vector2f(5, 1), re::Vector2f(5, -1), re::Vector2f(-5, -1), re::Vector2f(-5, 1));
        obj2->setRigidbodySimulated(false);
        auto obj3 = mainGame.addQuadrangle(re::Vector2f(15, 14), re::Vector2f(1, 1), re::Vector2f(1, -1), re::Vector2f(-1, -1), re::Vector2f(-1, 1));
        obj3->setRigidbodySimulated(false);
        obj3->setIsTrigger(true);
        obj3->addTriggerCallback(&MainApp::onTrigger);
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
