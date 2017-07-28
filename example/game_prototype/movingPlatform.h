#pragma once
#include <RealEngine/graphic.h>
#include <RealEngine/graphic/image.h>
#include <RealEngine/graphic/animation.h>
#include <RealEngine/math.h>
#include "RealEngine/physic_core.h"
#include "drawableGameObject.h"

class Platform : public Platform{
private:
    Vector2f start, end;
public:
    Platform(re::Vector2f pos, re::Vector2f size, re::Vector2f destination) : Platform::Platform(pos, size) 
    {
        start = pos;
        end = destination;
    }
    
};
