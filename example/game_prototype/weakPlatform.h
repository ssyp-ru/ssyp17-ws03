#pragma once
#include <RealEngine/baseApp.h>
#include <RealEngine/graphic.h>
#include <RealEngine/graphic/image.h>
#include <RealEngine/graphic/animation.h>
#include <RealEngine/math.h>
#include "RealEngine/physic_core.h"
#include "drawableGameObject.h"
#include <RealEngine/time.h>
#include <iostream>

class WeakPlatform : public Platform{
private:
    double hp;
    bool touched;
public:
    WeakPlatform(re::Vector2f pos, re::Vector2f size, double destroyTime) : Platform::Platform(pos, size) 
    {
        touched = false;
        hp = destroyTime;
        setFriction(1.0);
        setBounciness(0.0);
    }
    void onCollision(re::GameObjectPtr to, re::Vector2f vec) override
    {
        if (re::Vector2f(0, 1).angleBetween(vec) < 30.0 / 180.0 * 3.14159)
            touched = true;
    }
    virtual void update() override
    {
        if (touched) hp -= re::Time::delta_time;
        if (hp <= 0) destroy();
    }
};
