#pragma once

#include "graphic/image.h"
#include "graphic/color.h"

namespace re{

namespace graphic
{
    void background(Color color);
    void draw_rectangle(int x0, int y0, int w, int h, Color color);
    void draw_line(int x0, int y0, int x1, int y1, Color color);
    void draw_circle(int x0, int y0, int r, Color color);
    void draw_image(int x0, int y0, ImagePtr im);
    void set_fps(uint fps);
    void translate(int x, int y);
    void scale(float x, float y);
    void viewat(int x, int y); 
    void rotate(float angle);
}

}