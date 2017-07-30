#pragma once

#include <RealEngine/gui/base_button.h>

#include <vector>
#include <string>
#include <map>

namespace re {

    class GuiManager {

        private:
            std::map<std::string, std::vector<BaseButtonPtr>> layers;

        public:
            void layer_set_active(std::string layer, bool active);
            void register_button(BaseButtonPtr button, std::string layer);
            void display(int mouseX, int mouseY);
            void on_click(int mouse_button, int mouseX, int mouseY);
            BaseButtonPtr get_button_by_id(std::string id);
    };

}