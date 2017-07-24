#include "windowGLUT.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glx.h>

const char windowName[] = "RealEngine";

#include <iostream>
#include <thread>

re::WindowGLUT *globalWindow;

void glutCallbackReshape( int w, int h )
{
    std::cout << "reshape\n";
    glViewport(0,0,(GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w/2.0,w/2.0,-h/2.0,h/2.0,-1.0,1.0);
}

namespace re
{
    WindowGLUT::WindowGLUT( uint w, uint h )
    {
        this->w = w;
        this->h = h;

        globalWindow = this;

        //OpenGL code
        int argumentCount = 0;
        glutInit( &argumentCount, nullptr );
        glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );

        glutInitWindowSize( w, h );
        glutCreateWindow( windowName );
        
        glClearColor( 1, 1, 1, 1 );

        glViewport(0,0,(GLsizei)w, (GLsizei)h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0,w,-(int)h,0,-1,1);

        glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    }

    WindowGLUT::~WindowGLUT()
    {

    }
    
    void WindowGLUT::set_fps( uint fps )
    {
        this->fps = fps;
    }

    uint WindowGLUT::get_fps()
    {
        return this->fps;
    }

    void WindowGLUT::background(Color color)
    {
        glClearColor( color.r, color.g, color.b, color.t );
    }

    void WindowGLUT::draw_rectangle(int x0, int y0, int w, int h, Color color)
    {
        y0 *= -1;
        h *= -1;
        glColor3f( color.r / 255.0, color.g / 255.0, color.b / 255.0 );

        glRecti(x0,y0,x0+w,y0+h);
    }
    
    void WindowGLUT::draw_line(int x0, int y0, int x1, int y1, Color color)
    {}
        
    void WindowGLUT::draw_circle(int x0, int y0, int r, Color color)
    {
        y0 *= -1;
        glColor3f( color.r / 255.0, color.g / 255.0, color.b / 255.0 );
        glRecti(x0,y0,x0+r,y0+r);
    }
    
    void WindowGLUT::draw_image(int x0, int y0, ImagePtr im)
    {
        glRasterPos2i( x0, -y0 - (im->h) );
        glDrawPixels( im->w, im->h, GL_RGB, GL_UNSIGNED_BYTE, im->get_buffer() );
    }
        // virtual void draw_text(int x0, int y0, const std::string& text) override {}
    void WindowGLUT::translate(int x, int y) 
    {}
    
    void WindowGLUT::scale(float x, float y) 
    {}
    
    void WindowGLUT::rotate(float angle) 
    {}

    void WindowGLUT::display()
    {
        //std::cout << "render\n";

        glutSwapBuffers();
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void WindowGLUT::run_events()
    {}

    void WindowGLUT::register_event_handler(IBaseAppPtr e)
    {}

    uint WindowGLUT::get_width()
    {
        return this->w;
    }

    uint WindowGLUT::get_height()
    {
        return this->h;
    }

}