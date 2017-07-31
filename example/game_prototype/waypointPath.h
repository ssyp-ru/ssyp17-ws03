#pragma once
#include <RealEngine/graphic.h>
#include <RealEngine/graphic/image.h>
#include <RealEngine/graphic/animation.h>
#include <RealEngine/math.h>
#include "RealEngine/physic_core.h"
#include "drawableGameObject.h"
#include <iostream>

class WaypointPath
{
    std::vector<re::Vector2f> waypoints;
    int nextPoint;
    int curDirection;
    double speed;
    bool isActivated;
    bool isCycled; // true - at end of waypoints will go to [0] waypoint, false - will go reverse way
    DrawableGameObject *owner;
    int getPrevIndex()
    {
        if (curDirection == 1)
        {
            if ((nextPoint == 0) && (isCycled)) return waypoints.size() - 1;
            return nextPoint - 1;
        } else {
            if (((uint)nextPoint == waypoints.size() - 1) && (isCycled)) return 0;
            return nextPoint + 1;
        }
    }
    int goNextIndex()
    {
        if (curDirection == 1)
        {
            if ((uint)nextPoint == waypoints.size() - 1)
            {
                if (isCycled) return 0;
                curDirection = -1;
                return nextPoint - 1;
            }
            return nextPoint + 1;
        } else {
            if (nextPoint == 0)
            {
                if (isCycled) return waypoints.size() - 1;
                curDirection = 1;
                return nextPoint + 1;
            }
            return nextPoint - 1;
        }
    }
    void goNextWaypoint()
    {
        int curPoint = nextPoint;
        nextPoint = goNextIndex();
        owner->setVelocity((waypoints[nextPoint] - waypoints[curPoint]).Normalized() * speed);
    }
public:
    WaypointPath(double speed, DrawableGameObject *owner) 
    {
        nextPoint = 1;
        curDirection = 1;
        isActivated = false;
        waypoints.push_back(owner->getPosition());
        this->speed = speed;
        this->owner = owner;
    }
    void addWaypoint(re::Vector2f destination)
    {
        waypoints.push_back(destination);
    }
    void setCycled(bool value)
    {
        isCycled = value;
    }
    bool getCycled()
    {
        return isCycled;
    }
    void setActivated(bool value)
    {
        if (value == false)
            owner->setVelocity(re::Vector2f(0, 0));
        if ((value == true) && (waypoints.size() > 1)) 
            owner->setVelocity((waypoints[nextPoint] - waypoints[getPrevIndex()]).Normalized() * speed);
        isActivated = value;
    }
    bool getActivated()
    {
        return isActivated;
    }
    void update()
    {
        if (((re::GameObject)*owner).getCanChangeSpeed() == false)
            ((re::GameObject)*owner).setCanChangeSpeed(true);
        if ((isActivated) && (waypoints.size() > 1))
        {
            owner->setVelocity((waypoints[nextPoint] - owner->getPosition()).Normalized() * speed);
            if ((owner->getPosition() - waypoints[nextPoint]).Length() <= 0.25)
                goNextWaypoint();
        }
    }
};
