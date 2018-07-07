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

    re::Point2f size;
public:
    WeakPlatform(re::Point2f pos, re::Point2f siz, double destroyTime);
    void onCollisionStay(re::PhysicObjectPtr to, re::Point2f vec);
    virtual void update();

    virtual void display(int scale);
};
