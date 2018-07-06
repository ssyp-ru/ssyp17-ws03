#pragma once

#include <stdint.h>

#include "RealEngine/graphic/image.h"
#include "RealEngine/keyboard.h"
#include "RealEngine/baseApp.h"
#include "RealEngine/math.h"

#include <string>
#include <chrono>

namespace re
{

Key openGL_key_to_key(char key_code);
Key openGL_special_key_to_key(int key_code);

class OpenGL
{
private:
    OpenGL(){}

public:
    static OpenGL& instance()
    {
        static OpenGL self;
        return self;
    }

    void init( uint width, uint height, IBaseAppPtr BaseApp );
    void run();
    void close();

    void background( Color color );

    void draw();

    void draw_rectangle( int x, int y, int w, int h, Color color );
    void draw_line( int x0, int y0, int x1, int y1, Color color );
    //NOT WORK
    void draw_circle( int x0, int y0, int t, Color color );
    void draw_image( int x0, int y0, ImagePtr img );
    void draw_image_part( int x0, int y0, int x1, int y1, float w0, float h0, float w1, float h1, ImagePtr img );
    
    void draw_text( int x0, int y0, std::string text, Color color );
    void draw_text_custom( int x0, int y0, int font_id, std::string text, Color color );

    void translate( int x, int y );
    void viewAt( int x, int y );
    void scale( float co );
    void rotate( float angle );

    void setWindowName( std::string name );
    void goFullScreen();

    void draw_blew( int x0, int y0, int we, int he, int power );
    void blewAll();
    void setLay( int lay );

    Point2f trace_to_world( Point2f click );
    Point2f trace_to_screen( Point2f click );

    void set_fps(uint new_fps);
    uint get_fps();

    bool getKeyState( unsigned char c );

    void (*callbackPostInit)();
    void (*callbackIdle)();
    //std::function<void()> callbackIdle;

    uint getWidth();
    uint getHeight();

    int getCamX();
    int getCamY();

    bool key[256];

private:

    static void keyboardDown(unsigned char c, int a, int b);
    static void keyboardUp(unsigned char c, int a, int b);

    static void mousePress(int button,int state,int x, int y);
    static void mouseMove(int x, int y);

    static void keyboardSpecial( int c, int a, int b );
    static void keyboardUpSpecial( int c, int a, int b );

    static Key key_to_key(char key_code);
    static Key key_to_key_special(char key_code);

    int curLay;

    float zoom;

    IBaseAppPtr baseApp;
    std::string windowName;
    int windowId;
    uint w, h;
    uint camx, camy;
    int fps_count_max, wait_period_mic, frame_count, fps;
    std::chrono::time_point<std::chrono::steady_clock> last_frame_time, last_second;
};


} // namespace re