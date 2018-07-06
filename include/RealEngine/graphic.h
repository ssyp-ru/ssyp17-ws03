#pragma once

#include "graphic/image.h"
#include "graphic/color.h"
#include "math.h"
#include "keyboard.h"

namespace re{

    enum font
    {
        8_BY_13,
        9_BY_15,
        HELVETICA_10,
        HELVETICA_12,
        HELVETICA_18,
        TIMES_ROMAN_10,
        TIMES_ROMAN_24
    };
    //set background color, pls don't run evry tick
    void background(Color color);
    //draw rectangle, too smart
    void draw_rectangle(int x0, int y0, int w, int h, Color color);
    void draw_rectangle(Point2f pos, Point2f size, Color color);    
    //draw line, again super comment
    void draw_line(int x0, int y0, int x1, int y1, Color color);
    void draw_line(Point2f pos0, Point2f pos1, Color color);
    //simple draw image
    void draw_image(int x0, int y0, ImagePtr im);
    void draw_image(Point2f pos, ImagePtr im);
    //not simple draw image, (x0,y0)(x1,y1) position of rectangle, (w0,h0)(w1,h1) part of image
    void draw_image_part(int x0, int y0, int x1, int y1, float w0, float h0, float w1, float h1, ImagePtr im);
    void draw_image_part(Point2f pos, Point2f size, Point2f start, Point2f end, ImagePtr im);
    //set maximum fps
    void set_fps(uint fps);
    //change draw pos, strange foo
    void translate(int x, int y);
    void translate(Point2f pos);
    //ZOOMING
    void scale(float co);
    //change camera pos
    void view_at(int x, int y); 
    void view_at(Point2f pos); 
    //rotate screen
    void rotate(float angle);
    //window -> fullscrenn
    void go_full_screen();
    //change lay to draw(global)
    void set_draw_lay( int lay );

    //draw text, roo smart comment
    void draw_text( int x0, int y0, std::string text, Color color );
    void draw_text( Point2f pos, std::string text, Color color );
    void draw_text_custom( int x, int y, int font_id, std::string text, Color color );
    //return state( prese or not ) of key
    bool get_key_state( Key key );

    void set_fps(uint new_fps);
    uint get_fps();

    uint get_width();
    uint get_height();

    int get_cam_x();
    int get_cam_y();

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

    //launch one prticle, range of work 1 - 1000
    void launch_particle( stParticle &newParticle );
    //convert image to paticles, range of work 1000 - 100000
    void explode_image( Point2f pos, Point2f size, Point2f part, ImagePtr imge, Point2f center, Point2f impulse, float power, float random );

}