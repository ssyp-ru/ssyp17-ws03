#pragma once

#include "window/window_interface.h"

namespace re{

IWindowPtr get_window();

void base_app_init(IBaseApp* base_app_ptr, IWindow *window_ptr);
void base_app_tick(IBaseApp* base_app_ptr);

}  // namespace re