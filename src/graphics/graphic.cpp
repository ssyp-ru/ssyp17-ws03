#include "RealEngine/graphic.h"
#include "../main_app.h"

namespace re{

void draw_rectangle(int x0, int y0, int w, int h, Color color){
    get_window()->draw_rectangle(x0, y0, w, h, color);
}

void background(Color color){
    get_window()->background(color);
}

void draw_line(int x0, int y0, int x1, int y1, Color color){
    get_window()->draw_line(x0, y0, x1, y1, color);
}

void draw_circle(int x0, int y0, int r, Color color){
    get_window()->draw_circle(x0, y0, r, color);
}

void draw_image(int x0, int y0, ImagePtr im){
    get_window()->draw_image(x0, y0, im);
}

void set_fps(uint fps){
    get_window()->set_fps(fps);
}

void translate(int x, int y){
    get_window()->translate(x, y);
}

void scale(float x, float y){

}

void rotate(float angle){

}


} // namespace re