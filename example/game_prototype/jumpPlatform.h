#pragma once
#include <RealEngine/graphic.h>
#include <RealEngine/graphic/image.h>
#include <RealEngine/graphic/animation.h>
#include <RealEngine/math.h>
#include "RealEngine/physic_core.h"
#include "drawableGameObject.h"
#include "waypointPath.h"
#include "player.h"

class JumpPlatform : public Platform
{
private:
    double power;
public:
    JumpPlatform(re::Point2f pos, re::Point2f size, double power) : Platform::Platform(pos, size) 
    {
        this->power = power;
    }
    void onCollisionStay(re::PhysicObjectPtr obj, re::Point2f vec)
    {
        if (std::dynamic_pointer_cast<Player>(obj) != 0)
            std::dynamic_pointer_cast<Player>(obj)->isGrounded = false;
        obj->addImpulse(re::Point2f(0, -power * obj->getMass()));
    }
};
    