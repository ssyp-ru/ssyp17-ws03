#include "mainApp.h"

#include "utils/logger.h"
#include "opengl/opengl.h"

#include <unistd.h>
#include <iostream>

namespace re{

IBaseAppPtr MainApp::baseApp;

void runApp(int window_width, int window_height, IBaseAppPtr BaseApp){
    MainApp::baseApp = BaseApp;

    GL.callbackPostInit = MainApp::init;
    GL.callbackIdle = MainApp::tick;

    GL.init( window_width, window_height, BaseApp );
    
    GL.run();
}

void MainApp::init()
{
    baseApp->setup();
}

void MainApp::tick()
{
    baseApp->update();
    baseApp->display();

    GL.draw();
}

}