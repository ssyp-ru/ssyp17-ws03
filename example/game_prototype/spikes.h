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
    Spikes(re::Vector2f pos, re::Vector2f size, double damage, double interval) : Platform::Platform(pos, size)
    {
        this->damage = damage;
        this->interval = interval;
        lastAttackTime = re::StopWatch();
    }
    bool canAttack()
    {
        return (lastAttackTime.stop_watch() / 1e9 > interval);
    }
    void onCollisionStay(re::GameObjectPtr obj, re::Vector2f vec)
    {
        if (canAttack())
            if (/*(vec.angleBetween(re::Vector2f(0, -1)) <= 100 / 180 * 3.14159) && */(std::dynamic_pointer_cast<Unit>(obj) != 0))
            {
                re::GameObjectPtr ptr;
                for (unsigned int i = 0; i < (*worldContainer).size(); i++)
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