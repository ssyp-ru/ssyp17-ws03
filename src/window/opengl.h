#pragma once

#include <stdint.h>

#include "RealEngine/graphic/image.h"
#include "RealEngine/keyboard.h"
#include "RealEngine/baseApp.h"

#include <string>

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

    void background( Color color );

    void draw();

    void draw_rectangle( int x, int y, int w, int h, Color color );
    void draw_line( int x0, int y0, int x1, int y1, Color color );
    //NOT WORK
    void draw_circle( int x0, int y0, int t, Color color );
    void draw_image( int x0, int y0, ImagePtr img );
    void draw_image_part( int x0, int y0, int x1, int y1, float w0, float h0, float w1, float h1, ImagePtr img );
    
    void translate( int x, int y );
    void viewAt( int x, int y );
    void scale( float x, float y );
    void rotate( float angle );

    void setWindowName( std::string name );
    void goFullScreen();

    void draw_blew( int x0, int y0, int we, int he, int power );
    void blewAll();
    void setLay( int lay );

    bool getKeyState( unsigned char c );

    void (*callbackPostInit)();
    void (*callbackIdle)();
    //std::function<void()> callbackIdle;

    uint getWidth();
    uint getHeight();

    bool key[256];

    private:

    static void keyboardDown(unsigned char c, int a, int b);
    static void keyboardUp(unsigned char c, int a, int b);

    static void mousePress(int button,int state,int x, int y);
    static void mouseMove(int x, int y);

    static void keyboardSpecial( int c, int a, int b );
    static void keyboardUpSpecial( int c, int a, int b );

    static Key key_to_key(char key_code);

    int curLay;

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