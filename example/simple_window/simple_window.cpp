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
        y = 0;
    }

    void update() override {
    }

    void display() override {
        re::graphic::background(re::WHITE);
        re::graphic::draw_rectangle(x, y, 50, 50, re::RED);
        re::graphic::draw_rectangle(200, 200, 50, 50, re::RED);
        re::graphic::draw_line( x, 50, 200, 200, re::BLACK );
        re::graphic::draw_image( 0, 0, imgptr );
    }

    void on_key_pressed(re::Key key){
        if (key == re::Key::Escape){
            exit(0);
        } else if(key == (re::Key)112) {
            //re::graphic::goFullScreen();
        }
        std::cout << (int)key;
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

