#pragma once

#include "RealEngine/graphic/image.h"
#include "RealEngine/base_app.h"

#include <memory>

namespace re{

class IWindow{
public:
    // Render
    virtual void background(Color color)                                    = 0;
    virtual void draw_rectangle(int x0, int y0, int w, int h, Color color)  = 0;
    virtual void draw_line(int x0, int y0, int x1, int y1, Color color)     = 0;
    virtual void draw_circle(int x0, int y0, int r, Color color)            = 0;
    virtual void draw_image(int x0, int y0, ImagePtr im)                    = 0;
    // virtual void draw_text(int x0, int y0, const std::string& text)             = 0;
    virtual void translate(int x, int y)                                    = 0;
    virtual void scale(float x, float y)                                    = 0;
    virtual void rotate(float angle)                                        = 0;
    virtual void viewat(int x, int y) {}

    virtual void display()                  = 0;
    virtual void run_events()               = 0;
    virtual void register_event_handler(IBaseAppPtr e) = 0;
    
    virtual void set_fps(uint fps_count)     = 0;
    virtual uint get_fps()                  = 0;
    virtual uint get_width()                = 0;
    virtual uint get_height()               = 0;

    virtual void run() {}


    virtual ~IWindow() {}
};
typedef std::shared_ptr<IWindow> IWindowPtr;

} // namespace re