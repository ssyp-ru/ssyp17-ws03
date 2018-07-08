#include "RealEngine/physic_core.h"
#include <cmath>
#include <iostream>
#include "RealEngine/logger.h"
#include <vector>
#include <chrono>
#include <RealEngine/time.h>
#include <RealEngine/benchmark.h>

namespace re {

//Vector2f::Vector2f() 
//{
//	x = 0;
//	y = 0;
//}
//Vector2f::Vector2f(double x, double y)
//{
//	this->x = x;
//	this->y = y;
//}
//double Vector2f::length()
//{
//	if ((x == 0) && (y == 0)) return 0;
//	return sqrt(x*x+y*y);
//}
//void Vector2f::Normalize()
//{
//	double len = length();
//	x /= len;
//	y /= len;
//}
//Vector2f Vector2f::Normalized()
//{
//	return Vector2f(x / length(), y / length());
//}
//Vector2f Vector2f::operator+(Vector2f vec)
//{
//	return Vector2f(x + vec.x, y + vec.y);
//}
//Vector2f Vector2f::operator-(Vector2f vec)
//{
//	return Vector2f(x - vec.x, y - vec.y);
//}
//Vector2f Vector2f::operator*(double val)
//{
//	return Vector2f(x * val, y * val);
//}
//Vector2f Vector2f::operator/(double val)
//{
//	return Vector2f(x / val, y / val);
//}
//void Vector2f::operator*=(double val)
//{
//	x *= val;
//	y *= val;
//}
//void Vector2f::operator/=(double val)
//{
//	x /= val;
//	y /= val;
//}
//void Vector2f::operator+=(Vector2f vec)
//{
//	x += vec.x;
//	y += vec.y;
//}
//void Vector2f::operator-=(Vector2f vec)
//{
//	x -= vec.x;
//	y -= vec.y;
//}
//bool Vector2f::operator!=(Vector2f vec)
//{
//	return ((x != vec.x) || (y != vec.y));
//}
//bool Vector2f::operator==(Vector2f vec)
//{
//	return ((x == vec.x) && (y == vec.y));
//}
//double Vector2f::operator*(Vector2f vec)
//{
//	return x * vec.x + y * vec.y;
//}
//Vector2f Vector2f::projectOnVector(Vector2f vec)
//{
//	// proj.x = ( dp / (b.x*b.x + b.y*b.y) ) * b.x;
//	// proj.y = ( dp / (b.x*b.x + b.y*b.y) ) * b.y;
//    if (vec.x == 0) return Vector2f(0, y);
//    if (vec.y == 0) return Vector2f(x, 0);
//	double scalarProduct = *this * vec;
//	return Vector2f((scalarProduct / (vec.x * vec.x + vec.y * vec.y)) * vec.x, 
//		            (scalarProduct / (vec.x * vec.x + vec.y * vec.y)) * vec.y);
//}
//Vector2f Vector2f::getLeftNormal()
//{
//	return Vector2f(y, -x);
//}
//Vector2f Vector2f::getRightNormal()
//{
//	return Vector2f(-y, x);
//}
//double Vector2f::operator/(Vector2f vec)
//{
//	if (vec.x != 0)
//		return x / vec.x;
//	else
//		if (vec.y != 0)
//			return y / vec.y;
//		else
//			return 0;
//}
//Vector2f Vector2f::reflectFrom(Vector2f mirror)
//{
//	return *this + (this->projectOnVector(mirror) - *this) * 2;
//}
//bool Vector2f::isCollinearTo(Vector2f vec)
//{
//    if (((vec.x == 0) && (x == 0)) || ((vec.y == 0) && (y == 0))) return true;
//    return (vec.x / vec.y == x / y);
//}
//void Vector2f::rotate(double radians)
//{
//	double newx, newy;
//	newx = x * cos(radians) + y * sin(radians);
//	newy = y * cos(radians) - x * sin(radians);
//	x = newx;
//	y = newy;
//}
//Vector2f Vector2f::rotated(double radians)
//{
//	return Vector2f(x * cos(radians) + y * sin(radians), y * cos(radians) - x * sin(radians));
//}
//double Vector2f::angleBetween(Vector2f vec){
//	double a = length(), b = vec.length(), c = (*this - vec).length();
//	if ((a == 0) || (b == 0) || (c == 0)) return 0;
//	return acos((a*a + b*b - c*c) / (2*a*b));
//}


Edge::Edge(re::Point2f *P1, re::Point2f *P2)
{
	this->P1 = P1;
	this->P2 = P2;
}


bool Metadata::hasKey(std::string name)
{
	if (int_values.find(name) != int_values.end()) return true;
	if (double_values.find(name) != double_values.end()) return true;
	if (string_values.find(name) != string_values.end()) return true;
	if (bool_values.find(name) != bool_values.end()) return true;
	return false;
}

void Metadata::setInt(std::string name, int value)
{
	int_values[name] = value;
}
void Metadata::setDouble(std::string name, double value)
{
	double_values[name] = value;
}
void Metadata::setString(std::string name, std::string value)
{
	string_values[name] = value;
}
void Metadata::setBool(std::string name, bool value)
{
	bool_values[name] = value;
}

int Metadata::getInt(std::string name)
{
	if (int_values.find(name) == int_values.end()) 
	{
		re::Log::msg("Getting not-existing value from metadata.", re::Log::LEVEL::DEBUG);
		return 0;
	}
	return int_values[name];
}
double Metadata::getDouble(std::string name)
{
	if (double_values.find(name) == double_values.end())
	{
		re::Log::msg("Getting not-existing value from metadata.", re::Log::LEVEL::DEBUG);
		return 0.0;
	}
	return double_values[name];
}
std::string Metadata::getString(std::string name)
{
	if (string_values.find(name) == string_values.end())
	{
		re::Log::msg("Getting not-existing value from metadata.", re::Log::LEVEL::DEBUG);
		return "";
	}
	return string_values[name];
}
bool Metadata::getBool(std::string name)
{
	if (bool_values.find(name) == bool_values.end())
	{
		re::Log::msg("Getting not-existing value from metadata.", re::Log::LEVEL::DEBUG);
		return false;
	}
	return bool_values[name];
}


PhysicObject::PhysicObject()
{
	PhysicObject(re::Point2f(0, 0));
}
PhysicObject::PhysicObject(re::Point2f pos)
{
	destroyNextFrame = false;
	position = pos;
	circleRadius = 0;
    mass = 1;
	bounciness = 0.5;
	friction = 0.5;
    newPosition = re::Point2f();
    isRigidbodySimulated = true;
	isTrigger = false;
	velocity = re::Point2f();
	acceleration = re::Point2f();
	massCenter = re::Point2f();
	rotation = 0;
	rotationSpeed = 0;
}
void PhysicObject::addForce(re::Point2f force)
{
	acceleration += force / mass;
}
void PhysicObject::addImpulse(re::Point2f impulse)
{
	velocity += impulse / mass;
}
re::Point2f* PhysicObject::addPoint(re::Point2f point)
{
	vertexes.push_back(point);
	if (point.length() > circleRadius) circleRadius = point.length();
	countMassCenter();
	return &vertexes[vertexes.size() - 1];
}
void PhysicObject::addEdge(int a, int b)
{
    Edge *edge = new Edge(&vertexes[a], &vertexes[b]);
	edges.push_back(*edge);
}
std::vector<re::Point2f>* PhysicObject::getVertexes()
{
	return &vertexes;
}
std::vector<Edge>* PhysicObject::getEdges()
{
	return &edges;
}
void PhysicObject::rotate(double rotation)
{
	for (uint i = 0; i < vertexes.size(); i++)
	{
		vertexes[i].rotate(rotation);
	}
	this->rotation += rotation;
}
double PhysicObject::getMass() { return mass; }
void PhysicObject::setMass(double value) { value > 0? mass = value : mass = 1; }
double PhysicObject::getBounciness() { return bounciness; }
void PhysicObject::setBounciness(double value) { value >= 0? bounciness = value : bounciness = 0.5; }
double PhysicObject::getFriction() { return friction; }
void PhysicObject::setFriction(double value) { value <= 1? friction = value : friction = 0.5; }
bool PhysicObject::getRigidbodySimulated() {return isRigidbodySimulated; }
void PhysicObject::setRigidbodySimulated(bool value) { isRigidbodySimulated = value; }
bool PhysicObject::getIsTrigger() { return isTrigger; }
void PhysicObject::setIsTrigger(bool value) { isTrigger = value; }
double PhysicObject::getRotationSpeed() { return rotationSpeed; }
void PhysicObject::setRotationSpeed(double value) { rotationSpeed = value; }
re::Point2f PhysicObject::getPosition() { return position; }
void PhysicObject::setPosition(re::Point2f pos) { position = pos; }
re::Point2f PhysicObject::getVelocity()
{
    return velocity;
}
void PhysicObject::setVelocity(re::Point2f vec)
{
	if (canChangeSpeed)
    	velocity = vec;
}
bool PhysicObject::getCanChangeSpeed() { return canChangeSpeed; }
void PhysicObject::setCanChangeSpeed(bool value) { canChangeSpeed = value; }
void PhysicObject::countMassCenter() 
{
    double x = 0, y = 0;
    for (uint i = 0; i < vertexes.size(); i++)
    {
        x += position.x + vertexes[i].x;
        y += position.y + vertexes[i].y;
    }
    massCenter = re::Point2f(x/vertexes.size(), y/vertexes.size()) - position;
}
re::Point2f PhysicObject::getMassCenter()
{
    return position+massCenter;
}
bool PhysicObject::needTestWith(PhysicObject &gm)
{
    if (&gm == this)
        return false;

    if (not_check_mask && (gm.physic_type & not_check_mask))
        return false;

    double length = (position - gm.position).length();
    return length <= (circleRadius + gm.circleRadius);
}
re::Point2f PhysicObject::removeCollisionWith(PhysicObject &gm)
{
	// ТУДУ: запилить таки проверку коллизий
	re::Point2f outOfCollisionVector = re::Point2f();
	re::Point2f middleObjectsVec = (gm.getMassCenter() - getMassCenter()) * 2;
	//re::Point2f curLineForProjection = ((getMassCenter() + *edges[i].P2) - (getMassCenter() + *edges[i].P1)).getRightNormal();
    std::vector<re::Point2f> linesForProjection;
    for (uint i = 0; i < edges.size(); i++)
    {
        bool needToAdd = true;
        re::Point2f curLine = ((*edges[i].P1 + position) - (*edges[i].P2 + position)).getLeftNormal();
        for (uint j = 0; j < linesForProjection.size(); j++)
        {
            if (curLine.isCollinearTo(linesForProjection[j]))
            {
                needToAdd = false;
                break;
            }
        }
        if (needToAdd)
            linesForProjection.push_back(curLine);
    }
    //std::cout << "hey\n";
    for (uint i = 0; i < gm.edges.size(); i++)
    {
        bool needToAdd = true;
        re::Point2f curLine = ((*gm.edges[i].P1 + gm.position) - (*gm.edges[i].P2 + gm.position)).getLeftNormal();
        for (uint j = 0; j < linesForProjection.size(); j++)
        {
            if (curLine.isCollinearTo(linesForProjection[j]))
            {
                needToAdd = false;
                break;
            }
        }
        if (needToAdd)
            linesForProjection.push_back(curLine);
    }
    //std::cout << "hey\n";
	for (uint i = 0; i < linesForProjection.size(); i++)
	{
		re::Point2f selfProjection;
		re::Point2f otherProjection;
		re::Point2f crossProjection;
        //std::cout << "(" << linesForProjection[i].x << ", " << linesForProjection[i].y << ")\n";
		std::vector<re::Point2f*> crossProjectionPoints;
		//#pragma region getting self-projection
		//
			std::vector<re::Point2f> self_projectedPoints;
			//re::graphic::draw_line(linesForProjection[i].x * 250 + 200, linesForProjection[i].y * 250 + 200,
			//					   linesForProjection[i].x * -250 + 200, linesForProjection[i].y * -250 + 200,
			//					   re::BLACK);
			for (uint j = 0; j < vertexes.size(); j++)
			{
				self_projectedPoints.push_back((position + vertexes[j]).projectOnVector(linesForProjection[i]));
				//re::graphic::draw_line((position.x + vertexes[j].x) * 25, (position.y + vertexes[j].y) * 25,
				//					   self_projectedPoints[j].x * 25 + 200, self_projectedPoints[j].y * 25 + 200, re::BLACK);
				//re::graphic::draw_rectangle(self_projectedPoints[j].x * 25 + 190, self_projectedPoints[j].y * 25 + 190, 20, 20, re::BLACK);
				//std::cout << '>' << j << ' ' << self_projectedPoints[j].x << ' ' << self_projectedPoints[j].y << std::endl;
			}
			
            re::Point2f *self_smallestPoint = &self_projectedPoints[0], *self_biggestPoint = &self_projectedPoints[0];
			for (uint j = 1; j < self_projectedPoints.size(); j++)
			{
                if (linesForProjection[i].x == 0)
                {
                    if (self_projectedPoints[j].y < self_smallestPoint->y) self_smallestPoint = &self_projectedPoints[j];
                    if (self_projectedPoints[j].y > self_biggestPoint->y) self_biggestPoint = &self_projectedPoints[j];
                } else {
                    if (self_projectedPoints[j].x < self_smallestPoint->x) self_smallestPoint = &self_projectedPoints[j];
                    if (self_projectedPoints[j].x > self_biggestPoint->x) self_biggestPoint = &self_projectedPoints[j];
                }
			}
            selfProjection = *self_biggestPoint - *self_smallestPoint;
			crossProjectionPoints.push_back(self_smallestPoint);
			crossProjectionPoints.push_back(self_biggestPoint);
			if (selfProjection.ratio(middleObjectsVec.projectOnVector(linesForProjection[i])) < 0) selfProjection *= -1;
		//
		//#pragma endregion
		//#pragma region getting other-projection
		//
			std::vector<re::Point2f> other_projectedPoints;
			for (uint j = 0; j < gm.vertexes.size(); j++)
			{
				other_projectedPoints.push_back((gm.position + gm.vertexes[j]).projectOnVector(linesForProjection[i]));
				//re::graphic::draw_line((gm.position.x + gm.vertexes[j].x) * 25, (gm.position.y + gm.vertexes[j].y) * 25,
				//					   other_projectedPoints[j].x * 25 + 200, other_projectedPoints[j].y * 25 + 200, re::BLACK);
				//re::graphic::draw_rectangle(other_projectedPoints[j].x * 25 + 190, other_projectedPoints[j].y * 25 + 190, 20, 20, re::BLACK);
				//std::cout << '>' << j << ' ' << other_projectedPoints[j].x << ' ' << other_projectedPoints[j].y << std::endl;
			}
			
            re::Point2f *other_smallestPoint = &other_projectedPoints[0], *other_biggestPoint = &other_projectedPoints[0];
			for (uint j = 1; j < other_projectedPoints.size(); j++)
			{
                if (linesForProjection[i].x == 0)
                {
                    if (other_projectedPoints[j].y < other_smallestPoint->y) other_smallestPoint = &other_projectedPoints[j];
                    if (other_projectedPoints[j].y > other_biggestPoint->y) other_biggestPoint = &other_projectedPoints[j];
                } else {
                    if (other_projectedPoints[j].x < other_smallestPoint->x) other_smallestPoint = &other_projectedPoints[j];
                    if (other_projectedPoints[j].x > other_biggestPoint->x) other_biggestPoint = &other_projectedPoints[j];
                }
			}
            otherProjection = *other_biggestPoint - *other_smallestPoint;
			crossProjectionPoints.push_back(other_smallestPoint);
			crossProjectionPoints.push_back(other_biggestPoint);
			if (otherProjection.ratio(middleObjectsVec.projectOnVector(linesForProjection[i])) < 0) otherProjection *= -1;
		//
		//#pragma endregion
		//#pragma region getting cross-projection
		if ((*crossProjectionPoints[0] - *crossProjectionPoints[3]).length() > 
		    (*crossProjectionPoints[2] - *crossProjectionPoints[1]).length())
			crossProjection = *crossProjectionPoints[0] - *crossProjectionPoints[3];
		else
			crossProjection = *crossProjectionPoints[2] - *crossProjectionPoints[1];
		if (crossProjection.ratio(middleObjectsVec.projectOnVector(linesForProjection[i])) < 0) crossProjection *= -1;
		//#pragma endregion
        //std::cout << "cross " << crossProjection.x << " " << crossProjection.y << "\n";
        //std::cout << "self " << selfProjection.x << " " << selfProjection.y << '\n';
        //std::cout << "other " << otherProjection.x << " " << otherProjection.y << '\n';
		if (crossProjection.length() - selfProjection.length() - otherProjection.length() > 0)
        {
            //std::cout << "(" << middleObjectsVec.x << ", " << middleObjectsVec.y << ")\n";
            //std::cout << middleObjectsVec.length() << '\n';
            //std::cout << (middleObjectsVec.projectOnVector(curLineForProjection)).length() << "\n";
            //std::cout << "(" << otherProjection.x << ", " << otherProjection.y << ")\n";
			return re::Point2f(0, 0);
        }
		if ((outOfCollisionVector.length() == 0) || ((crossProjection - selfProjection - otherProjection).length() < outOfCollisionVector.length()))
		{
			outOfCollisionVector = (crossProjection - selfProjection - otherProjection) / 2;
		}
		//
		//for (auto p : crossProjectionPoints)
		//{
		//	re::graphic::draw_rectangle(p->x * 25 + 190, p->y * 25 + 190, 20, 20, re::RED);
		//}
		//re::Point2f a = *crossProjectionPoints[0], b = *crossProjectionPoints[1], c = *crossProjectionPoints[2], d = *crossProjectionPoints[3];
		//std::cout << a.isCollinearTo(b - a) << a.isCollinearTo(c - a) << a.isCollinearTo(d - a) << '\n';
		//std::cout << "/////\n" << a.x << " " << a.y << '\n' << b.x << " " << b.y << '\n' << c.x << " " << c.y << '\n' << d.x << " " << d.y << '\n';
		//if (((c - a) / (b - a) < 0) || ((c - a) / (b - a) > 1) && 
		//    ((d - a) / (b - a) < 0) || ((d - a) / (b - a) > 1) &&
		//	((a - c) / (d - c) < 0) || ((a - c) / (d - c) > 1) && 
		//    ((b - c) / (d - c) < 0) || ((b - c) / (d - c) > 1))
		//	return re::Point2f(0, 0);
		
		//outOfCollisionVector = re::Point2f(10, 0);
	}
	//std::cout << "(" << outOfCollisionVector.x << ", " << outOfCollisionVector.y << ")\n";
	return outOfCollisionVector;
}
void PhysicObject::updatePos()
{
	double deltaTime = re::Time::delta_time;
	if (isRigidbodySimulated) 
	{
	    velocity += acceleration * deltaTime;
	    acceleration.x = 0;
	    acceleration.y = 0;
	}
	position += velocity * deltaTime;
    newPosition = re::Point2f(0, 0);
	rotate(rotationSpeed * deltaTime);
    if (!isRigidbodySimulated) 
	{
		if (acceleration.length() != 0)
			acceleration = re::Point2f(0, 0);
	}
}
bool PhysicObject::isPointInside(Point2f point)
{
	return (point - getPosition()).length() <= circleRadius;
}
void PhysicObject::destroy()
{
	destroyNextFrame = true;
}


void PhysicWorld::updatePhysics()
{
    //std::cout << "WTF\n";
	for (uint i = 0; i < world.size(); i++)
		world[i]->updatePos();
	for (uint i = 0; i < world.size(); i++)
	{
		for (uint j = 0; j < world.size(); j++)
		{
            if (world[i] != world[j])
			    if (world[i]->needTestWith(*(world[j]))) // is objects near to each other
			    {
			    	re::Point2f outOfCollisionVector = world[i]->removeCollisionWith(*(world[j]));
			    	if (outOfCollisionVector.length() > 1e-5)
			    	{
			    		//std::cout << "Collision detected.\n";
						// тута вызывать событие коллизии
						// вызовется даже если обрабатываемый объект не динамичен
						if (!world[i]->isTrigger)
						{
							if (!world[j]->isTrigger && ((world[j]->isRigidbodySimulated) || (world[i]->isRigidbodySimulated)))
								world[i]->onCollisionStay(world[j], outOfCollisionVector);
						}
						else
							if (world[j]->isRigidbodySimulated)
								world[i]->onTriggerStay(world[j]);
        				if (world[i]->isRigidbodySimulated && !world[j]->isTrigger)
						{
			    			re::Point2f reflected = world[i]->velocity.reflectFrom(outOfCollisionVector.getLeftNormal());
							re::Point2f frictionProject;
							if (world[i]->friction == -1)
                        		frictionProject = reflected.projectOnVector(outOfCollisionVector.getLeftNormal()) * (1 - world[j]->friction);
							else
								frictionProject = reflected.projectOnVector(outOfCollisionVector.getLeftNormal()) * (1 - (world[i]->friction + world[j]->friction) / 2);
                        	re::Point2f bouncinessProject = reflected.projectOnVector(outOfCollisionVector) * (world[i]->bounciness + world[j]->bounciness) / 2;
							re::Point2f impulseVector = re::Point2f(0, 0);
							if (!((world[i]->friction == -1) && (world[j]->velocity.length() != 0)))
                        		impulseVector = frictionProject + bouncinessProject - world[i]->velocity;
							else
								impulseVector = (world[j]->velocity - world[i]->velocity) + bouncinessProject;
                        	world[i]->newPosition += outOfCollisionVector;
							if ((world[j]->isRigidbodySimulated) && (!world[j]->isTrigger))
							{
                        		world[i]->velocity += impulseVector * (world[j]->mass / (world[i]->mass + world[j]->mass));
                        		world[j]->velocity -= impulseVector * (world[i]->mass / (world[i]->mass + world[j]->mass));
							} else {
								world[i]->velocity += impulseVector;
							}
						}
			    	}
			    }
		}
	}
    for (uint i = 0; i < world.size(); i++)
    {
        //if ((world[i]->newPosition.x != 0) && (world[i]->newPosition.y != 0))
            world[i]->position += world[i]->newPosition;
			world[i]->newPosition = re::Point2f(0, 0);
    }
}
void PhysicWorld::update()
{
    while(isGame)
    {
		//std::cout << ticksAlive << ")\n";
        updateTick();
        //if (ticksAlive > 1000) isGame = false;
		//usleep(1000); 
    }
}
int PhysicWorld::getTicksAlive() { return ticksAlive; }
std::vector<PhysicObjectPtr> PhysicWorld::getWorld()
{
	return world;
}
PhysicWorld::PhysicWorld() { }
void PhysicWorld::start()
{
    isGame = true;
    //std::thread t(&PhysicWorld::update, this);
    //t.join();
	update();
}
void PhysicWorld::addObject(PhysicObjectPtr obj)
{
	obj->worldContainer = &world;
    world.push_back(obj);
}
void PhysicWorld::updateTick()
{
    if (ticksAlive == 0) isGame = true;

	for (uint i = 0; i < world.size(); i++)
	{
		if (world[i]->destroyNextFrame)
			world.erase(world.begin() + i);
	}
	//std::cout << ticksAlive << ")\n";
    updatePhysics();
    ticksAlive++;
    //usleep(1000);
    //if (ticksAlive > 1000) isGame = false;
}

PhysicObjectPtr PhysicWorld::addTriangle(re::Point2f pos, re::Point2f p1, re::Point2f p2, re::Point2f p3)
{
	PhysicObjectPtr obj = std::make_shared<PhysicObject>(pos);
    obj->setMass(1);
    obj->setFriction(0.5);
    obj->setBounciness(0.5);
	obj->addPoint(p1);
	obj->addPoint(p2);
	obj->addPoint(p3);
	obj->addEdge(0, 1);
	obj->addEdge(1, 2);
	obj->addEdge(2, 0);
	addObject(obj);
	return obj;
}
PhysicObjectPtr PhysicWorld::addQuadrangle(re::Point2f pos, re::Point2f p1, re::Point2f p2, re::Point2f p3, re::Point2f p4)
{
	PhysicObjectPtr obj = std::make_shared<PhysicObject>(pos);
    obj->setMass(1);
    obj->setFriction(0.5);
    obj->setBounciness(0.5);
	obj->addPoint(p1);
	obj->addPoint(p2);
	obj->addPoint(p3);
	obj->addPoint(p4);
	obj->addEdge(0, 1);
	obj->addEdge(1, 2);
	obj->addEdge(2, 3);
	obj->addEdge(3, 0);
	addObject(obj);
	return obj;
}

} // namespace re