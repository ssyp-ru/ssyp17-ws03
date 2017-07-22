#include "windowGLUT.h"

namespace re
{
    WindowGLUT::WindowGLUT( uint w, uint h )
    {
        this->w = w;
        this->h = h;
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
    {}

    void WindowGLUT::draw_rectangle(int x0, int y0, int w, int h, Color color)
    {}
        
    void WindowGLUT::draw_line(int x0, int y0, int x1, int y1, Color color)
    {}
        
    void WindowGLUT::draw_circle(int x0, int y0, int r, Color color)
    {}
    
    void WindowGLUT::draw_image(int x0, int y0, ImagePtr im)
    {}
        // virtual void draw_text(int x0, int y0, const std::string& text) override {}
    void WindowGLUT::translate(int x, int y) 
    {}
    
    void WindowGLUT::scale(float x, float y) 
    {}
    
    void WindowGLUT::rotate(float angle) 
    {}

    void WindowGLUT::display()
    {}

    void WindowGLUT::run_events()
    {}

    void WindowGLUT::register_event_handler(IBaseAppPtr e)
    {}

    uint WindowGLUT::get_width()
    {
    }

    uint WindowGLUT::get_height()
    {}

}