#include <RealEngine/baseApp.h>
#include <RealEngine/graphic.h>
#include <RealEngine/graphic/image.h>
#include <RealEngine/physic_core.h>
#include <RealEngine/tiled_parser.h>
#include <RealEngine/xml_parser.h>
#include "drawableGameObject.h"

#include <iostream>
#include <vector>
#include <memory>

///////////////////////////////////////////////////////////////////////////////
re::Game mainGame;

class MainApp : public re::IBaseApp {
public:
	re::Game mainGame;
    re::Map map;

    void setup() override 
    {
        re::background(re::WHITE);
        //20 PIXELS - 1 METER
        re::view_at(-100,-100);
        std::cout << "begin map parse" << std::endl;
        map = (re::parse_tiled( re::parse_xml( "map.tmx" ) ))[0];
        std::cout << "end map parse" << std::endl;
        for (auto objectData : map.objectgroup[0].group)
        {
            std::shared_ptr<DrawableGameObject> dobj = std::make_shared<DrawableGameObject>(re::Vector2f(objectData.x, objectData.y));
            for (auto vertex : objectData.poly->points)
                dobj->addPoint(re::Vector2f(vertex.x, vertex.y));
            for (int i = 0; i < objectData.poly->points.size() - 1; i++)
                dobj->addEdge(i, i + 1);
            dobj->addEdge(objectData.poly->points.size() - 1, 0);
            re::GameObjectPtr obj = dobj;
            mainGame.addObject(obj);
        }
    }

    void update() override {
		
    }

    void display() override {
        //re::view_at( 0, 0 ); дёргать view_at каждый кадр плохо
        for (auto obj : mainGame.getWorld()){
            std::dynamic_pointer_cast<DrawableGameObject, re::GameObject>(obj)->display(1.0 / zoomOut);
        }
        //re::draw_image_part(0,0, 400,400, 0,0, 1,1, 
        //                    map.layer[0].background);
    }

    void on_key_pressed(re::Key key){
        if (key == re::Key::Escape){
            exit(0);
        }
        if (key == re::Key::R)
            zoomOut -= 0.25;
        if (key == re::Key::F)
            zoomOut += 0.25;
        
    }

    void on_mouse_move( int x0, int y0 )
    {
        
    }
private:
    float zoomOut = 1.0;
};

int main(){
    re::setWindowName( "RealEngine" );
    re::runApp( 800, 800, std::make_shared<MainApp>() );
    return 0;
}
