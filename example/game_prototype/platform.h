#pragma once
#include <RealEngine/graphic.h>
#include <RealEngine/graphic/image.h>
#include <RealEngine/graphic/animation.h>
#include <RealEngine/math.h>
#include "RealEngine/physic_core.h"
#include "drawableGameObject.h"

class Platform : public DrawableGameObject{
public:
    Platform(re::Vector2f pos, re::Vector2f size) : DrawableGameObject::DrawableGameObject(pos) 
    {
        addPoint(re::Vector2f(0, 0));
        addPoint(re::Vector2f(0, size.Y));
        addPoint(re::Vector2f(size.X, size.Y));
        addPoint(re::Vector2f(size.X, 0));
        addEdge(0, 1);
        addEdge(1, 2);
        addEdge(2, 3);
        addEdge(3, 0);
    }
};
