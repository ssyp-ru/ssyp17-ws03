#pragma once
#include "RealEngine/physic_core.h"
#include "drawableGameObject.h"

class DamageTrigger : public DrawableGameObject{
private:
    double lifeTime;
    double damage;
    re::PhysicObjectPtr sender;
public:
    DamageTrigger(re::Point2f pos, re::Point2f size, double damage, re::PhysicObjectPtr sender);
    virtual void onTriggerStay(re::PhysicObjectPtr obj);
    virtual void update();
    virtual void display(int k);
};
