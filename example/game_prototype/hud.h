#pragma once
#include <RealEngine/graphic.h>
#include <RealEngine/graphic/image.h>
#include "RealEngine/physic_core.h"
#include <iostream>
#include "player.h"
#include <string>
#include "RealEngine/resource_manager.h"

class HUD
{
private:
    Player *curPlayer;
    re::ResourceManager *rm;
public:
    HUD(Player *currentPlayer, re::ResourceManager *rm);
    void display();
};