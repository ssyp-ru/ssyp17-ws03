#pragma once

#include "window_interface.h"

#include "RealEngine/graphic/image.h"

#include "xcb/xcb.h"
#include <xkbcommon/xkbcommon.h>
#include <xkbcommon/xkbcommon-x11.h>
#include <xcb/xcb_image.h>
#include <xcb/xcb_icccm.h>

#include <chrono>

namespace re{

class WindowXCB
    : public IWindow
{
public:
    WindowXCB(uint width, uint height);
    virtual ~WindowXCB();
    
    // Render
    virtual void background(Color color)                                override;
    virtual void draw_rectangle(int x0, int y0, int w, int h, Color color)   override;
    virtual void draw_line(int x0, int y0, int x1, int y1, Color color)      override;
    virtual void draw_circle(int x0, int y0, int r, Color color)             override;
    virtual void draw_image(int x0, int y0, ImagePtr im)                override;
    // virtual void draw_text(int x0, int y0, const std::string& text) override {}
    virtual void translate(int x, int y) override {}
    virtual void scale(float x, float y) override {}
    virtual void rotate(float angle) override {}

    virtual void display();
    virtual void run_events();
    virtual void register_event_handler(IBaseAppPtr e) override;
    virtual void set_fps(uint fps_count) override ;
    virtual uint get_fps() override ;
    virtual uint get_width() override ;
    virtual uint get_height() override ;

private:
    xcb_connection_t    *conn;
    xcb_screen_t        *screen;
    xcb_window_t        window;
    xcb_gcontext_t      color_context, gc;
    struct xkb_context  *ctx;
    struct xkb_state    *state;
    struct xkb_keymap   *keymap;
    uint32_t            mask;
    uint32_t            values[3];
    xcb_image_t         *back_image;
    xcb_pixmap_t        pmap;
    xcb_size_hints_t    *hints;
    uint8_t             *image32;

    IBaseAppPtr         event_handler;

    xcb_colormap_t    colormap;

    int fps_count_max, wait_period_mic, frame_count, fps;
    std::chrono::time_point<std::chrono::steady_clock> last_frame_time, last_second;


    uint16_t width, height;
};

} // namespace re