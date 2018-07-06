#pragma once
#include <RealEngine/graphic.h>
#include <RealEngine/graphic/image.h>
#include <RealEngine/graphic/animation.h>
#include <RealEngine/math.h>
#include "RealEngine/physic_core.h"
#include <functional>
#include <iostream>
#include <vector>

class DrawableGameObject : public re::PhysicObject
{
public:
    DrawableGameObject(re::Point2f pos) : re::PhysicObject::PhysicObject(pos) 
    {

    }
    virtual void update()
    {
        setCanChangeSpeed(true);
    }
    virtual void display(float scale)
    {
        for (auto curEdge : *getEdges())
        {
            re::draw_line((curEdge.P1->x + getPosition().x) * scale,
                         (curEdge.P1->y + getPosition().y) * scale,
                         (curEdge.P2->x + getPosition().x) * scale,
                         (curEdge.P2->y + getPosition().y) * scale,
                         re::BLACK);
        }
    }
};
