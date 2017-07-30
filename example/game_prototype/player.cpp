#pragma once
//#include <RealEngine/graphic.h>
//#include <RealEngine/graphic/image.h>
//#include <RealEngine/graphic/animation.h>
#include "RealEngine/physic_core.h"
#include "unit.h"
#include "damageTrigger.h"
#include "icePlatform.h"
#include "player.h"

Player::Player(re::Vector2f pos) : Unit::Unit(pos, re::Vector2f(1, 1.5)) 
{
    hp = 100;
    attackDelay = 0.3;
    attackDamage = 2;
    direction = 1;
}
void Player::onCollisionStay(re::GameObjectPtr to, re::Vector2f vec)
{
    if (re::Vector2f(0, -1).angleBetween(vec) < 60.0 / 180.0 * 3.14159) 
    { 
        if (!((std::dynamic_pointer_cast<IcePlatform>(to) != 0) && (getVelocity().Length() > 0.5)))
        {
            isGrounded = true;
        }
    }
}
void Player::attack()
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
void Player::update()
{
    addForce(re::Vector2f(0, 20 * getMass()));
    if ((getVelocity().Y > 0.1) && (isGrounded)) isGrounded = false;
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
        movingAnim->setSpeed(0.5);
    else
        movingAnim->setSpeed(0);
    if (re::get_key_state(re::Key::W))
    {
        if (isGrounded)
        {
            addImpulse(re::Vector2f(0, -45 * getMass()));
            isGrounded = false;
        }
    }
    Unit::update();
}
void Player::display(int scale)
{
    re::draw_image(position.X * scale, position.Y * scale, movingAnim->getNextFrame());
}

