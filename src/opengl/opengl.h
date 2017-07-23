#pragma once

#include <stdint.h>

#include "RealEngine/graphic/image.h"
#include "RealEngine/keyboard.h"
#include "RealEngine/baseApp.h"

#include <string>

#define GL OpenGL::instance()

namespace re
{

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

    void background( Color color );

    void draw();

    void draw_rectangle( int x, int y, int w, int h, Color color );
    void draw_line( int x0, int y0, int x1, int y1, Color color );
    //NOT WORK
    void draw_circle( int x0, int y0, int t, Color color );
    void draw_image( int x0, int y0, ImagePtr img );
    
    void translate( int x, int y );
    void viewAt( int x, int y );
    void scale( float x, float y );
    void rotate( float angle );

    void setWindowName( std::string name );

    void (*callbackPostInit)();
    void (*callbackIdle)();
    //std::function<void()> callbackIdle;

    uint getWidth();
    uint getHeight();

    private:

    static void keyboardDown(unsigned char c, int a, int b);
    static void keyboardUp(unsigned char c, int a, int b);

    static void mousePress(int button,int state,int x, int y);
    static void mouseMove(int x, int y);

    IBaseAppPtr baseApp;
    std::string windowName;
    uint w, h;
};

/*
class Singleton{
public:
    Singleton& instance(){
        static Singleton s;
        return s;
    }

private:
    Singleton();
    Singleton& operator=();
};
*/
}