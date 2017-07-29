#pragma once
#include <RealEngine/graphic.h>
#include <RealEngine/graphic/image.h>
#include <RealEngine/graphic/animation.h>
#include <RealEngine/math.h>
#include "RealEngine/physic_core.h"
#include <functional>
#include <iostream>
#include "drawableGameObject.h"

class Player : public DrawableGameObject{
public:
    bool isGrounded = false;
    re::Animation movingAnim; // moving animation
    Player(re::Vector2f pos, re::Vector2f size) : DrawableGameObject::DrawableGameObject(pos) 
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
    void onCollisionStay(re::GameObjectPtr to, re::Vector2f vec) override
    {
        if (re::Vector2f(0, -1).angleBetween(vec) < 60.0 / 180.0 * 3.14159) 
        { 
            isGrounded = true;
        }
    }
    void display(int scale) override
    {
        re::draw_image(position.X * scale, position.Y * scale, movingAnim.getNextFrame());
    }
};
