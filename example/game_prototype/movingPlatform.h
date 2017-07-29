#pragma once
#include <RealEngine/graphic.h>
#include <RealEngine/graphic/image.h>
#include <RealEngine/graphic/animation.h>
#include <RealEngine/math.h>
#include "RealEngine/physic_core.h"
#include "drawableGameObject.h"
#include "waypointPath.h"

class MovingPlatform : public Platform
{
public:
    WaypointPath *path;

    MovingPlatform(re::Vector2f pos, re::Vector2f size, double speed) : Platform::Platform(pos, size) 
    {
        path = new WaypointPath(speed, (DrawableGameObject*)this);
    }
    virtual void update() override
    {
        path->update();
    }
};
