#pragma once
#include "RealEngine/physic_core.h"
#include "drawableGameObject.h"

class DamageTrigger : public DrawableGameObject{
private:
    double lifeTime;
    double damage;
    re::GameObjectPtr sender;
public:
    DamageTrigger(re::Vector2f pos, re::Vector2f size, double damage, re::GameObjectPtr sender);
    virtual void onTriggerStay(re::GameObjectPtr obj);
    virtual void update();
};
