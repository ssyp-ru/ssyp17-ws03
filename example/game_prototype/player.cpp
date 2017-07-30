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

Player::Player(re::Vector2f pos) : Unit::Unit(pos, re::Vector2f(2, 3)) 
{
    level = 1;
    exp = 0;
    hp = 100;
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
void Player::onCollisionStay(re::GameObjectPtr to, re::Vector2f vec)
{
    if (re::Vector2f(0, -1).angleBetween(vec) < 60.0 / 180.0 * 3.14159) 
    { 
        if (!((std::dynamic_pointer_cast<IcePlatform>(to) != 0) && (getVelocity().Length() > 0.5)))
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
    if (direction == 1){
        trig = std::make_shared<DamageTrigger>(position + vertexes[3] + vertexes[1] / 4,
         re::Vector2f(vertexes[3].X * 2, vertexes[1].Y / 2), attackDamage, ptr);
    }
    else{
        trig = std::make_shared<DamageTrigger>(position + vertexes[1] / 4,
         re::Vector2f(vertexes[3].X * -2, vertexes[1].Y / 2), attackDamage, ptr);
    }
    attackAnim->getNextFrame();
    worldContainer->push_back(trig);
}
void Player::update()
{
    addForce(re::Vector2f(0, 60 * getMass()));

    for (uint i = 0; i < abilities.size(); i++)
        abilities[i]->update();

    if ((getVelocity().Y > 0.1) && (isGrounded)) isGrounded = false;
    if (re::get_key_state(re::Key::D))
    {
        direction = 1;
        setVelocity(re::Vector2f(10 * getMass(), getVelocity().Y));
    }
    if (re::get_key_state(re::Key::A))
    {
        direction = -1;
        setVelocity(re::Vector2f(-10 * getMass(), getVelocity().Y));
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
            addImpulse(re::Vector2f(0, -30 * getMass()));
            isGrounded = false;
        }
    }
    Unit::update();
}
void Player::display(int scale)
{
    if( attackAnim->curPosition != 0 )
    {
        if( direction == 1 )
        {
            re::draw_image_part( position.X * scale, position.Y * scale,
                                position.X * scale + 32, position.Y * scale +48,
                                0,0, 1,1,
                                attackAnim->getNextFrame() );
        }
        else
        {
            re::draw_image_part( position.X * scale, position.Y * scale,
                                position.X * scale + 32, position.Y * scale +48,
                                1,0, 0,1,
                                attackAnim->getNextFrame() );
        }
    }
    else if(isGrounded){
        if( direction == 1 )
        {
            re::draw_image_part( position.X * scale, position.Y * scale,
                                position.X * scale + 32, position.Y * scale +48,
                                0,0, 1,1,
                                movingAnim->getNextFrame() );
        }
        else
        {
            re::draw_image_part( position.X * scale, position.Y * scale,
                                position.X * scale + 32, position.Y * scale +48,
                                1,0, 0,1,
                                movingAnim->getNextFrame() );
        }
    }
    else if(isOnIce){
        if( direction == 1 )
        {
            re::draw_image_part( position.X * scale, position.Y * scale,
                            position.X * scale + 32, position.Y * scale +48,
                            0,0, 1,1,
                            slidingAnim->getNextFrame() );
        }
        else
        {
            re::draw_image_part( position.X * scale, position.Y * scale,
                            position.X * scale + 32, position.Y * scale +48,
                            1,0, 0,1,
                            slidingAnim->getNextFrame() );
        }
        
    } else{
        if( direction == 1 )
        {
            re::draw_image_part( position.X * scale, position.Y * scale,
                            position.X * scale + 32, position.Y * scale +48,
                            0,0, 1,1,
                            jumpingAnim->getNextFrame() );
        }
        else
        {
            re::draw_image_part( position.X * scale, position.Y * scale,
                            position.X * scale + 32, position.Y * scale +48,
                            1,0, 0,1,
                            jumpingAnim->getNextFrame() );
        }
    }
    //re::draw_image(position.X * scale, position.Y * scale, movingAnim->getNextFrame());
}

