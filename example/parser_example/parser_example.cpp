#include <RealEngine/xml_parser.h>
#include <RealEngine/tiled_parser.h>
#include <RealEngine/logger.h>
#include <RealEngine/baseApp.h>
#include <RealEngine/graphic.h>
#include <iostream>

class MainApp : public re::IBaseApp {
public:
    int x;
    int y;
    re::Map map;
    re::ImagePtr bg;
    std::vector<re::ImagePtr> emptyBG;
    uint i, t = 0;
    void setup() override {
        i = 1; t = 0;
        x = 0;
        y = 0;
        re::Log::log_level(re::Log::LEVEL::DEBUG);
        re::set_fps(60);
        for(int j = 1; j <= 32; j*=2) {
            re::ImagePtr img = std::make_shared<re::Image>(1920,1080, 4);
            for(uint y = 0; y < 1080; y++)
                for(uint x = 0; x < 1920; x++) {
                    if(((x/j)%2 == 0) ^ ((y/j)%2 == 0)) {
                        img->set_pix_color(x,y,re::GREEN);
                    } else {
                        img->set_pix_color(x,y,re::BLACK);
                    }
                }
            img->reGenTexture();
            emptyBG.push_back(img);
        }
        map = re::parse_tiled(re::parse_xml("test.tmx"))[0];
    }

    void update() override {}

    void display() override {
        re::background(re::BLACK);
        re::draw_image_part( 0,0,1920,1080,0,0,1,1, emptyBG[t++/30]);
        while( t >= 30*5) {
            t = 0;
        }
        re::draw_image( 0, 0, map.layer[0].background );
    }

    void on_key_pressed(re::Key key){
        if (key == re::Key::Escape){
            re::exitApp();
        } else if(key == re::Key::A) {
//            re::goFullScreen();
        }
        std::cout << (int)key << " pressed!" << std::endl;
    }

    void on_mouse_move( int x0, int y0 )
    {
        x = x0 - 25;
        y = y0 - 25;
    }
};

int main(){
    re::setWindowName( "RealEngine" );
    re::runApp( 1920, 1080, std::make_shared<MainApp>() );
}