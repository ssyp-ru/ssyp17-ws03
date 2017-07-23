#include "RealEngine/graphic.h"
#include <iostream>

#include "../opengl/opengl.h"

namespace re
{

namespace graphic
{
    void draw_rectangle(int x0, int y0, int w, int h, Color color){
        GL.draw_rectangle( x0, -y0, w, h, color );
    }

    void background(Color color){
        GL.background( color );
    }

    void draw_line(int x0, int y0, int x1, int y1, Color color){
        GL.draw_line( x0, y0, x1, y1, color );
    }

    void draw_circle(int x0, int y0, int r, Color color){
        GL.draw_circle( x0, -y0, r, color );
    }

    void draw_image(int x0, int y0, ImagePtr im){
        GL.draw_image( x0, -y0, im );
    }

    void set_fps(uint fps){
        
    }

    void translate(int x, int y){
        GL.translate( -x, y );
    }

    void viewat(int x, int y)
    {
        GL.viewAt( x, y );
    }

    void scale(float x, float y){
        GL.scale( x, y );
    }

    void rotate(float angle){
        GL.rotate( angle );
    }
}

} // namespace re