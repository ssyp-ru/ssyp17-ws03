#pragma once
#include "RealEngine/physic_core.h"
#include "drawableGameObject.h"

class DeathTrigger : public DrawableGameObject
{
public:
    DeathTrigger(re::Point2f pos, re::Point2f size);
    void onTriggerStay(re::PhysicObjectPtr obj);
    virtual void display(int k);
};
