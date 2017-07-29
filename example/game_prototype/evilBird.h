#pragma once
#include "enemy.h"

class EvilBird: public Enemy {
public:
    re::Animation movingAnim; // moving animation
    EvilBird(re::Vector2f pos) : Enemy::Enemy(pos, re::Vector2f(1, 1)) 
    {
        hp = 10;
        attackDelay = 0.5;
        attackDamage = 2;
    }
    virtual void onCollisionStay(re::GameObjectPtr to, re::Vector2f vec) override
    {
        if (std::dynamic_pointer_cast<Player>(to) != 0)
        {
            tryAttack();
        }
    }
    virtual void attack() override
    {
        re::GameObjectPtr ptr;
        for (uint i = 0; i < (*worldContainer).size(); i++)
        {
            if ((*worldContainer)[i].get() == this)
            {
                ptr = (*worldContainer)[i];
                break;
            }
        }
        re::GameObjectPtr trig = std::make_shared<DamageTrigger>(position - re::Vector2f(0.75, 0.75),
             re::Vector2f(2.5, 2.5), attackDamage, ptr);
        worldContainer->push_back(trig);
    }
    virtual void update() override
    {
        
    }
    //void display(int scale) override
    //{
    //    re::draw_image(position.X * scale, position.Y * scale, movingAnim.getNextFrame());
    //}
};