#include <RealEngine/graphic.h>
#include <RealEngine/graphic/image.h>
#include <RealEngine/graphic/animation.h>
#include <RealEngine/math.h>
#include "base_button.h"
#include <functional>
#include <iostream>

namespace re{

    BaseButton::BaseButton(Point2f point, ImagePtr img){
        x = point.x;
        y = point.y;
        buttonImg = img;
        xLength = img->w;
        yLength = img->h;
    }
    BaseButton::BaseButton(int xSource, int ySource, ImagePtr img){
        x = xSource;
        y = ySource;
        buttonImg = img;
        xLength = img->w;
        yLength = img->h;
    }

    bool BaseButton::check_if_mouse_over(int mouseX, int mouseY){
        if(mouseX >= x && mouseX <= x + xLength){
            if(mouseY >= y && mouseY <= y + yLength){
                return true;
            }
        }
        return false;
    }
    bool BaseButton::check_if_mouse_over(Point2f point){
        if(point.x >= x && point.x <= x + xLength){
            if(point.y >= y && point.y <= y + yLength){
                return true;
            }
        }
        return false;
    }
    void BaseButton::register_action(std::function<void()> func){
        std::cout << "GSB reg " <<  std::endl;
        actionFunc = func;
    }
    void BaseButton::action(int button){
        std::cout << "GSB action " <<  std::endl;
        actionFunc();
    }
    void BaseButton::display(){
        re::draw_image(x, y, buttonImg);
    }
} // namespace re