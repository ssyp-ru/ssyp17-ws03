#include "opengl.h"

#include "RealEngine/keyboard.h"
#include "RealEngine/baseApp.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glx.h>

#include <string>
#include <iostream>
#include <stdlib.h>

#include <math.h>

namespace re
{


void accFrustum(GLdouble left,GLdouble right,
                GLdouble bottom,GLdouble top,
                GLdouble zNear,GLdouble zFar,
                GLdouble pixdx,GLdouble pixdy,
                GLdouble eyedx, GLdouble eyedy,
                GLdouble focus)

{
      GLdouble xwsize, ywsize;
      GLdouble dx, dy;
      GLint viewport[4];

      glGetIntegerv(GL_VIEWPORT,viewport);

      xwsize=right-left;
      ywsize=top-bottom;
      dx=-(pixdx*xwsize/(GLdouble)viewport[2] + eyedx*zNear/focus);
      dy=-(pixdy*ywsize/(GLdouble)viewport[3] + eyedy*zNear/focus);

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glFrustum(left+dx,right+dx,bottom+dy,top+dy,zNear,zFar);
      glMatrixMode(GL_MODELVIEW);
      glTranslatef(-eyedx,-eyedy,0.0);
}

void OpenGL::init( uint width, uint height, IBaseAppPtr BaseApp )
{
    w = width;
    h = height;
    
    for( int i = 0; i < 256; i++ ) key[i] = false;

    baseApp = BaseApp;

    int argumentCount = 0;
    glutInit( &argumentCount, nullptr );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ACCUM );

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
    glOrtho( 0, w, -(int)h, 0, -100, 100 );
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

    glutIdleFunc( OpenGL::callbackIdle );
    glutKeyboardFunc( OpenGL::keyboardDown );
    glutKeyboardUpFunc( OpenGL::keyboardUp );
    glutMouseFunc( OpenGL::mousePress );
    glutMotionFunc( OpenGL::mouseMove );

    glutSpecialFunc( OpenGL::keyboardSpecial );
    glutSpecialUpFunc( OpenGL::keyboardUpSpecial );

    glClearColor( 0.5, 0.5, 0.5, 1.0 );
    glClearAccum( 0, 0, 0, 0 );
    
    curLay = 0;
    
    callbackPostInit();
}

void OpenGL::run()
{
    glutMainLoop();
}

void OpenGL::goFullScreen()
{
    glutFullScreen();
}

void OpenGL::setWindowName( std::string name )
{
    windowName = name;
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
    glRecti( x0, y0, x0 + w, y0 + h );
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

void OpenGL::setLay( int lay )
{
    curLay = lay;
}

void OpenGL::draw_blew( int x0, int y0, int we, int he, int power )
{
    unsigned char *imge = (unsigned char*)malloc( we * he * 4 );//[100][100][4];
    unsigned char *imgeBlew = (unsigned char*)malloc( we * he * 4 );
    Image img( we, he, 4 );
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);

    glBindTexture(GL_TEXTURE_2D,img.getTex());
    glCopyTexSubImage2D(GL_TEXTURE_2D,0, 0,0, 0,480-he, we,he );
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA , GL_UNSIGNED_BYTE, imge);

    for( int any_name = 0; any_name < 0; any_name++ ){;}

    for( int any_name = 0; any_name < power; any_name++ )
    {
        for( int i = 0; i < we*he*4; i++ )
        {
            imgeBlew[i] = 255;
        }

        for( int i = 1; i < he-1; i++ )
        {
            for( int j = 1; j < we-1; j++ )
            {
                for( int k = 0; k < 3; k++ )
                {
                    imgeBlew[(i * we * 4) + (j * 4) + k] = 0;

                    imgeBlew[ (i*we*4) + (j*4) + k ] += imge[ ((i-1)*we*4) + (j*4) + k ] / 5;
                    imgeBlew[ (i*we*4) + (j*4) + k ] += imge[ ((i+1)*we*4) + (j*4) + k ] / 5;

                    imgeBlew[ (i*we*4) + (j*4) + k ] += imge[ (i*we*4) + ((j-1)*4) + k ] / 5;
                    imgeBlew[ (i*we*4) + (j*4) + k ] += imge[ (i*we*4) + ((j+1)*4) + k ] / 5;

                    imgeBlew[ (i*we*4) + (j*4) + k ] += imge[ (i*we*4) + (j*4) + k ] / 5;
                }
            }
        }

        for( int i = 0; i < we*he*4; i++ )
        {
            imge[i] = imgeBlew[i];
        }
    }
    
    free(imge);
    Image aimg( (void*)imgeBlew, we, he, 4 );

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D,aimg.getTex());

    glBegin(GL_QUADS);
        glTexCoord2f(0.0,1.0); glVertex3f(x0,-y0,0.1);
        glTexCoord2f(0.0,0.0); glVertex3f(x0,-y0-aimg.h,0.1);
        glTexCoord2f(1.0,0.0); glVertex3f(x0+aimg.w,-y0-aimg.h,0.1);
        glTexCoord2f(1.0,1.0); glVertex3f(x0+aimg.w,-y0,0.1);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void drawTexRect( int x0, int y0, int x1, int y1 )
{
    glBegin(GL_QUADS);
        glTexCoord2f(0.0,1.0); glVertex3f(x0,y0,0.1);
        glTexCoord2f(0.0,0.0); glVertex3f(x0,y1,0.1);
        glTexCoord2f(1.0,0.0); glVertex3f(x1,y1,0.1);
        glTexCoord2f(1.0,1.0); glVertex3f(x1,y0,0.1);
    glEnd();
}

void OpenGL::blewAll()
{
    unsigned char *imge = (unsigned char*)malloc( w * h * 4 );
    Image img( w, h, 4 );
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);

    glBindTexture(GL_TEXTURE_2D,img.getTex());
    glCopyTexSubImage2D(GL_TEXTURE_2D,0, 0,0, 0,0, w,h );
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA , GL_UNSIGNED_BYTE, imge);
    glBindTexture(GL_TEXTURE_2D,img.getTex());

    glClear( GL_ACCUM_BUFFER_BIT );
    glAccum( GL_ACCUM, 0.2 );

    glClear( GL_COLOR_BUFFER_BIT );
    drawTexRect( 1,0,w+1,-h );
    glAccum( GL_ACCUM, 0.2 );

    glClear( GL_COLOR_BUFFER_BIT );
    drawTexRect( -1,0,w-1,-h );
    glAccum( GL_ACCUM, 0.2 );

    glClear( GL_COLOR_BUFFER_BIT );
    drawTexRect( 0,1,w,-h+1 );
    glAccum( GL_ACCUM, 0.2 );

    glClear( GL_COLOR_BUFFER_BIT );
    drawTexRect( 0,-1,w,-h-1 );
    glAccum( GL_ACCUM, 0.2 );

    glClear( GL_COLOR_BUFFER_BIT );

    glAccum( GL_RETURN, 1 );

    //std::cout << h << '\n';

    glDisable(GL_TEXTURE_2D);
    free( imge );
}

void OpenGL::draw_image( int x0, int y0, ImagePtr img )
{
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D,img->getTex());

    glBegin(GL_QUADS);
        glTexCoord2f(0.0,0.0); glVertex3f( x0, -y0, curLay );
        glTexCoord2f(0.0,1.0); glVertex3f( x0, -y0 - img->h, curLay );
        glTexCoord2f(1.0,1.0); glVertex3f( x0 + img->w, -y0 - img->h, curLay );
        glTexCoord2f(1.0,0.0); glVertex3f( x0 + img->w, -y0, curLay );
    glEnd();

    glDisable(GL_TEXTURE_2D);

    blewAll();
    //draw_blew( 0,0, 640,480, 1 );
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
    if( !OpenGL::instance().key[c] )
    {
        OpenGL::instance().key[c] = true;
        OpenGL::instance().baseApp->on_key_pressed( (Key)c );
    }
}

void OpenGL::keyboardUp( unsigned char c, int a, int b )
{
    if( OpenGL::instance().key[c] )
    {
        OpenGL::instance().key[c] = false;
        OpenGL::instance().baseApp->on_key_released( (Key)c );
    }
}

bool OpenGL::getKeyState( unsigned char c )
{
    return key[c];
}

void OpenGL::mouseMove( int x, int y )
{
    OpenGL::mousePress( 0, 0, x, y );
}

void OpenGL::keyboardSpecial( int c, int a, int b )
{
    if( !OpenGL::instance().key[c] )
    {
        OpenGL::instance().key[c] = true;
        OpenGL::instance().baseApp->on_key_pressed( (Key)c );
    }
}

void OpenGL::keyboardUpSpecial( int c, int a, int b )
{
    if( OpenGL::instance().key[c] )
    {
        OpenGL::instance().key[c] = false;
        OpenGL::instance().baseApp->on_key_released( (Key)c );
    }
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

    OpenGL::instance().baseApp->on_mouse_move( wx, -wy );
    if(state){ OpenGL::instance().baseApp->on_button_pressed( button ); }
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