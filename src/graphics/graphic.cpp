#include "RealEngine/graphic.h"
#include <iostream>

#include "../window/opengl.h"

namespace re
{
    void draw_rectangle(int x0, int y0, int w, int h, Color color){
        OpenGL::instance().draw_rectangle( x0, -y0, w, -h, color );
    }

    void background(Color color){
        OpenGL::instance().background( color );
    }

    void draw_line(int x0, int y0, int x1, int y1, Color color){
        OpenGL::instance().draw_line( x0, y0, x1, y1, color );
    }

    void draw_circle(int x0, int y0, int r, Color color){
        OpenGL::instance().draw_circle( x0, y0, r, color );
    }

    void draw_image(int x0, int y0, ImagePtr im){
        OpenGL::instance().draw_image( x0, y0, im );
    }

    void draw_image_part(int x0, int y0, int x1, int y1, float w0, float h0, float w1, float h1, ImagePtr im){
        OpenGL::instance().draw_image_part( x0, y0, x1, y1, w0, h0, w1, h1, im );
    }

    void draw_text( int x0, int y0, std::string text, Color color )
    {
        OpenGL::instance().draw_text( x0, y0, text, color );
    }

    bool getKeyState( Key key )
    {
        return OpenGL::instance().getKeyState( (int)key );
    }

    void goFullScreen()
    {
        OpenGL::instance().goFullScreen();
    }

    void setDrawLay( int lay )
    {
        OpenGL::instance().setLay( lay );
    }

    void set_fps(uint fps){
        
    }

    void translate(int x, int y){
        OpenGL::instance().translate( -x, y );
    }

    void viewat(int x, int y)
    {
        OpenGL::instance().viewAt( x, y );
    }

    void scale(float x, float y){
        OpenGL::instance().scale( x, y );
    }

    void rotate(float angle){
        OpenGL::instance().rotate( angle );
    }
} // namespace re