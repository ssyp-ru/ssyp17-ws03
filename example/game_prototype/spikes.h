#pragma once
#include "platform.h"
#include <iostream>

class Spikes : public Platform
{
private:
    double damage;
    double interval;
    re::StopWatch lastAttackTime;
public:
    Spikes(re::Point2f pos, re::Point2f size, double damage, double interval) : Platform::Platform(pos, size)
    {
        this->damage = damage;
        this->interval = interval;
        lastAttackTime = re::StopWatch();
    }
    bool canAttack()
    {
        return (lastAttackTime.stop_watch() / 1e9 > interval);
    }
    void onCollisionStay(re::PhysicObjectPtr obj, re::Point2f vec)
    {
        if (canAttack())
            if (/*(vec.angleBetween(re::Point2f(0, -1)) <= 100 / 180 * 3.14159) && */(std::dynamic_pointer_cast<Unit>(obj) != 0))
            {
                re::PhysicObjectPtr ptr;
                for (uint i = 0; i < (*worldContainer).size(); i++)
                {
                    if ((*worldContainer)[i].get() == this)
                    {
                        ptr = (*worldContainer)[i];
                        break;
                    }
                }
                lastAttackTime = re::StopWatch();
                std::dynamic_pointer_cast<Unit>(obj)->dealDamage(damage, ptr);
            }
    }
};