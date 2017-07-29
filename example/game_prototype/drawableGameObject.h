#pragma once
#include <RealEngine/graphic.h>
#include <RealEngine/graphic/image.h>
#include <RealEngine/graphic/animation.h>
#include <RealEngine/math.h>
#include "RealEngine/physic_core.h"
#include <functional>
#include <iostream>
#include <vector>

class DrawableGameObject : public re::GameObject
{
public:
    DrawableGameObject(re::Vector2f pos) : re::GameObject::GameObject(pos) 
    {

    }
    virtual void update()
    {
        setCanChangeSpeed(true);
    }
    virtual void display(int scale)
    {
        for (auto curEdge : *getEdges())
        {
            re::draw_line((curEdge.P1->X + getPosition().X) * scale,
                         (curEdge.P1->Y + getPosition().Y) * scale,
                         (curEdge.P2->X + getPosition().X) * scale,
                         (curEdge.P2->Y + getPosition().Y) * scale,
                         re::BLACK);
        }
    }
};
