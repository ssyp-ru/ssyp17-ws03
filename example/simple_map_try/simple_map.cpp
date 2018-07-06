#include <RealEngine/baseApp.h>
#include <RealEngine/graphic.h>
#include <RealEngine/graphic/image.h>
#include <RealEngine/physic_core.h>
#include <RealEngine/tiled_parser.h>
#include <RealEngine/xml_parser.h>

#include <iostream>
#include <vector>
#include <memory>

///////////////////////////////////////////////////////////////////////////////

class MainApp : public re::IBaseApp {
public:
	re::PhysicWorld mainGame;
    re::Map map;

    void setup() override 
    {
        re::background(re::WHITE);
        //20 PIXELS - 1 METER
        map = (re::parse_tiled( re::parse_xml( "untitled.tmx" ) ))[0];
    }

    void update() override {
		
    }

    void display() override {
        re::view_at( 0, 0 );
        re::draw_image_part(0,0, 400,400, 0,0, 1,1, 
                            map.layer[0].background);
    }

    void on_key_pressed(re::Key key){
        if (key == re::Key::Escape){
            exit(0);
        }
    }

    void on_mouse_move( int x0, int y0 )
    {
        
    }
private:
};

int main(){
    re::setWindowName( "RealEngine" );
    re::runApp( 640, 480, std::make_shared<MainApp>() );
    return 0;
}
