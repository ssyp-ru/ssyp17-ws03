#pragma once
#include <RealEngine/graphic.h>
#include <RealEngine/graphic/image.h>
#include <RealEngine/graphic/animation.h>
#include <RealEngine/math.h>
#include "RealEngine/physic_core.h"
#include <functional>
#include <iostream>

class Player : public re::GameObject{
public:
    bool isGrounded = false;
    re::Animation movingAnim; // moving animation
    Player(re::Vector2f pos, re::Vector2f size) : re::GameObject::GameObject(pos) 
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
    void onCollision(re::GameObjectPtr from, re::GameObjectPtr to, re::Vector2f vec)
    {
        if (re::Vector2f(0, -1).angleBetween(vec) < 45.0 / 180.0 * 3.14159) 
        { 
            isGrounded = true;
        }
    }
    std::function<void(re::GameObjectPtr, re::GameObjectPtr, re::Vector2f)> getCallback()
    {
        return std::bind(&Player::onCollision, this
            , std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    }
    void display(int scale);
};
