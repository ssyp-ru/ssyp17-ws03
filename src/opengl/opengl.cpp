#include "opengl.h"

#include "RealEngine/keyboard.h"
#include "RealEngine/baseApp.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glx.h>

#include <string>
#include <iostream>

namespace re
{

void OpenGL::init( uint width, uint height, IBaseAppPtr BaseApp )
{
    w = width;
    h = height;
    
    baseApp = BaseApp;

    int argumentCount = 0;
    glutInit( &argumentCount, nullptr );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );

    glutInitWindowSize( w, h );
    glutCreateWindow( windowName.c_str() );
        
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

    glutIdleFunc( OpenGL::callbackIdle );
    glutKeyboardFunc( OpenGL::keyboardDown );
    glutKeyboardUpFunc( OpenGL::keyboardUp );
    glutMouseFunc( OpenGL::mousePress );
    glutMotionFunc( OpenGL::mouseMove );

    callbackPostInit();
}

void OpenGL::run()
{
    glutMainLoop();
}

void OpenGL::background( Color color )
{
    glClearColor(   color.r / 255.0, 
                    color.g / 255.0,
                    color.b / 255.0, 
                    color.t / 255.0 );
}

void OpenGL::draw_rectangle( int x0, int y0, int w, int h, Color color )
{
    glColor4f(  color.r / 255.0, 
                color.g / 255.0, 
                color.b / 255.0, 
                color.t / 255.0 );
    glRecti(x0,y0,x0+w,y0+h);
}

void OpenGL::draw_line( int x0, int y0, int x1, int y1, Color color )
{
    glColor4f(  color.r / 255.0, 
                color.g / 255.0, 
                color.b / 255.0, 
                color.t / 255.0 );

    glLineWidth(5.0);
    glBegin(GL_LINES);
        glVertex2i(x0,-y0);
        glVertex2i(x1,-y1);
    glEnd();
}

void OpenGL::draw_circle( int x0, int y0, int r, Color color )
{
    //TODO
}

void OpenGL::draw_image( int x0, int y0, ImagePtr img )
{
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D,img->getTex());

    glBegin(GL_QUADS);
        glTexCoord2f(0.0,0.0); glVertex3f(x0,-y0,0.0);
        glTexCoord2f(0.0,1.0); glVertex3f(x0,-y0-img->h,0.0);
        glTexCoord2f(1.0,1.0); glVertex3f(x0+img->w,-y0-img->h,0.0);
        glTexCoord2f(1.0,0.0); glVertex3f(x0+img->w,-y0,0.0);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void OpenGL::translate( int x, int y )
{
    glTranslatef( x, y, 0 );
}

void OpenGL::viewAt( int x, int y )
{
    glViewport( x, y, w, h );
}

void OpenGL::scale( float x, float y )
{
    //TODO
}

void OpenGL::rotate( float angle )
{
    //TODO
}

void OpenGL::draw()
{
    glutSwapBuffers();
    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGL::keyboardDown( unsigned char c, int a, int b )
{
    GL.baseApp->on_key_pressed( (Key)c );
}

void OpenGL::keyboardUp( unsigned char c, int a, int b )
{
    GL.baseApp->on_key_released( (Key)c );
}

void OpenGL::mouseMove( int x, int y )
{
    OpenGL::mousePress( 0, 0, x, y );
}

void OpenGL::mousePress( int button, int state, int x, int y )
{
    GLint viewport[4];
    GLdouble mvmatrix[16], projmatrix[16];

    GLint realy;
    GLdouble wx,wy,wz;

    glGetIntegerv(GL_VIEWPORT,viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
    glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);

    realy=viewport[3]-(GLint)y-1;

    gluUnProject((GLdouble)x,(GLdouble)realy,0.0,mvmatrix,projmatrix,viewport,&wx,&wy,&wz);

    GL.baseApp->on_mouse_move( wx, -wy );
    if(state){ GL.baseApp->on_button_pressed( button ); }
}

uint OpenGL::getWidth()
{
    return w;
}

uint OpenGL::getHeight()
{
    return h;
}

}