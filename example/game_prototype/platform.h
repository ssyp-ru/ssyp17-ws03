#pragma once
#include <RealEngine/graphic.h>
#include <RealEngine/graphic/image.h>
#include <RealEngine/graphic/animation.h>
#include <RealEngine/math.h>
#include "RealEngine/physic_core.h"
#include "drawableGameObject.h"

class Platform : public DrawableGameObject{
public:
    Platform(re::Point2f pos, re::Point2f size) : DrawableGameObject::DrawableGameObject(pos) 
    {
        setRigidbodySimulated(false);
        setFriction(1.0);
        setBounciness(0.0);
        addPoint(re::Point2f(0, 0));
        addPoint(re::Point2f(0, size.y));
        addPoint(re::Point2f(size.x, size.y));
        addPoint(re::Point2f(size.x, 0));
        addEdge(0, 1);
        addEdge(1, 2);
        addEdge(2, 3);
        addEdge(3, 0);
    }
    void display(int scale) override
    {

    }
};
