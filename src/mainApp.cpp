#include "mainApp.h"

#include "window/opengl.h"
#include "graphics/particle.h"

#include <unistd.h>
#include <iostream>

namespace re{

IBaseAppPtr MainApp::baseApp;

void setWindowName( std::string name )
{
    OpenGL::instance().setWindowName( name );
}

void runApp(int window_width, int window_height, IBaseAppPtr BaseApp){
    MainApp::baseApp = BaseApp;

    OpenGL::instance().callbackPostInit = MainApp::init;
    OpenGL::instance().callbackIdle = MainApp::tick;

    OpenGL::instance().init( window_width, window_height, BaseApp );
    
    OpenGL::instance().run();
}

void exitApp()
{
    OpenGL::instance().close();
}

void MainApp::init()
{
    initParticles();
    baseApp->setup();
}

long MainApp::delta_time = 0;
StopWatch MainApp::previous_time;

void MainApp::tick()
{
    StopWatch now;
    delta_time = previous_time.diff(now);
    previous_time = now;
    baseApp->update();
    baseApp->display();
    
    drawParticles( 0.1 );

    OpenGL::instance().draw();
}

}