#include "windowGLUT.h"
#include "RealEngine/keyboard.h"
#include "RealEngine/base_app.h"
#include "../main_app.h"


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

void glutCallbackKeyDown( unsigned char c, int a, int b )
{
    std::cout << c << '\n';
    globalWindow->appPtr->on_key_pressed( (re::Key)c );
}

void glutCallbackKeyUp( unsigned char c, int a, int b )
{
    globalWindow->appPtr->on_key_released( (re::Key)c );
}

void glutCallbackMouseMove(int button,int state,int x, int y)
{
    GLint viewport[4];
    GLdouble mvmatrix[16], projmatrix[16];

    GLdouble wx,wy,wz;

    glGetIntegerv(GL_VIEWPORT,viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
    glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);

            //viewport[3] - высоте окна в пикселях
    //realy=viewport[3]-(GLint)y-1;
    printf("Координаты в позиции курсора (%4d,%4d)\n",x,0);
    //gluUnProject((GLdouble)x,(GLdouble)0,0.0,mvmatrix,projmatrix,viewport,&wx,&wy,&wz);
}

void glutCallbackIDLE()
{
    static bool init = false;
    if(!init)
        base_app_init(globalWindow->BaseApp, globalWindow );
    base_app_tick( globalWindow->BaseApp );
}

namespace re
{
    WindowGLUT::WindowGLUT( uint w, uint h, re::IBaseApp* baseApp )
    {
        this->BaseApp = baseApp;

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
        glEnable( GL_BLEND );
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glShadeModel(GL_FLAT);

        glViewport(0,0,(GLsizei)w, (GLsizei)h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0,w,-(int)h,0,-1,1);

        glPixelStorei(GL_UNPACK_ALIGNMENT,1);

        glEnable(GL_DEPTH_TEST);

        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

        glutIdleFunc( glutCallbackIDLE );

        glutMainLoop();
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
    {
        glColor4f( color.r / 255.0, color.g / 255.0, color.b / 255.0, color.t / 255.0 );

        glLineWidth(5.0);
        glBegin(GL_LINES);
            glVertex2i(x0,-y0);
            glVertex2i(x1,-y1);
        glEnd();
    }
        
    void WindowGLUT::draw_circle(int x0, int y0, int r, Color color)
    {
        y0 *= -1;
        glColor3f( color.r / 255.0, color.g / 255.0, color.b / 255.0 );
        glRecti(x0,y0,x0+r,y0+r);
    }
    
    void WindowGLUT::draw_image(int x0, int y0, ImagePtr im)
    {
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
        glBindTexture(GL_TEXTURE_2D,im->preapreForGL());

        glBegin(GL_QUADS);
            glTexCoord2f(0.0,0.0); glVertex3f(x0,-y0,0.0);
            glTexCoord2f(0.0,1.0); glVertex3f(x0,-y0-im->h,0.0);
            glTexCoord2f(1.0,1.0); glVertex3f(x0+im->w,-y0-im->h,0.0);
            glTexCoord2f(1.0,0.0); glVertex3f(x0+im->w,-y0,0.0);
        glEnd();

        glDisable(GL_TEXTURE_2D);
        /*glPushMatrix();

        glEnable(GL_TEXTURE_2D);

        GLuint tex = im->preapreForGL();

        //glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
        glBindTexture(GL_TEXTURE_2D,tex);

        glBegin(GL_QUADS);
            glTexCoord2i(0,0); glVertex2i(0,0);
            glTexCoord2i(0,1); glVertex2i(0,100);
            glTexCoord2i(1,1); glVertex2i(100,100);
            glTexCoord2i(1,0); glVertex2i(100,0);
        glEnd();
        
        glDisable(GL_TEXTURE_2D);

        glPopMatrix();*/
    }
        
    void WindowGLUT::translate(int x, int y) 
    {
        glTranslatef( x, y, 0 );
    }

    void WindowGLUT::viewat(int x, int y) 
    {
        glViewport(-x,y, w, h);
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
    {
        appPtr = e;

    }

    uint WindowGLUT::get_width()
    {
        return this->w;
    }

    uint WindowGLUT::get_height()
    {
        return this->h;
    }

}