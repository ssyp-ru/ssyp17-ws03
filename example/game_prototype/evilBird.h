#pragma once
#include "enemy.h"
#include "waypointPath.h"

class EvilBird: public Enemy {
public:
    WaypointPath *path;
    re::Animation movingAnim; // moving animation

    EvilBird(re::Vector2f pos, double speed) : Enemy::Enemy(pos, re::Vector2f(1, 1)) 
    {
        hp = 10;
        attackDelay = 0.5;
        attackDamage = 2;
        path = new WaypointPath(speed, (DrawableGameObject*)this);
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
        re::GameObjectPtr trig;
        if (direction == 1)
        {
            trig = std::make_shared<DamageTrigger>(position + vertexes[3],
             re::Vector2f(1, 1), attackDamage, ptr);
        } else {
            trig = std::make_shared<DamageTrigger>(position - vertexes[3],
             re::Vector2f(1, 1), attackDamage, ptr);
        }
        worldContainer->push_back(trig);
    }
    virtual void update() override
    {
        Enemy::update();
        path->update();
        if ((velocity.X > 0) && (direction != 1)) direction = 1;
        if ((velocity.X < 0) && (direction != -1)) direction = -1;
    }
    //void display(int scale) override
    //{
    //    re::draw_image(position.X * scale, position.Y * scale, movingAnim.getNextFrame());
    //}
};