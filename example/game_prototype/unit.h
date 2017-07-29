#pragma once
#include <RealEngine/physic_core.h>
#include <RealEngine/stop_watch.h>
#include "drawableGameObject.h"

class Unit : public DrawableGameObject {
    protected:
        Unit(re::Vector2f pos, re::Vector2f size) : DrawableGameObject::DrawableGameObject(pos) {
            addPoint(re::Vector2f(0, 0));
            addPoint(re::Vector2f(0, size.Y));
            addPoint(re::Vector2f(size.X, size.Y));
            addPoint(re::Vector2f(size.X, 0));
            addEdge(0, 1);
            addEdge(1, 2);
            addEdge(2, 3);
            addEdge(3, 0);
        }
        double hp;
        double attackDamage;
        double attackDelay;
        re::StopWatch lastAttackTime;
        re::Animation move_ani; // moving animation
        virtual void attack()
        {
            
        }
        virtual void onCollisionStay(re::GameObjectPtr obj, re::Vector2f vec) { }
        virtual void update() { }
    public:
        void tryAttack()
        {
            if (lastAttackTime.stop_watch() / 1e9 > attackDelay)
            {
                lastAttackTime = re::StopWatch();
                attack();
            }
        }
        virtual void dealDamage(double dmg) {
            hp -= dmg;
            if(hp <= 0.0) destroy();
        }
};