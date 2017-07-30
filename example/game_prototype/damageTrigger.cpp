#include "damageTrigger.h"
#include <RealEngine/time.h>
#include "unit.h"
#include "enemy.h"
#include "player.h"

void DamageTrigger::update()
{
    lifeTime -= re::Time::delta_time;
    if (lifeTime <= 0) destroy();
}
void DamageTrigger::onTriggerStay(re::GameObjectPtr obj)
{
    if ((std::dynamic_pointer_cast<Enemy>(obj) != 0) && (std::dynamic_pointer_cast<Player>(sender) != 0))
    {
        (std::dynamic_pointer_cast<Unit>(obj))->dealDamage(damage, sender);
        destroy();
    }
    if ((std::dynamic_pointer_cast<Player>(obj) != 0) && (std::dynamic_pointer_cast<Enemy>(sender) != 0))
    {
        (std::dynamic_pointer_cast<Unit>(obj))->dealDamage(damage, sender);
        destroy();
    }
}
DamageTrigger::DamageTrigger(re::Vector2f pos, re::Vector2f size, double damage, re::GameObjectPtr sender) : DrawableGameObject::DrawableGameObject(pos) 
{
    setRigidbodySimulated(false);
    setIsTrigger(true);
    addPoint(re::Vector2f(0, 0));
    addPoint(re::Vector2f(0, size.Y));
    addPoint(re::Vector2f(size.X, size.Y));
    addPoint(re::Vector2f(size.X, 0));
    addEdge(0, 1);
    addEdge(1, 2);
    addEdge(2, 3);
    addEdge(3, 0);
    lifeTime = 0.05;
    this->damage = damage;
    this->sender = sender;
}
void DamageTrigger::display(int k)
{
    
}