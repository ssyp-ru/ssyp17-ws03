#pragma once
#include "ability.h"
#include "time.h"

class Ability_Invincibility : public Ability
{
private:
    double longevity;
    double timeToDeactivate;
public:
    Ability_Invincibility(double longevity) : Ability::Ability(1, 2)
    {
        timeToDeactivate = 0;
        this->longevity = longevity;
    }
    virtual void cast() override 
    { 
        Ability::cast(); 
        timeToDeactivate = longevity;
        std::dynamic_pointer_cast<Unit>(sender)->setIsInvincible(true);
    };
    virtual void update() override 
    {
        if (timeToDeactivate > 0)
        {
            timeToDeactivate -= re::Time::delta_time;
            if (timeToDeactivate <= 0)
                std::dynamic_pointer_cast<Unit>(sender)->setIsInvincible(false);
        }
    }
};