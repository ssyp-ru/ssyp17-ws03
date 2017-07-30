#pragma once
#include <RealEngine/graphic.h>
#include <RealEngine/graphic/image.h>
#include <RealEngine/graphic/animation.h>
#include <RealEngine/math.h>
#include <functional>

//First ver: only rectangle buttons
//TODO: Make ability to create non-rectangle buttons (ex. circles, etc.)
//TODO: Make different states of button (ex. changes color after press)
//TODO: mouse-hover events?
/*
 * magic-shmagic button constants:
 * 0: left-mouse click,
 * 1: mid button click,
 * 2: right-mouse click,
 * 3: scroll up,
 * 4: scroll down.
 */
namespace re {

    class BaseButton {
        private:
            int x, y, xLength, yLength;
            bool active = true;
            std::string id;
            ImagePtr buttonImg, buttonImg_over;
            std::function<void()> actionFunc;
            
        public:
            BaseButton(Point2f point, std::string id, ImagePtr img, ImagePtr img_over);
            BaseButton(int xSource, int ySource, std::string id, ImagePtr img, ImagePtr img_over);
            BaseButton(int xSource, int ySource, int xLength, int yLength, std::string id);
            std::string get_id();
            void set_active(bool active);
            bool is_active();
            void display();
            void display_mouse_over();
            void register_action(std::function<void()> func);
            void action(int button); // mouse button
            bool check_if_mouse_over(int mouseX, int mouseY);
            bool check_if_mouse_over(Point2f point);
    };

    typedef std::shared_ptr<BaseButton> BaseButtonPtr;

} // namespace re