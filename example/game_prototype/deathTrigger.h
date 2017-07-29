#pragma once
#include "RealEngine/physic_core.h"
#include "drawableGameObject.h"

class DeathTrigger : public DrawableGameObject
{
public:
    DeathTrigger(re::Vector2f pos, re::Vector2f size);
    void onTriggerStay(re::GameObjectPtr obj);
};
