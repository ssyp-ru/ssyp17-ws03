#include "deathTrigger.h"
#include "unit.h"

DeathTrigger::DeathTrigger(re::Point2f pos, re::Point2f size) : DrawableGameObject::DrawableGameObject(pos)
{
    setRigidbodySimulated(false);
    setIsTrigger(true);
    addPoint(re::Point2f(0, 0));
    addPoint(re::Point2f(0, size.y));
    addPoint(re::Point2f(size.x, size.y));
    addPoint(re::Point2f(size.x, 0));
    addEdge(0, 1);
    addEdge(1, 2);
    addEdge(2, 3);
    addEdge(3, 0);
}
void DeathTrigger::onTriggerStay(re::PhysicObjectPtr obj)
{
    if (std::dynamic_pointer_cast<Unit>(obj) != 0)
        std::dynamic_pointer_cast<Unit>(obj)->destroy();
}
void DeathTrigger::display(int k)
{
   
}