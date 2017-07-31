#include "deathTrigger.h"
#include "unit.h"

DeathTrigger::DeathTrigger(re::Vector2f pos, re::Vector2f size) : DrawableGameObject::DrawableGameObject(pos)
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
}
void DeathTrigger::onTriggerStay(re::GameObjectPtr obj)
{
    if (std::dynamic_pointer_cast<Unit>(obj) != 0)
        std::dynamic_pointer_cast<Unit>(obj)->destroy();
}
void DeathTrigger::display(int k)
{
   
}