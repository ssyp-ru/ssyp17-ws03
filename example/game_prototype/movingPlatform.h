#pragma once
#include <RealEngine/graphic.h>
#include <RealEngine/graphic/image.h>
#include <RealEngine/graphic/animation.h>
#include <RealEngine/math.h>
#include "RealEngine/physic_core.h"
#include "drawableGameObject.h"

class MovingPlatform : public Platform{
private:
    std::vector<re::Vector2f> waypoints;
    int nextPoint;
    int curDirection;
    double speed;
    bool isActivated;
    bool isCycled; // true - at end of waypoints will go to [0] waypoint, false - will go reverse way
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
        setVelocity((waypoints[nextPoint] - waypoints[curPoint]).Normalized() * speed);
    }
public:
    MovingPlatform(re::Vector2f pos, re::Vector2f size, double speed) : Platform::Platform(pos, size) 
    {
        nextPoint = 1;
        curDirection = 1;
        isActivated = false;
        waypoints.push_back(pos);
        this->speed = speed;
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
            setVelocity(re::Vector2f(0, 0));
        if (value == true)
        {
            setVelocity((waypoints[nextPoint] - waypoints[getPrevIndex()]).Normalized() * speed);
        }
        isActivated = value;
    }
    bool getActivated()
    {
        return isActivated;
    }
    virtual void update() override
    {
        if (isActivated)
        {
            int prevIndex = getPrevIndex();
            //std::cout << prevIndex << '\n';
            //std::cout << waypoints[prevIndex].X << " " << waypoints[prevIndex].Y << '\n';
            if ((getPosition() - waypoints[prevIndex]).Length() >= (waypoints[nextPoint] - waypoints[prevIndex]).Length())
            {
                goNextWaypoint();
            }
        }
    }
};
