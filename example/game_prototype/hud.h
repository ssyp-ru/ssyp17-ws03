#pragma once
#include <RealEngine/graphic.h>
#include <RealEngine/graphic/image.h>
#include "RealEngine/physic_core.h"
#include <iostream>
#include "player.h"

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
        re::draw_rectangle(100, 100, 100, 100, re::BLACK);    
    }
};