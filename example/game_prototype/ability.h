#pragma once
#include "unit.h"
#include <iostream>

class Ability
{
private:
    int cooldown;
    int curCooldown;
    int requiredLevel;
protected:
    Ability(int requiredLevel, int cooldown)
    {
        this->requiredLevel = requiredLevel;
        this->cooldown = cooldown;
        curCooldown = 0;
    }
public:
    re::PhysicObjectPtr sender;
    void reduceCooldown() { curCooldown--;}
    bool canCast() { return (curCooldown <= 0); }
    virtual void cast() { curCooldown = cooldown; std::cout << "Ability casted.\n"; };
    virtual void update() { }
    int getRequiredLevel() { return requiredLevel; }
    int getCooldown() { return curCooldown; }
};