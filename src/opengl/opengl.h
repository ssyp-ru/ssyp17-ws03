#pragma once

#include <stdint.h>

#include "RealEngine/graphic/image.h"
#include "RealEngine/keyboard.h"
#include "RealEngine/baseApp.h"

#include <string>

namespace re
{

class OpenGL
{
    public:

    static void init( uint width, uint height, IBaseAppPtr BaseApp );
    static void run();

    static void background( Color color );

    static void draw();

    static void draw_rectangle( int x, int y, int w, int h, Color color );
    static void draw_line( int x0, int y0, int x1, int y1, Color color );
    //NOT WORK
    static void draw_circle( int x0, int y0, int t, Color color );
    static void draw_image( int x0, int y0, ImagePtr img );
    
    static void translate( int x, int y );
    static void viewAt( int x, int y );
    static void scale( float x, float y );
    static void rotate( float angle );

    static void setWindowName( std::string name );

    static void (*callbackPostInit)();
    static void (*callbackIdle)();

    static uint getWidth();
    static uint getHeight();

    private:

    static void keyboardDown(unsigned char c, int a, int b);
    static void keyboardUp(unsigned char c, int a, int b);

    static void mouseMove(int button,int state,int x, int y);

    static IBaseAppPtr baseApp;
    static std::string windowName;
    static uint w, h;
};

}