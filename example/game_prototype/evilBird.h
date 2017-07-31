#pragma once
#include "enemy.h"
#include "waypointPath.h"

class EvilBird: public Enemy {
public:
    WaypointPath *path;
    re::AnimationPtr movingAnim; // moving animation

    EvilBird(re::Vector2f pos, double speed) : Enemy::Enemy(pos, re::Vector2f(3.5, 2.5)) 
    {
        hp = 10;
        maxhp = hp;
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
    virtual void onDeath(re::GameObjectPtr sender) override
    {
        if (std::dynamic_pointer_cast<Player>(sender) != 0)
            std::dynamic_pointer_cast<Player>(sender)->addExp(100);            
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
            trig = std::make_shared<DamageTrigger>(position - re::Vector2f(1, 0),
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
    void display(int scale) override
    {
        if( direction == 1 )
        {
            re::draw_image_part( position.X * scale, position.Y * scale,
                            position.X * scale + 60, position.Y * scale +40,
                            0,0, 1,1,
                            movingAnim->getNextFrame() );
        }
        else
        {
            re::draw_image_part( position.X * scale, position.Y * scale,
                            position.X * scale + 60, position.Y * scale +40,
                            1,0, 0,1,
                            movingAnim->getNextFrame() );
        }
        //re::draw_image(position.X * scale, position.Y * scale, movingAnim.getNextFrame());
    }
};