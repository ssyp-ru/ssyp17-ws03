#include <RealEngine/base_app.h>
#include <RealEngine/graphic.h>
#include <RealEngine/graphic/image.h>
#include <RealEngine/graphic/animation.h>
#include <string>
#include <iostream>
#include <chrono>

class MainApp : public re::IBaseApp{
public:
    MainApp() : testanimCustom(0, true){}
    void setup(){
        re::set_fps(60);
        x = 0;
        y = 0;
        //customAnimSpeed = 0;
        re::Image spritelist("spritelist.png");
        for (int i = 0; i < 9; i++){
            testanimCustom.add_frame(spritelist.get_subimage(3 + 17*i, 5, 16, 25));
            //imgptr = std::make_shared<re::Image>("test.png")
            //testanimCustom.add_frame(std::make_shared<re::Image>("anim" + std::to_string(i) + ".png"));
        }
    }

    void update() override {
      //  x += 0;
        y += 5;
        x = x + customAnimSpeed*30;
        //customAnimSpeed += 0.05;
        //if (customAnimSpeed > 3) customAnimSpeed = 0;
        if (y > 600) y = 0;
        if (x > 600) x = 0;
        
    }

    void display() override {
        re::background(re::WHITE);
        /*re::draw_rectangle(x, 50, 50, 50, re::RED);
        re::draw_rectangle(200, 200, 50, 50, re::RED);
        re::draw_circle(100, 100, 50, re::GREEN);
        re::Image testpic("test.png");
        re::ImagePtr subimg = testpic.get_subimage(y, y+10, 100, 100);
        re::draw_image(100, 100, subimg);*/
        re::draw_rectangle(0, 126, 400, 2, re::BLACK);
        re::draw_image(100+x, 100, testanimCustom.getNextFrame());
        //testanimCustom.setSpeed(customAnimSpeed);
    }
    void on_key_released(re::Key key){
        std::cout << "Key release\n";
        if(key == re::Key::Right || key == re::Key::Left){
            testanimCustom.setSpeed(0);
        }
    }

    void on_key_pressed(re::Key key){
        std::cout << "Key pressed\n";
        if (key == re::Key::Escape){
            exit(0);
        }
        if (key == re::Key::Right){
            testanimCustom.setSpeed(0.8);
       //     x+= 10;
        //    if(x == 350){ 
          //      x = -100;
            //}
        }
        if (key == re::Key::Left){
            testanimCustom.setSpeed(0.8);
            x-=10;
            if(x == -100){
                x = 350;
            }
        }
    }
private:
    int x;
    int y;
    float customAnimSpeed;
    re::Animation testanimCustom;
    const int keyDelay = 5;
};

int main(){
    re::init_graphics(re::GraphicalBackend::XCB, 400, 400);
    re::run_base_app(new MainApp());
    return 0;
}