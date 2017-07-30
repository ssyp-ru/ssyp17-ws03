#pragma once
#include <RealEngine/graphic.h>
#include <RealEngine/graphic/image.h>
#include "RealEngine/physic_core.h"
#include <iostream>
#include "player.h"
#include <string>

class HUD
{
private:
    Player *curPlayer;
public:
    HUD(Player *currentPlayer)
    {
        curPlayer = currentPlayer;
    }
    void display()
    {
        std::string hpString;
        hpString = "";
        hpString += std::to_string((int)curPlayer->getHp());
        hpString += "/";
        hpString += std::to_string((int)curPlayer->getMaxHp());
        re::draw_rectangle(re::get_cam_x(), re::get_cam_y(), re::get_width(), 40, re::BLACK);    
        re::draw_rectangle(re::get_cam_x(), re::get_cam_y(), re::get_width() * curPlayer->getHp() / curPlayer->getMaxHp(), 35, re::DARKRED);    
        re::draw_text(re::get_cam_x() + re::get_width() / 2 - 20, re::get_cam_y() + 23, hpString, re::BLACK);
    }
};