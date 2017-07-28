#include <RealEngine/xml_parser.h>
#include <RealEngine/tiled_parser.h>
#include <RealEngine/logger.h>
#include <RealEngine/baseApp.h>
#include <RealEngine/graphic.h>

class MainApp : public re::IBaseApp {
public:
    re::Map map;
    re::ImagePtr bg;
    uint i;
    void setup() override {
        re::Log::log_level(re::Log::LEVEL::DEBUG);
        re::set_fps(1);
        map = re::parse_tiled(re::parse_xml("test.tmx"))[0];
    }

    void update() override {
    }

    void display() override {
        re::draw_image( 0, 0, map.layer[0].background );
        re::background(re::BLACK);
        //re::draw_image_part( 0,0,1920,1080,0,0,1,1,map.layer[0].background);

    }

    void on_key_pressed(re::Key key){
        if (key == re::Key::Escape){
            re::exitApp();
        } else if(key == (re::Key)112) {
            //re::graphic::goFullScreen();
        }
    }
};

int main(){
    re::setWindowName( "RealEngine" );
    re::runApp( 1920, 1080, std::make_shared<MainApp>() );
}