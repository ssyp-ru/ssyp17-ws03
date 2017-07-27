#pragma once
#include <RealEngine/graphic.h>
#include <RealEngine/graphic/image.h>
#include <RealEngine/graphic/animation.h>
#include <RealEngine/math.h>

class Player{
    private:
    re::Point2f playercoords;
    int curVelocity = 0; // goes 0 by default
    int moveVelocity;
    re::Animation movingAnim; // moving animation
    public:
    Player() : playercoords(0, 0){}
    void updatePosition();
    void move(bool isForward);
    void stop();
    Player(int startingX, int startingY, int vel, const re::Animation& movAnim);
    re::Point2f getCurPosition();
    void display();
    };
