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

#include "platform.h"

class WeakPlatform : public Platform{
private:
    double hp;
    bool touched;

    static re::ImagePtr tex;

    re::Vector2f size;
public:
    WeakPlatform(re::Vector2f pos, re::Vector2f siz, double destroyTime);
    void onCollisionStay(re::GameObjectPtr to, re::Vector2f vec);
    virtual void update();

    virtual void display(int scale);
};
