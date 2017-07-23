#include "mainApp.h"

#include "utils/logger.h"
#include "opengl/opengl.h"

#include <unistd.h>
#include <iostream>

namespace re{

IBaseAppPtr MainApp::baseApp;

void runApp(int window_width, int window_height, IBaseAppPtr BaseApp){
    MainApp::baseApp = BaseApp;

    OpenGL::callbackPostInit = MainApp::init;
    OpenGL::callbackIdle = MainApp::tick;

    OpenGL::init( window_width, window_height, BaseApp );
    
    OpenGL::run();
}

void MainApp::init()
{
    baseApp->setup();
}

void MainApp::tick()
{
    baseApp->update();
    baseApp->display();

    OpenGL::draw();
}

}