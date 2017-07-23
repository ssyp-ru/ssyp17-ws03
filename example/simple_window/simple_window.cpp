#include <RealEngine/baseApp.h>
#include <RealEngine/graphic.h>

#include <iostream>

class MainApp : public re::IBaseApp{
public:
    re::ImagePtr imgptr;

    void setup() override {
        re::graphic::set_fps(60);
        imgptr = std::make_shared<re::Image>( "test.png" );
        x = 0;
    }

    void update() override {
        x += 10;
        if (x > 390) x = 0;
    }

    void display() override {
        re::graphic::background(re::WHITE);
        re::graphic::draw_rectangle(x, 50, 50, 50, re::RED);
        re::graphic::draw_rectangle(200, 200, 50, 50, re::RED);
        re::graphic::draw_image( 0, 0, imgptr );
        re::graphic::draw_line( x, 50, 200, 200, re::BLACK );
    }

    void on_key_pressed(re::Key key){
        if (key == re::Key::Escape){
            exit(0);
        }
    }
private:
    int x;
};

int main(){
    re::runApp( 400, 400, std::make_shared<MainApp>() );
    return 0;
}