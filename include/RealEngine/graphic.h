#pragma once

#include "graphic/image.h"
#include "graphic/color.h"
#include "math.h"
#include "keyboard.h"

namespace re{

    void background(Color color);
    void draw_rectangle(int x0, int y0, int w, int h, Color color);
    void draw_line(int x0, int y0, int x1, int y1, Color color);
    void draw_circle(int x0, int y0, int r, Color color);
    void draw_image(int x0, int y0, ImagePtr im);
    void draw_image_part(int x0, int y0, int x1, int y1, float w0, float h0, float w1, float h1, ImagePtr im);
    void set_fps(uint fps);
    void translate(int x, int y);
    void scale(float x, float y);
    void viewat(int x, int y); 
    void rotate(float angle);
    void goFullScreen();
    void setDrawLay( int lay );

    void draw_text( int x0, int y0, std::string text, Color color );

    bool getKeyState( Key key );

    struct stParticle
    {
        float life;
        Point2f pos;
        Point2f vec;
        Point2f move;

        Point2f size;

        ImagePtr imge;

        Point2f startImg;
        Point2f endImg;
    };

    void launchParticle( stParticle &newParticle );
    void explodeImage( Point2f pos, Point2f size, Point2f part, ImagePtr imge, Point2f center, Point2f impulse, float power, float random );

}