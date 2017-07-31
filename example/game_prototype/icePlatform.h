#pragma once
#include <RealEngine/baseApp.h>
#include <RealEngine/graphic.h>
#include <RealEngine/graphic/image.h>
#include <RealEngine/graphic/animation.h>
#include <RealEngine/math.h>
#include "RealEngine/physic_core.h"
#include "drawableGameObject.h"
#include <RealEngine/time.h>
#include "platform.h"
#include "player.h"

class IcePlatform : public Platform{
public:
    IcePlatform(re::Vector2f pos, re::Vector2f size) : Platform::Platform(pos, size) 
    {
        setFriction(0.0);
        setBounciness(0.0);
    }
    void onCollisionStay(re::GameObjectPtr to, re::Vector2f vec) override
    {
        if (std::dynamic_pointer_cast<Player>(to) != 0)
        {
            if (to->getVelocity().Length() > 2.5)
            {
                std::dynamic_pointer_cast<Player>(to)->isGrounded = false;
                to->setCanChangeSpeed(false);
            } else 
            {
                to->setCanChangeSpeed(true);
                std::dynamic_pointer_cast<Player>(to)->isGrounded = true;
            }
        }
    }
    virtual void update() override
    {
        
    }
};
