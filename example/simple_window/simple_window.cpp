#include <RealEngine/base_app.h>
#include <RealEngine/graphic.h>



class MainApp : public re::IBaseApp{
public:
    void setup() override {
        re::set_fps(60);
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
    re::init_graphics(re::GraphicalBackend::XCB, 400, 400);
    re::run_base_app(new MainApp());
    return 0;
}