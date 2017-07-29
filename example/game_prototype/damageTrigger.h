#pragma once
#include "RealEngine/physic_core.h"
#include "drawableGameObject.h"
/*#include <RealEngine/graphic.h>
#include <RealEngine/graphic/image.h>
#include <RealEngine/graphic/animation.h>
#include <RealEngine/math.h>
#include <RealEngine/time.h>
#include "unit.h"
#include "enemy.h"
#include "player.h"*/

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
