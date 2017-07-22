#pragma once

#include "window_interface.h"
#include "RealEngine/graphic/image.h"

namespace re
{

    class WindowGLUT : public IWindow
    {
        public: 

        WindowGLUT( uint w, uint h );
        virtual ~WindowGLUT();

        virtual void background(Color color)                                override;
        virtual void draw_rectangle(int x0, int y0, int w, int h, Color color)   override;
        virtual void draw_line(int x0, int y0, int x1, int y1, Color color)      override;
        virtual void draw_circle(int x0, int y0, int r, Color color)             override;
        virtual void draw_image(int x0, int y0, ImagePtr im)                override;
        // virtual void draw_text(int x0, int y0, const std::string& text) override {}
        virtual void translate(int x, int y) override;
        virtual void scale(float x, float y) override;
        virtual void rotate(float angle) override;
        virtual void viewat(int x, int y) override; 

        virtual void display();
        virtual void run_events();
        virtual void register_event_handler(IBaseAppPtr e) override;
        virtual void set_fps(uint fps_count) override ;
        virtual uint get_fps() override ;
        virtual uint get_width() override ;
        virtual uint get_height() override ;

        private:

        uint fps;
        uint w, h;

        uint startList;
    };
}