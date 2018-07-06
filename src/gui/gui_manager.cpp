#include <RealEngine/gui/base_button.h>
#include <RealEngine/gui/gui_manager.h>

#include <memory>

namespace re {

    void GuiManager::register_button(BaseButtonPtr button, std::string layer) {
        layers[layer].push_back(button);
    }

    void GuiManager::display(int mouseX, int mouseY) {
        for (auto layer : layers) {
            for (auto button : layer.second) {
                if (button->check_if_mouse_over(mouseX, mouseY))
                    button->display_mouse_over();
                else
                    button->display();
            }
        }
    }


    void GuiManager::on_click(int mouse_button, int mouseX, int mouseY) {
        /*
         * magic-shmagic button constants:
         * 0 - LMB
         * 1 - MMB (wheel)
         * 2 - RMB
         * 3 - Wheel up
         * 4 - Wheel down
         */
        if (mouse_button == 0) {
            for (auto layer : layers) {
                for (auto button : layer.second) {
                    if (button->check_if_mouse_over(mouseX, mouseY) && button->is_active()){
                        button->action(mouse_button);
                    }
                }
            }
        }
    }

    BaseButtonPtr GuiManager::get_button_by_id(std::string id) {
        for (auto layer : layers) {
            for (auto button : layer.second) {
                if (button->get_id() == id) {
                    return button;
                }
            }
        }
        return nullptr;
    }

    void GuiManager::layer_set_active(std::string layer, bool active) {
        for (auto button : layers[layer]) {
            button->set_active(active);
        }
    }
}