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

#define checkImageWidth 128
#define checkImageHeight 128

GLubyte checkImage[checkImageHeight][checkImageWidth][4];

void makeCheckImage()
{
   int i,j,c;

   for (i=0;i<checkImageHeight;i++)
   {
      for (j=0;j<checkImageWidth;j++)
      {
         c=(((i&0x08)==0)^((j&0x8)==0))*255;
         checkImage[i][j][0]=(GLubyte)c;
         checkImage[i][j][1]=(GLubyte)c;
         checkImage[i][j][2]=(GLubyte)c;
         checkImage[i][j][3]=128;
      }
   }
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

        glEnable( GL_ALPHA );
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glShadeModel(GL_FLAT);

        glViewport(0,0,(GLsizei)w, (GLsizei)h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0,w,-(int)h,0,-1,1);

        glPixelStorei(GL_UNPACK_ALIGNMENT,1);
        makeCheckImage();
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
        glColor4f( color.r / 255.0, color.g / 255.0, color.b / 255.0, color.t / 255.0 );

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
        im->preapreForGL();
        glPushMatrix();
        glRasterPos2i( x0, -y0 - (im->h) );
        glDrawPixels( im->w, im->h, GL_RGBA, GL_UNSIGNED_BYTE, im->get_buffer() );
        glPopMatrix();
        
    }
        
    void WindowGLUT::translate(int x, int y) 
    {
        glOrtho(0,w + x,(-(int)h) - y,0,-1,1);
    }
    
    void WindowGLUT::scale(float x, float y) 
    {}
    
    void WindowGLUT::rotate(float angle) 
    {}

    void WindowGLUT::display()
    {
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