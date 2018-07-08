#include <RealEngine/graphic.h>
#include <RealEngine/graphic/image.h>
#include <RealEngine/graphic/animation.h>
#include <RealEngine/math.h>
#include <RealEngine/gui/base_button.h>
#include <functional>
#include <iostream>

namespace re {

BaseButton::BaseButton(int xSource, int ySource, std::string id, ImagePtr img, ImagePtr img_over){
    x = xSource;
    y = ySource;
    buttonImg = img;
    buttonImg_over = img_over;
    xLength = img->w;
    yLength = img->h;
    this->id = id;
}
BaseButton::BaseButton(Point2f point, std::string id, ImagePtr img, ImagePtr img_over) 
    : BaseButton::BaseButton(point.x, point.y, id, img, img_over) 
{}

BaseButton::BaseButton(int xSource, int ySource, int xLength, int yLength, std::string id) {
    x = xSource;
    y = ySource;
    this->xLength = xLength;
    this->yLength = yLength;
    buttonImg = nullptr;
    buttonImg_over = nullptr;
    this->id = id;
}

BaseButton::BaseButton(int xSource, int ySource, int xLength, int yLength, std::string id, 
                        std::string text, Color baseColor, Color textColor, Color overColor) {
    x = xSource;
    y = ySource;
    this->xLength = xLength;
    this->yLength = yLength;
    buttonImg = nullptr;
    buttonImg_over = nullptr;
    this->baseColor = baseColor;
    this->textColor = textColor;
    this->overColor = overColor;
    this->id = id;
}

std::string BaseButton::get_id() {
    return id;
}

void BaseButton::set_active(bool active) {
    this->active = active;
}

bool BaseButton::is_active() {
    return active;
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
    return check_if_mouse_over(point.x, point.y);
}

void BaseButton::register_action(std::function<void()> func){
    actionFunc = func;
}

void BaseButton::action(int button){
    actionFunc();
}

void BaseButton::display(){    
    if (active && buttonImg){
        draw_image(x, y, buttonImg);
    }else if(active){
        draw_rectangle(x, y, xLength, yLength, baseColor);
        draw_text(x, y, text, textColor);
    }

}

void BaseButton::display_mouse_over(){
    if (active && buttonImg_over){
        draw_image_part(x, y,xLength, yLength, 0, 0, 1, 1, buttonImg_over);
    }else if(active){
        draw_rectangle(x, y, xLength, yLength, overColor);
        draw_text(x, y, text, textColor);
    }
}

Point2f BaseButton::get_pos(){
    Point2f pos;
    pos.x = x;
    pos.y = y;
    return pos;
}

} // namespace re