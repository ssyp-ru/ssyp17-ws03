#include "mainApp.h"

#include "window/opengl.h"
#include "graphics/particle.h"
#include "event_manager/event_manager.h"
#include <RealEngine/time.h>
#include <unistd.h>
#include <iostream>
#include <RealEngine/logger.h>

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

double Time::delta_time;
StopWatch Time::previous_time;

void MainApp::tick()
{
    Time::updateTime();

    send_all_event();

    baseApp->update();
    baseApp->display();
    re::Log::screen_msg();
    drawParticles( 0.1 );

    OpenGL::instance().draw();
}

}