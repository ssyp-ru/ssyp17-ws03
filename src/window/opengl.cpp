#include "opengl.h"

#include "RealEngine/keyboard.h"
#include "RealEngine/baseApp.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glx.h>

#include <string>
#include <string.h>
#include <iostream>
#include <stdlib.h>

#include <math.h>

namespace re
{

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
    windowId = glutCreateWindow( windowName.c_str() );
        
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

void OpenGL::close()
{
    glutDestroyWindow( windowId );
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

    glLineWidth(1.0);
    glBegin(GL_LINES);
        glVertex2i(x0,-y0);
        glVertex2i(x1,-y1);
    glEnd();
}

void OpenGL::draw_circle( int x0, int y0, int r, Color color )
{
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
}

void OpenGL::draw_image_part( int x0, int y0, int x1, int y1, float w0, float h0, float w1, float h1, ImagePtr img )
{
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D,img->getTex());

    glBegin(GL_QUADS);
        glTexCoord2f(w0,h0); glVertex3f( x0, -y0, curLay );
        glTexCoord2f(w0,h1); glVertex3f( x0, -y1, curLay );
        glTexCoord2f(w1,h1); glVertex3f( x1, -y1, curLay );
        glTexCoord2f(w1,h0); glVertex3f( x1, -y0, curLay );
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void OpenGL::translate( int x, int y )
{
    glTranslatef( x, y, 0 );
}

void OpenGL::viewAt( int x, int y )
{
    glViewport( 0, 0, w, h );
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho( x, w+x, (-(int)h)-y, -y, -100, 100 );
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
    unsigned char key = (int)key_to_key(c);
    if( !OpenGL::instance().key[key] )
    {
        OpenGL::instance().key[key] = true;
        OpenGL::instance().baseApp->on_key_pressed( (Key)key );
    }
}

void OpenGL::keyboardUp( unsigned char c, int a, int b )
{
    unsigned char key = (int)key_to_key(c);
    if( OpenGL::instance().key[key] )
    {
        OpenGL::instance().key[key] = false;
        OpenGL::instance().baseApp->on_key_released( (Key)key );
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
    unsigned char key = (int)key_to_key_special(c);
    if( !OpenGL::instance().key[key] )
    {
        OpenGL::instance().key[key] = true;
        OpenGL::instance().baseApp->on_key_pressed( (Key)key );
    }
}

void OpenGL::keyboardUpSpecial( int c, int a, int b )
{
    unsigned char key = (int)key_to_key_special(c);
    if( OpenGL::instance().key[key] )
    {
        OpenGL::instance().key[key] = false;
        OpenGL::instance().baseApp->on_key_released( (Key)key );
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


Key OpenGL::key_to_key(char key_code){

    if( key_code >= 'a' && key_code <= 'z' )
    {
        return (Key)((int)Key::A + (key_code - 'a'));
    } else if( key_code >= 'A' && key_code <= 'Z' ) {
        return (Key)((int)Key::A + (key_code - 'A'));
    } else if( key_code >= '0' && key_code <= '9' ){
        return (Key)((int)Key::Num0 + (key_code - '0'));
    }

    return Key::Unknown;
}

Key OpenGL::key_to_key_special(char key_code)
{
    switch(key_code){
        case 27:                    return Key::Escape;
        case GLUT_KEY_RIGHT:        return Key::Right;
        case GLUT_KEY_LEFT:         return Key::Left;
        case GLUT_KEY_UP:           return Key::Up;
        case GLUT_KEY_DOWN:         return Key::Down;
    }
}

void OpenGL::draw_text( int x0, int y0, std::string text, Color color )
{
    glColor4f(  color.r / 255.0, 
                color.g / 255.0, 
                color.b / 255.0, 
                color.t / 255.0 );

    void *font = GLUT_BITMAP_9_BY_15;
    char *s = (char *)text.c_str();

    glRasterPos2f( x0, -y0 );

    if (s && strlen(s)) {
        while (*s) {
            glutBitmapCharacter(font, *s);
            s++;
        }
    }
}

}