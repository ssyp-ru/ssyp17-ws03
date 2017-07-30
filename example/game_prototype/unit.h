#pragma once
#include <RealEngine/physic_core.h>
#include <RealEngine/stop_watch.h>
#include "drawableGameObject.h"
#include <functional>

class Unit : public DrawableGameObject {
protected:
    int direction;
    Unit(re::Vector2f pos, re::Vector2f size) : DrawableGameObject::DrawableGameObject(pos) {
        addPoint(re::Vector2f(0, 0));
        addPoint(re::Vector2f(0, size.Y));
        addPoint(re::Vector2f(size.X, size.Y));
        addPoint(re::Vector2f(size.X, 0));
        addEdge(0, 1);
        addEdge(1, 2);
        addEdge(2, 3);
        addEdge(3, 0);
        isInvincible = false;
    }
    double hp, maxhp;
    double attackDamage;
    double attackDelay;
    bool isInvincible;
    re::StopWatch lastAttackTime;
    re::Animation move_ani; // moving animation
    virtual void attack()
    {
        
    }
    virtual void onCollisionStay(re::GameObjectPtr obj, re::Vector2f vec) { }
    virtual void update() { DrawableGameObject::update(); if (hp > maxhp) hp = maxhp; }
public:
    virtual void onDeath(re::GameObjectPtr lastAttacker)
    {
        
    }
    void tryAttack()
    {
        if (lastAttackTime.stop_watch() / 1e9 > attackDelay)
        {
            lastAttackTime = re::StopWatch();
            attack();
        }
    }
    double getHp() { return hp; }
    double getMaxHp() { return maxhp; }
    void setMaxHp(double value) { maxhp = value; }
    double getDamage() { return attackDamage; }
    void setDamage(double value) {  attackDamage = value; }
    void setIsInvincible(bool value) { isInvincible = value; }
    bool getIsInvincible() { return isInvincible; }
    virtual void dealDamage(double dmg, re::GameObjectPtr sender) {
        hp -= dmg;
        if ((isInvincible) && (hp < 1)) hp = 1;
        if(hp <= 0.0) 
        {
            onDeath(sender);
            destroy();
        }
    }
    virtual void heal(double amount) { hp += amount; }
};