#include <RealEngine/baseApp.h>
#include <RealEngine/pixel.h>
#include <RealEngine/graphic.h>
#include <RealEngine/math.h>

class MainApp : public re::IBaseApp{
public:
    re::PixelEngine pixel_engine;
    bool static_pixel = true;
    re::Point2f cursor;
    re::Key last_key;

    void setup() override {
        re::background( re::WHITE );
        re::set_fps(30);
        re::view_at(0,0);
    }

    void update() override {
        pixel_engine.update(1'00'000);
    }

    void display() override {
        int id = pixel_engine.map_dynamic_ptr[int(cursor.y)][int(cursor.x)];
        pixel_engine.draw();
        re::draw_text(
            re::Point2f(40,40),
            std::to_string( pixel_engine.dynamic_vec[id][0] ),
            re::BLACK );
        re::draw_text(
            re::Point2f(40,60),
            std::to_string( pixel_engine.dynamic_vec[id][1] ),
            re::BLACK );
        re::draw_text(
            re::Point2f(140,40),
            std::to_string( pixel_engine.dynamic_vec_to[id][0] ),
            re::BLACK );
        re::draw_text(
            re::Point2f(140,60),
            std::to_string( pixel_engine.dynamic_vec_to[id][1] ),
            re::BLACK );
    }

    void on_key_pressed(re::Key key) override {
        last_key = key;
    }

    virtual void on_button_pressed(int button) override {
        switch(last_key)
        {
        case re::Key::A:
            pixel_engine.spam_static_pixels( cursor.y, cursor.x );
            break;
        case re::Key::S:
            pixel_engine.spam_dynamic_pixels( cursor.y, cursor.x, -1 );
            break;
        case re::Key::D:
            pixel_engine.spam_dynamic_pixels( cursor.y, cursor.x, 150 );
            break;
        case re::Key::X:
            pixel_engine.spam_powder( cursor.y, cursor.x );
            break;
        case re::Key::Z:
            for( int i = -10; i < 10; i++ )
            {
                for( int j = -10; j < 10; j++ )
                {
                    pixel_engine.add_static_pixel( i + cursor.y, j + cursor.x, rand() % 255, rand() % 255, rand() % 255 );
                }
            }
        }
    }

    void on_mouse_move( int x0, int y0 ) override {
        cursor.x = x0 / re::to_screen_coff;
        cursor.y = y0 / re::to_screen_coff;
    }
};

int main(){
    re::setWindowName( "PixelEngine" );
    re::runApp( 1000, 600, std::make_shared<MainApp>() );
    return 0;
}