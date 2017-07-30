#pragma once
#include "ability.h"

class Ability_Heal : public Ability
{
private:
    double healAmount;
public:
    Ability_Heal(double healAmount) : Ability::Ability(2, 2)
    {
        this->healAmount = healAmount;
    }
    virtual void cast() override 
    { 
        Ability::cast(); 
        std::dynamic_pointer_cast<Unit>(sender)->heal(healAmount);
    };
    virtual void update() override 
    {
        
    }
};