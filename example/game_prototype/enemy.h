#pragma once
#include "player.h"
#include "damageTrigger.h"
#include <RealEngine/physic_core.h>

class Enemy: public Unit {
public:
    re::Animation movingAnim; // moving animation
    Enemy(re::Vector2f pos, re::Vector2f size) : Unit::Unit(pos, size) 
    {
        setFriction(0.5);
    }
    virtual void onCollisionStay(re::GameObjectPtr to, re::Vector2f vec) override
    { }
    virtual void onDeath(re::GameObjectPtr sender) override { }
    virtual void attack() override { }
    virtual void update() override { Unit::update(); }
};