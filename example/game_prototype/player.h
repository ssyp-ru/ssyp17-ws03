#pragma once
#include <RealEngine/graphic.h>
#include <RealEngine/graphic/image.h>
#include <RealEngine/graphic/animation.h>
#include <RealEngine/math.h>
#include "RealEngine/physic_core.h"
#include "unit.h"
#include "ability.h"

class Player : public Unit{
private:
    std::vector<Ability*> abilities;
    int expToNextLevel[18] = { 100, 100, 150, 200, 300, 450, 500, 600, 1000, 1200, 1300, 1500, 1550, 1600, 1700, 1800, 1900, 2000 };
    int exp, level;
    bool isOnIce = false;
public:
    bool isGrounded = false;
    re::AnimationPtr movingAnim; // moving animation
    re::AnimationPtr jumpingAnim;
    re::AnimationPtr slidingAnim;
    re::AnimationPtr attackAnim;
    Player(re::Vector2f pos);
    void tryCast(int abilityIndex);
    void addAbility(Ability *ab);
    std::vector<Ability*>* getAbilities();
    void addExp(int amount);
    int getLevel();
    void onCollisionStay(re::GameObjectPtr to, re::Vector2f vec);
    void attack();
    void update();
    void display(int scale);
};
