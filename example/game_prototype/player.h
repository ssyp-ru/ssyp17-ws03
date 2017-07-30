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
<<<<<<< HEAD
    int direction;
    re::Vector2f playerSize;
    re::Vector2f respawnCoords; //Respawn coordinates. Temporarily solution (!), because we have no map.
    std::function<void()> deathFunction;
public:
    bool isGrounded = false;
    re::Animation movingAnim; // moving animation
    Player(re::Vector2f pos, re::Vector2f size) : Unit::Unit(pos, size) 
    {
        respawnCoords = pos; 
        playerSize = size;
        hp = 10;
        attackDelay = 0.3;
        attackDamage = 100;
        direction = 1;
    }
    void onCollisionStay(re::GameObjectPtr to, re::Vector2f vec) override
    {
        if (re::Vector2f(0, -1).angleBetween(vec) < 60.0 / 180.0 * 3.14159) 
        { 
            isGrounded = true;
        }
    }
    void attack() override
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
            trig = std::make_shared<DamageTrigger>(position + vertexes[3] + vertexes[1] / 4,
             re::Vector2f(vertexes[3].X * 2, vertexes[1].Y / 2), attackDamage, ptr);
        else
            trig = std::make_shared<DamageTrigger>(position + vertexes[1] / 4,
             re::Vector2f(vertexes[3].X * -2, vertexes[1].Y / 2), attackDamage, ptr);
        worldContainer->push_back(trig);
    }
    void update() override
    {
        if (re::get_key_state(re::Key::D))
        {
            direction = 1;
            setVelocity(re::Vector2f(5 * getMass(), getVelocity().Y));
        }
        if (re::get_key_state(re::Key::A))
        {
            direction = -1;
            setVelocity(re::Vector2f(-5 * getMass(), getVelocity().Y));
        }
        if (re::get_key_state(re::Key::F))
            tryAttack();

        if ((isGrounded) && ((re::get_key_state(re::Key::D)) || (re::get_key_state(re::Key::A))))
            movingAnim.setSpeed(0.3);
        else
            movingAnim.setSpeed(0);

        if (re::get_key_state(re::Key::W))
        {
            if (isGrounded)
            {
                addImpulse(re::Vector2f(0, -15 * getMass()));
                isGrounded = false;
            }
        }

        if ((getVelocity().Y > 0.1) && (isGrounded)) isGrounded = false;

        if (position.Y >= 50){
            dealDamage(10); //fell to death (10 = max hp). TODO: Replace with death trigger
        }

    }
    void display(int scale) override
    {
        re::draw_image_part( position.X * scale, position.Y * scale,
                            (position.X + playerSize.X) * scale, (position.Y + playerSize.Y) * scale,
                            0, 0,
                            1, 1,
                            movingAnim.getNextFrame() );
        //re::draw_image(position.X * scale, position.Y * scale, movingAnim.getNextFrame());
    }
    void respawn(){
        hp = 10;
        setPosition(respawnCoords);
    }
    void registerDeathFunction(std::function<void()> func){
        deathFunction = func;
    }
    void onDeath() override{
        deathFunction();
    }
=======
    std::vector<Ability*> abilities;
    int expToNextLevel[18] = { 100, 100, 150, 200, 300, 450, 500, 600, 1000, 1200, 1300, 1500, 1550, 1600, 1700, 1800, 1900, 2000 };
    int exp, level;
public:
    bool isGrounded = false;
    re::AnimationPtr movingAnim; // moving animation
    Player(re::Vector2f pos);
    void tryCast(int abilityIndex);
    void addAbility(Ability *ab);
    std::vector<Ability*>* getAbilities();
    void addExp(int amount);
    int getLevel();
    void reduceCooldowns();
    void onCollisionStay(re::GameObjectPtr to, re::Vector2f vec);
    void attack();
    void update();
    void display(int scale);
>>>>>>> 21533de7282e4fb29ab43c38ab6ae4f3acd98030
};
