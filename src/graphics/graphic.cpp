#include "RealEngine/graphic.h"
#include <iostream>

#include "../window/opengl.h"

namespace re
{
    void draw_rectangle(int x0, int y0, int w, int h, Color color){
        OpenGL::instance().draw_rectangle( x0, -y0, w, -h, color );
    }

    void draw_rectangle(Point2f pos, Point2f size, Color color)
    {
        OpenGL::instance().draw_rectangle( pos.x, -pos.y, size.x, -size.y, color );
    }

    void background(Color color){
        OpenGL::instance().background( color );
    }

    void draw_line(int x0, int y0, int x1, int y1, Color color){
        OpenGL::instance().draw_line( x0, y0, x1, y1, color );
    }

    void draw_line(Point2f pos0, Point2f pos1, Color color){
        OpenGL::instance().draw_line( pos0.x, pos0.y, pos1.x, pos1.y, color );
    }

    void draw_image(int x0, int y0, ImagePtr im){
        OpenGL::instance().draw_image( x0, y0, im );
    }

    void draw_image(Point2f pos, ImagePtr im){
        OpenGL::instance().draw_image( pos.x, pos.y, im );
    }

    void draw_image_part(int x0, int y0, int x1, int y1, float w0, float h0, float w1, float h1, ImagePtr im){
        OpenGL::instance().draw_image_part( x0, y0, x1, y1, w0, h0, w1, h1, im );
    }

    void draw_image_part(Point2f pos, Point2f size, Point2f start, Point2f end, ImagePtr im){
        OpenGL::instance().draw_image_part( pos.x, pos.y, size.x, size.y, start.x, start.y, end.x, end.y, im );
    }

    void draw_text( int x0, int y0, std::string text, Color color )
    {
        OpenGL::instance().draw_text( x0, y0, text, color );
    }

    void draw_text( Point2f pos, std::string text, Color color )
    {
        OpenGL::instance().draw_text( pos.x, pos.y, text, color );
    }

    bool get_key_state( Key key )
    {
        return OpenGL::instance().getKeyState( (int)key );
    }

    void go_full_screen()
    {
        OpenGL::instance().goFullScreen();
    }

    uint get_width()
    {
        return OpenGL::instance().getWidth();
    }

    uint get_height()
    {
        return OpenGL::instance().getHeight();
    }

    int get_cam_x()
    {
        return OpenGL::instance().getCamX();
    }

    int get_cam_y()
    {
        return OpenGL::instance().getCamY();
    }

    void set_draw_lay( int lay )
    {
        OpenGL::instance().setLay( lay );
    }

    void translate(int x, int y){
        OpenGL::instance().translate( -x, y );
    }

    void translate(Point2f pos){
        OpenGL::instance().translate( -pos.x, pos.y );
    }

    void view_at(int x, int y)
    {
        OpenGL::instance().viewAt( x, y );
    }

    void view_at(Point2f pos)
    {
        OpenGL::instance().viewAt( pos.x, pos.y );
    }

    void scale(float co){
        OpenGL::instance().scale( co );
    }

    void rotate(float angle){
        OpenGL::instance().rotate( angle );
    }

    void set_fps(uint new_fps){
        OpenGL::instance().set_fps(new_fps);
    }

    uint get_fps(){
        return OpenGL::instance().get_fps();
    }
} // namespace re