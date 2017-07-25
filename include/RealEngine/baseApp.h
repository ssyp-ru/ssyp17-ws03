#pragma once

#include "keyboard.h"
#include <memory>
#include <string>
namespace re{

class IBaseApp{
public:
    virtual void setup()    = 0;
    virtual void update()   = 0;
    virtual void display()  = 0;

    // Events handlers
    virtual void on_key_pressed(Key key) {}
    virtual void on_key_released(Key key) {}
    virtual void on_button_pressed(int button) {}
    virtual void on_mouse_move(int mouseX, int mouseY) {}

};
typedef std::shared_ptr<IBaseApp> IBaseAppPtr;

void runApp(int window_width, int window_height, IBaseAppPtr BaseApp);
void setWindowName( std::string name );

}