#include <RealEngine/baseApp.h>
#include <RealEngine/graphic.h>
#include <RealEngine/xml_parser.h>
#include <RealEngine/config_manager.h>
#include <RealEngine/resource_manager.h>
#include <RealEngine/graphic/animation.h>
#include <RealEngine/graphic/image.h>

#include <iostream>
#include <string>
using namespace re;

using std::cout;
using std::endl;

class MainApp : public IBaseApp{
    public:
        ResourceManager rm;
        ImagePtr screen, mario;

        void setup() override {
            set_fps(60);

            cout << ConfigManager::get_property("field1/var1") << endl;
            cout << ConfigManager::get_property("field1/var2") << endl;
            cout << ConfigManager::get_property("field2/subfield/var1") << endl;
            
            rm.load_file("anims.xml");
            screen = rm.get_image("screenshot");
            mario = rm.get_image("mario_sprite_sheet");
        }

        void update() override {
        }

        void display() override {
            background(re::WHITE);

            draw_image(0, 0, screen);
            draw_image(0, 0, mario);
            draw_image(0, 300, rm.get_animation("mario_sprite")->getNextFrame());
        }

        void on_key_pressed(Key key) {
            if (key == Key::Escape) exit(0);
        }

        void on_mouse_move(int x0, int y0) {
        }
};

int main(){
    setWindowName("RealEngine");
    runApp(640, 480, std::make_shared<MainApp>());
    return 0;
}

