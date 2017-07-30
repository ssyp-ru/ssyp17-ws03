#pragma once
#include <RealEngine/graphic.h>
#include <RealEngine/graphic/image.h>
#include <RealEngine/graphic/animation.h>
#include <RealEngine/math.h>
#include "RealEngine/physic_core.h"
#include "unit.h"

class Player : public Unit{
public:
    bool isGrounded = false;
    re::Animation movingAnim; // moving animation
    Player(re::Vector2f pos);
    void onCollisionStay(re::GameObjectPtr to, re::Vector2f vec);
    void attack();
    void update();
    void display(int scale);
};
