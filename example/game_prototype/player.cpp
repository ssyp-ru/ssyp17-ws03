//#include <RealEngine/graphic.h>
//#include <RealEngine/graphic/image.h>
//#include <RealEngine/graphic/animation.h>
#include "RealEngine/physic_core.h"
#include "unit.h"
#include "damageTrigger.h"
#include "icePlatform.h"
#include "player.h"
#include <iostream>
#include "ability_damageBoost.h"

Player::Player(re::Point2f pos) : Unit::Unit(pos, re::Point2f(2, 3)) 
{
    level = 1;
    exp = 0;
    hp = 20;
    maxhp = hp;
    attackDelay = 0.3;
    attackDamage = 2;
    direction = 1;
}
void Player::addAbility(Ability *ab)
{
    abilities.push_back(ab);
    for (uint i = 0; i < (*worldContainer).size(); i++)
    {
        if ((*worldContainer)[i].get() == this)
        {
            ab->sender = (*worldContainer)[i];
            break;
        }
    }
}
void Player::onCollisionStay(re::PhysicObjectPtr to, re::Point2f vec)
{
    if (re::Point2f(0, -1).angleBetween(vec) < 60.0 / 180.0 * 3.14159) 
    { 
        if (!((std::dynamic_pointer_cast<IcePlatform>(to) != 0) && (getVelocity().length() > 2.5)))
        {
            isOnIce = false;
            isGrounded = true;
        } else {
            isOnIce = true;
        }
    }
}
void Player::tryCast(int abilityIndex)
{
    if ((abilityIndex < 0) || ((uint)abilityIndex >= abilities.size())) return;
    if ((abilities[abilityIndex]->getRequiredLevel() <= level) && (abilities[abilityIndex]->canCast()))
    {
        abilities[abilityIndex]->cast();
    }
}
int Player::getLevel() { return level; }
std::vector<Ability*>* Player::getAbilities()
{
    return &abilities;
}
void Player::addExp(int amount)
{
    exp += amount;
    if ((level != 20) && (exp >= expToNextLevel[level - 1]))
    {
        std::cout << "New level!\n";
        exp -= expToNextLevel[level - 1];
        level++;
        if (level == 20) exp = 0;
    }
}
void Player::attack()
{
    re::PhysicObjectPtr ptr;
    for (uint i = 0; i < (*worldContainer).size(); i++)
    {
        if ((*worldContainer)[i].get() == this)
        {
            ptr = (*worldContainer)[i];
            break;
        }
    }
    re::PhysicObjectPtr trig;
    if (direction == 1){
        trig = std::make_shared<DamageTrigger>(position + vertexes[3] + vertexes[1] / 4,
         re::Point2f(vertexes[3].x * 2, vertexes[1].y / 2), attackDamage, ptr);
    }
    else{
        trig = std::make_shared<DamageTrigger>(position + vertexes[1] / 4,
         re::Point2f(vertexes[3].x * -2, vertexes[1].y / 2), attackDamage, ptr);
    }
    attackAnim->getNextFrame();
    worldContainer->push_back(trig);
}
void Player::update()
{
    if (velocity.y > 20) setVelocity(re::Point2f(velocity.x, 20));

    addForce(re::Point2f(0, 60 * getMass()));

    for (uint i = 0; i < abilities.size(); i++)
        abilities[i]->update();

    if ((getVelocity().y > 0.1) && (isGrounded)) isGrounded = false;
    if (re::get_key_state(re::Key::D))
    {
        direction = 1;
        setVelocity(re::Point2f(10 * getMass(), getVelocity().y));
    }
    if (re::get_key_state(re::Key::A))
    {
        direction = -1;
        setVelocity(re::Point2f(-10 * getMass(), getVelocity().y));
    }
    // abilities
    if (re::get_key_state(re::Key::Num1))
        tryCast(0);
    if (re::get_key_state(re::Key::Num2))
        tryCast(1);
    if (re::get_key_state(re::Key::Num3))
        tryCast(2);
    if (re::get_key_state(re::Key::Num4))
        tryCast(3);
    if (re::get_key_state(re::Key::Num5))
        tryCast(4);
    if (re::get_key_state(re::Key::Num6))
        tryCast(5);
    if (re::get_key_state(re::Key::Num7))
        tryCast(6);
    if (re::get_key_state(re::Key::Num8))
        tryCast(7);
    //
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
            addImpulse(re::Point2f(0, -25 * getMass()));
            isGrounded = false;
        }
    }
    Unit::update();
}
void Player::reduceCooldowns()
{
    for (uint i = 0; i < abilities.size(); i++)
        abilities[i]->reduceCooldown();
}
void Player::display(int scale)
{
    if( attackAnim->curPosition != 0 )
    {
        if( direction == 1 )
        {
            re::draw_image_part( position.x * scale, position.y * scale,
                                position.x * scale + 32, position.y * scale +48,
                                0,0, 1,1,
                                attackAnim->getNextFrame() );
        }
        else
        {
            re::draw_image_part( position.x * scale, position.y * scale,
                                position.x * scale + 32, position.y * scale +48,
                                1,0, 0,1,
                                attackAnim->getNextFrame() );
        }
    }
    else if(isGrounded){
        if( direction == 1 )
        {
            re::draw_image_part( position.x * scale, position.y * scale,
                                position.x * scale + 32, position.y * scale +48,
                                0,0, 1,1,
                                movingAnim->getNextFrame() );
        }
        else
        {
            re::draw_image_part( position.x * scale, position.y * scale,
                                position.x * scale + 32, position.y * scale +48,
                                1,0, 0,1,
                                movingAnim->getNextFrame() );
        }
    }
    else if(isOnIce){
        if( direction == 1 )
        {
            re::draw_image_part( position.x * scale, position.y * scale,
                            position.x * scale + 32, position.y * scale +48,
                            0,0, 1,1,
                            slidingAnim->getNextFrame() );
        }
        else
        {
            re::draw_image_part( position.x * scale, position.y * scale,
                            position.x * scale + 32, position.y * scale +48,
                            1,0, 0,1,
                            slidingAnim->getNextFrame() );
        }
        
    } else{
        if( direction == 1 )
        {
            re::draw_image_part( position.x * scale, position.y * scale,
                            position.x * scale + 32, position.y * scale +48,
                            0,0, 1,1,
                            jumpingAnim->getNextFrame() );
        }
        else
        {
            re::draw_image_part( position.x * scale, position.y * scale,
                            position.x * scale + 32, position.y * scale +48,
                            1,0, 0,1,
                            jumpingAnim->getNextFrame() );
        }
    }
    //re::draw_image(position.x * scale, position.y * scale, movingAnim->getNextFrame());
}

