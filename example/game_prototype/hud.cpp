#include "hud.h"
#include "ability_damageBoost.h"
#include "ability_heal.h"
#include "ability_invincibility.h"
#include <iostream>

HUD::HUD(Player *currentPlayer, re::ResourceManager *rm)
{
    curPlayer = currentPlayer;
    this->rm = rm;
}
void HUD::display()
{
    std::string hpString;
    hpString = "";
    hpString += std::to_string((int)curPlayer->getHp());
    hpString += "/";
    hpString += std::to_string((int)curPlayer->getMaxHp());
    re::draw_rectangle(re::get_cam_x(), re::get_cam_y(), re::get_width(), 40, re::BLACK);   
    re::draw_rectangle(re::get_cam_x(), re::get_cam_y() + 40, 100, 60, re::BLACK);    
    re::draw_rectangle(re::get_cam_x(), re::get_cam_y(), re::get_width() * curPlayer->getHp() / curPlayer->getMaxHp(), 35, re::DARKRED);    
    re::draw_text(re::get_cam_x() + re::get_width() / 2 - 20, re::get_cam_y() + 23, hpString, re::GRAY);
    re::draw_text(re::get_cam_x() + 35, re::get_cam_y() + 85, "dmg: " + std::to_string((int)curPlayer->getDamage()), re::GRAY);
    re::draw_image_part(re::get_cam_x() + 5, re::get_cam_y() + 45, re::get_cam_x() + 45, re::get_cam_y() + 85,
            0, 0, 1, 1, rm->get_image("fork"));
    re::draw_rectangle(re::get_cam_x() + re::get_width() - curPlayer->getAbilities()->size() * 50 - (curPlayer->getAbilities()->size() + 1) * 10, re::get_cam_y() + 40, 250, 70, re::BLACK);
    uint size = curPlayer->getAbilities()->size();
    for (uint i = 0; i < size; i++)
    {
        re::ImagePtr ptr = nullptr;
        switch (i)
        {
            case 0:
                ptr = rm->get_image("ability_damageBoost");
                break;
            case 1:
                ptr = rm->get_image("ability_heal");
                break;
            case 2:
                ptr = rm->get_image("ability_invincibility");
                break;
            default:
                re::draw_rectangle(re::get_cam_x() + re::get_width() - (size - i) * 60,
                           re::get_cam_y() + 50, 50, 50, re::LIGHTGRAY);
                break;
        }
        //Ability *curAb = (*(curPlayer->getAbilities()))[i];
        //if ((Ability_DamageBoost*)(curAb))
        //    ptr = rm->get_image("ability_damageBoost");
        //if ((Ability_Heal*)(curAb))
        //    ptr = rm->get_image("ability_heal");
        //if ((Ability_Invincibility*)(curAb))
        //    ptr = rm->get_image("ability_invincibility");
        if (ptr != nullptr)
            re::draw_image_part(re::get_cam_x() + re::get_width() - (size - i) * 60,
                                re::get_cam_y() + 50,
                                re::get_cam_x() + re::get_width() - (size - i) * 60 + 50,
                                re::get_cam_y() + 50 + 50, 0, 0, 1, 1, 
                                ptr);
        if (((*curPlayer->getAbilities())[i]->getRequiredLevel() <= curPlayer->getLevel()) && ((*curPlayer->getAbilities())[i]->getCooldown() > 0))
            re::draw_text(re::get_cam_x() + re::get_width() - (size - i) * 60 + 20, re::get_cam_y() + 80, std::to_string((*curPlayer->getAbilities())[i]->getCooldown()), re::BLACK);
    }
    for (uint i = 0; i < size; i++)
    {
        if (((*curPlayer->getAbilities())[i]->getCooldown() > 0) || ((*curPlayer->getAbilities())[i]->getRequiredLevel() > curPlayer->getLevel()))
            re::draw_rectangle(re::get_cam_x() + re::get_width() - (size - i) * 60, re::get_cam_y() + 50, 50, 50, re::TRANSPARENTBLACK);
    }
}