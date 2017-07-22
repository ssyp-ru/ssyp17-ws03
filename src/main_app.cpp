//#include "RealEngine/base_app.h"
#include "window/window_interface.h"
#include "main_app.h"

#include "window/windowGLUT.h"
#include "utils/logger.h"

#include "recource_manager/xml_parser.h"

#include <unistd.h>
#include <iostream>

namespace re{

// Global variable
static IWindowPtr       window_backend;
static GraphicalBackend backend_type;
static IBaseAppPtr      base_app;

re::IWindowPtr get_window() {
    return window_backend;
}

void runApp(int window_width, int window_height, IBaseApp* BaseApp){
    new WindowGLUT( window_width, window_height, BaseApp );
    //lost control
}

void base_app_init(IBaseApp* base_app_ptr, IWindowPtr window_ptr)
{
    window_backend = window_ptr;

    base_app.reset(base_app_ptr);

    base_app->setup();
}

<<<<<<< HEAD
void base_app_tick(IBaseApp* base_app_ptr)
{
    window_backend->run_events();

    log.track();

    base_app->update();
    base_app->display();

    window_backend->display();
    log.msg("frame took " + std::to_string(log.track_elapsed()) + " ns.");
}

}