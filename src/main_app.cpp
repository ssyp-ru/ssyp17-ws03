#include "RealEngine/base_app.h"
#include "main_app.h"

#include "window/windowXCB.h"
#include "window/windowGLUT.h"
#include "utils/logger.h"

#include <unistd.h>
#include <iostream>

namespace re{

// Global variable
static IWindowPtr       window_backend;
static GraphicalBackend backend_type;
static IBaseAppPtr      base_app;

void run_xcb_backend();
void run_openGL_backend();

IWindowPtr get_window() {
    return window_backend;
}

void init_graphics(GraphicalBackend backend_type_, int window_width, int window_height){
    backend_type = backend_type_;
    switch(backend_type){

        case GraphicalBackend::XCB :
            window_backend = std::make_shared<WindowXCB>(window_width, window_height);
            break;

        // make same case for openGL

        case GraphicalBackend::OPENGL :
            window_backend = std::make_shared<WindowGLUT>( window_width, window_height );
            break;

        default:
            std::cerr << "init_graphics: Unknown graphical backend type. Abort\n";
            exit(1);
    }
}


void run_base_app(IBaseApp* base_app_ptr){
    Log log("l1.txt");
    log.msg("Hello world!");
    base_app.reset(base_app_ptr);
    base_app->setup();
    window_backend->register_event_handler(base_app);

    while(1){
        log.track();
        window_backend->run_events();

        base_app->update();
        base_app->display();

        window_backend->display();
        log.msg("frame took " + std::to_string(log.track_elapsed()) + " ns.");
    }
    /*switch(backend_type){
        case GraphicalBackend::XCB : {
            run_xcb_backend();
            break;

        case GraphicalBackend::OPENGL :
            
        }
    }*/
    
}

/*
void run_xcb_backend(){
    Log log;
    log.l1() << "foo" << '\n';
    base_app->setup();
    window_backend->register_event_handler(base_app);

    while(1){
        window_backend->run_events();

        base_app->update();
        base_app->display();

        window_backend->display();
    }
}
// */
}