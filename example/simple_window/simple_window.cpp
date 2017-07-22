#include <RealEngine/base_app.h>
#include <RealEngine/graphic.h>

#include <iostream>

class MainApp : public re::IBaseApp{
public:
    re::ImagePtr imgptr;

    void setup() override {
        re::set_fps(60);
        std::cout << "test 0\n";
        imgptr = std::make_shared<re::Image>( "test.png" );
        std::cout << "test 1\n";
        x = 0;
    }

    void update() override {
        x += 10;
        if (x > 390) x = 0;
    }

    void display() override {
        re::background(re::WHITE);
        re::draw_rectangle(x, 50, 50, 50, re::RED);
        re::draw_rectangle(200, 200, 50, 50, re::RED);
        re::draw_circle(100, 100, 50, re::GREEN);
        re::draw_image( 0, 0, imgptr );
        re::draw_line( x, 50, 200, 200, re::BLACK );
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
    re::runApp( 400, 400, new MainApp );
    return 0;
}