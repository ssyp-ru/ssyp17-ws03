#pragma once
#include <RealEngine/physic_core.h>

class Unit : public re::GameObject {
    protected:
        Unit() {

        }
        double hp;
        double attack_damage;
        re::Animation move_ani; // moving animation
    public:
        virtual void onDeath() {
            // Play sad theme
        }
        virtual void onCollision() {}
        virtual void damage(double dmg) {
            hp -= dmg;
            if(hp <= 0.0) {
                onDeath();
            }
        }
};