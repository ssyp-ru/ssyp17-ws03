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
/*magic-shmagic button constants:
        0: left-mouse click,
        1: mid button click,
        2: right-mouse click,
        3: scroll up,
        4: scroll down.*/
namespace re{

class BaseButton{
private:
    int x, y, xLength, yLength;
    ImagePtr buttonImg;
    std::function<void()> actionFunc;
public:
    BaseButton(Point2f point, ImagePtr img);
    BaseButton(int xSource, int ySource, ImagePtr img);
    void display();
    void register_action(std::function<void()> func);
    void action(int button); // mouse button
    bool check_if_mouse_over(int mouseX, int mouseY);
    bool check_if_mouse_over(Point2f point);
};

} //namespace re