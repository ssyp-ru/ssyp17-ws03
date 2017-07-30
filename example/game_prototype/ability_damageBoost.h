#pragma once
#include "ability.h"
#include "time.h"

class Ability_DamageBoost : public Ability
{
private:
    double longevity;
    double damage;
    double timeToDeactivate;
public:
    Ability_DamageBoost(double longevity, double damage) : Ability::Ability(1, 2)
    {
        timeToDeactivate = 0;
        this->longevity = longevity;
        this->damage = damage;
    }
    virtual void cast() override 
    { 
        Ability::cast(); 
        timeToDeactivate = longevity;
        std::dynamic_pointer_cast<Unit>(sender)->setDamage(damage + std::dynamic_pointer_cast<Unit>(sender)->getDamage());
    };
    virtual void update() override 
    {
        if (timeToDeactivate > 0)
        {
            timeToDeactivate -= re::Time::delta_time;
            if (timeToDeactivate <= 0)
            {
                std::dynamic_pointer_cast<Unit>(sender)->setDamage(std::dynamic_pointer_cast<Unit>(sender)->getDamage() - damage);
            }
        }
    }
};