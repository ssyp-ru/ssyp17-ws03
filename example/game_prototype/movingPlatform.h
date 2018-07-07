#pragma once
#include <RealEngine/graphic.h>
#include <RealEngine/graphic/image.h>
#include <RealEngine/graphic/animation.h>
#include <RealEngine/math.h>
#include "RealEngine/physic_core.h"
#include "drawableGameObject.h"
#include "waypointPath.h"
#include "platform.h"

class MovingPlatform : public Platform
{
    static re::ImagePtr tex;
public:
    WaypointPath *path;

    MovingPlatform(re::Point2f pos, re::Point2f size, double speed);
    virtual void display(int scale);
    virtual void update();
};
