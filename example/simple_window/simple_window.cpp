#include <RealEngine/baseApp.h>
#include <RealEngine/graphic.h>

#include <iostream>

class MainApp : public re::IBaseApp{
public:
    re::ImagePtr imgptr;
    bool explode = false;

    void setup() override {
        re::set_fps(60);
        imgptr = std::make_shared<re::Image>( "test.png" );
        x = 0;
        y = 0;

        re::stParticle nPartic;
        nPartic.life = 100;
        nPartic.imge = imgptr;
        nPartic.pos = re::Point2f(200,200);
        nPartic.startImg = re::Point2f(0,0);
        nPartic.endImg = re::Point2f(1,1);
        nPartic.vec = re::Point2f(0.2,0.4);
        nPartic.move = re::Point2f(0,-7);
        // nPartic.size = re::Point2f( 50, 50 );

        re::launchParticle( nPartic );
        
        //re::explodeImage( 100, 100, 200, 200, 4, 4, imgptr );
        re::explodeImage(   re::Point2f( 100, 100 ), 
                            re::Point2f( 200, 200 ), 
                            re::Point2f( 4, 4 ), 
                            imgptr, 
                            re::Point2f( 0.5, -3 ),
                            re::Point2f( 3, -20 ),
                            1,
                            0.25 );
    }

    void update() override {
    }
    void display() override {
        re::background(re::WHITE);
        re::draw_rectangle(x, y, 50, 50, re::RED);
        re::draw_rectangle(200, 200, 50, 50, re::RED);
        re::draw_line( x, 50, 200, 200, re::BLACK );
        //re::draw_image( 0, 0, imgptr );
    }

    void on_key_pressed(re::Key key){
        std::cout << "Key pressed\n";
        if (key == re::Key::Escape){
            re::exitApp();
        } else if(key == (re::Key)112) {
            //re::graphic::goFullScreen();
        }
    }

    void on_mouse_move( int x0, int y0 )
    {
        x = x0 - 25;
        y = y0 - 25;
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

