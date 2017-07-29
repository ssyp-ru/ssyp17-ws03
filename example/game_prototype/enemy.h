#pragma once
#include "unit.h"
#include "player.h"
#include <RealEngine/physic_core.h>

class Enemy: public Unit {
public:
    re::Animation movingAnim; // moving animation
    Enemy(re::Vector2f pos, re::Vector2f size) : Unit::Unit(pos, size) 
    {
        
    }
    virtual void onCollisionStay(re::GameObjectPtr to, re::Vector2f vec) override
    {

    }
    virtual void attack() override
    {
        
    }
    virtual void update() override
    {
        
    }
};