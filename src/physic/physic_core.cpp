#include "RealEngine/physic_core.h"
#include <cmath>
#include <iostream>
#include "RealEngine/logger.h"

namespace re {

Vector2f::Vector2f() 
{
	X = 0;
	Y = 0;
}
Vector2f::Vector2f(double X, double Y)
{
	this->X = X;
	this->Y = Y;
}
double Vector2f::Length()
{
	if ((X == 0) && (Y == 0)) return 0;
	return sqrt(X*X+Y*Y);
}
void Vector2f::Normalize()
{
	double len = Length();
	X /= len;
	Y /= len;
}
Vector2f Vector2f::Normilized()
{
	return Vector2f(X / Length(), Y / Length());
}
Vector2f Vector2f::operator+(Vector2f vec)
{
	return Vector2f(X + vec.X, Y + vec.Y);
}
Vector2f Vector2f::operator-(Vector2f vec)
{
	return Vector2f(X - vec.X, Y - vec.Y);
}
Vector2f Vector2f::operator*(double val)
{
	return Vector2f(X * val, Y * val);
}
Vector2f Vector2f::operator/(double val)
{
	return Vector2f(X / val, Y / val);
}
void Vector2f::operator*=(double val)
{
	X *= val;
	Y *= val;
}
void Vector2f::operator/=(double val)
{
	X /= val;
	Y /= val;
}
void Vector2f::operator+=(Vector2f vec)
{
	X += vec.X;
	Y += vec.Y;
}
void Vector2f::operator-=(Vector2f vec)
{
	X -= vec.X;
	Y -= vec.Y;
}
bool Vector2f::operator!=(Vector2f vec)
{
	return ((X != vec.X) || (Y != vec.Y));
}
bool Vector2f::operator==(Vector2f vec)
{
	return ((X == vec.X) && (Y == vec.Y));
}
double Vector2f::operator*(Vector2f vec)
{
	return X * vec.X + Y * vec.Y;
}
Vector2f Vector2f::projectOnVector(Vector2f vec)
{
	// proj.x = ( dp / (b.x*b.x + b.y*b.y) ) * b.x;
	// proj.y = ( dp / (b.x*b.x + b.y*b.y) ) * b.y;
    if (vec.X == 0) return Vector2f(0, Y);
    if (vec.Y == 0) return Vector2f(X, 0);
	double scalarProduct = *this * vec;
	return Vector2f((scalarProduct / (vec.X * vec.X + vec.Y * vec.Y)) * vec.X, 
		            (scalarProduct / (vec.X * vec.X + vec.Y * vec.Y)) * vec.Y);
}
Vector2f Vector2f::getLeftNormal()
{
	return Vector2f(Y, -X);
}
Vector2f Vector2f::getRightNormal()
{
	return Vector2f(-Y, X);
}
double Vector2f::operator/(Vector2f vec)
{
	if (vec.X != 0)
		return X / vec.X;
	else
		if (vec.Y != 0)
			return Y / vec.Y;
		else
			return 0;
}
Vector2f Vector2f::reflectFrom(Vector2f mirror)
{
	return *this + (this->projectOnVector(mirror) - *this) * 2;
}
bool Vector2f::isCollinearTo(Vector2f vec)
{
    if (((vec.X == 0) && (X == 0)) || ((vec.Y == 0) && (Y == 0))) return true;
    return (vec.X / vec.Y == X / Y);
}
void Vector2f::rotate(double radians)
{
	double newX, newY;
	newX = X * cos(radians) + Y * sin(radians);
	newY = Y * cos(radians) - X * sin(radians);
	X = newX;
	Y = newY;
}
Vector2f Vector2f::rotated(double radians)
{
	return Vector2f(X * cos(radians) + Y * sin(radians), Y * cos(radians) - X * sin(radians));
}
double Vector2f::angleBetween(Vector2f vec){
	double a = Length(), b = vec.Length(), c = (*this - vec).Length();
	if ((a == 0) || (b == 0) || (c == 0)) return 0;
	return acos((a*a + b*b - c*c) / (2*a*b));
}


Edge::Edge(Vector2f *P1, Vector2f *P2)
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


GameObject::GameObject()
{
	GameObject(Vector2f(0, 0));
}
GameObject::GameObject(Vector2f pos)
{
	position = pos;
	circleRadius = 0;
    mass = 1;
	bounciness = 0.5;
	friction = 0.5;
    newPosition = Vector2f();
    isRigidbodySimulated = true;
	isTrigger = false;
	velocity = Vector2f();
	acceleration = Vector2f();
	massCenter = Vector2f();
	rotation = 0;
	rotationSpeed = 0;
}
void GameObject::addForce(Vector2f force)
{
	acceleration += force / mass;
}
void GameObject::addImpulse(Vector2f impulse)
{
	velocity += impulse / mass;
}
Vector2f* GameObject::addPoint(Vector2f point)
{
	vertexes.push_back(point);
	if (point.Length() > circleRadius) circleRadius = point.Length();
	countMassCenter();
	return &vertexes[vertexes.size() - 1];
}
void GameObject::addEdge(int a, int b)
{
    Edge *edge = new Edge(&vertexes[a], &vertexes[b]);
	edges.push_back(*edge);
}
std::vector<Vector2f>* GameObject::getVertexes()
{
	return &vertexes;
}
std::vector<Edge>* GameObject::getEdges()
{
	return &edges;
}
void GameObject::rotate(double rotation)
{
	for (uint i = 0; i < vertexes.size(); i++)
	{
		vertexes[i].rotate(rotation);
	}
	this->rotation += rotation;
}
double GameObject::getMass() { return mass; }
void GameObject::setMass(double value) { value > 0? mass = value : mass = 1; }
double GameObject::getBounciness() { return bounciness; }
void GameObject::setBounciness(double value) { value >= 0? bounciness = value : bounciness = 0.5; }
double GameObject::getFriction() { return friction; }
void GameObject::setFriction(double value) { value <= 1? friction = value : friction = 0.5; }
bool GameObject::getRigidbodySimulated() {return isRigidbodySimulated; }
void GameObject::setRigidbodySimulated(bool value) { isRigidbodySimulated = value; }
bool GameObject::getIsTrigger() { return isTrigger; }
void GameObject::setIsTrigger(bool value) { isTrigger = value; }
double GameObject::getRotationSpeed() { return rotationSpeed; }
void GameObject::setRotationSpeed(double value) { rotationSpeed = value; }
Vector2f GameObject::getPosition() { return position; }
void GameObject::setPosition(Vector2f pos) { position = pos; }
Vector2f GameObject::getVelocity()
{
    return velocity;
}
void GameObject::setVelocity(Vector2f vec)
{
    velocity = vec;
}
void GameObject::countMassCenter() 
{
    double x = 0, y = 0;
    for (uint i = 0; i < vertexes.size(); i++)
    {
        x += position.X + vertexes[i].X;
        y += position.Y + vertexes[i].Y;
    }
    massCenter = Vector2f(x/vertexes.size(), y/vertexes.size()) - position;
}
Vector2f GameObject::getMassCenter()
{
    return position+massCenter;
}
bool GameObject::needTestWith(GameObject &gm)
{
    //std::cout << (&gm == this);
	return (&gm == this)? false : (position - gm.position).Length() <= (circleRadius + gm.circleRadius);
}
Vector2f GameObject::removeCollisionWith(GameObject &gm)
{
	// ТУДУ: запилить таки проверку коллизий
	Vector2f outOfCollisionVector = Vector2f();
	Vector2f middleObjectsVec = (gm.getMassCenter() - getMassCenter()) * 2;
	//Vector2f curLineForProjection = ((getMassCenter() + *edges[i].P2) - (getMassCenter() + *edges[i].P1)).getRightNormal();
    std::vector<Vector2f> linesForProjection;
    for (uint i = 0; i < edges.size(); i++)
    {
        bool needToAdd = true;
        Vector2f curLine = ((*edges[i].P1 + position) - (*edges[i].P2 + position)).getLeftNormal();
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
        Vector2f curLine = ((*gm.edges[i].P1 + gm.position) - (*gm.edges[i].P2 + gm.position)).getLeftNormal();
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
		Vector2f selfProjection;
		Vector2f otherProjection;
		Vector2f crossProjection;
        //std::cout << "(" << linesForProjection[i].X << ", " << linesForProjection[i].Y << ")\n";
		std::vector<Vector2f*> crossProjectionPoints;
		//#pragma region getting self-projection
		//
			std::vector<Vector2f> self_projectedPoints;
			//re::graphic::draw_line(linesForProjection[i].X * 250 + 200, linesForProjection[i].Y * 250 + 200,
			//					   linesForProjection[i].X * -250 + 200, linesForProjection[i].Y * -250 + 200,
			//					   re::BLACK);
			for (uint j = 0; j < vertexes.size(); j++)
			{
				self_projectedPoints.push_back((position + vertexes[j]).projectOnVector(linesForProjection[i]));
				//re::graphic::draw_line((position.X + vertexes[j].X) * 25, (position.Y + vertexes[j].Y) * 25,
				//					   self_projectedPoints[j].X * 25 + 200, self_projectedPoints[j].Y * 25 + 200, re::BLACK);
				//re::graphic::draw_rectangle(self_projectedPoints[j].X * 25 + 190, self_projectedPoints[j].Y * 25 + 190, 20, 20, re::BLACK);
				//std::cout << '>' << j << ' ' << self_projectedPoints[j].X << ' ' << self_projectedPoints[j].Y << std::endl;
			}
			
            Vector2f *self_smallestPoint = &self_projectedPoints[0], *self_biggestPoint = &self_projectedPoints[0];
			for (uint j = 1; j < self_projectedPoints.size(); j++)
			{
                if (linesForProjection[i].X == 0)
                {
                    if (self_projectedPoints[j].Y < self_smallestPoint->Y) self_smallestPoint = &self_projectedPoints[j];
                    if (self_projectedPoints[j].Y > self_biggestPoint->Y) self_biggestPoint = &self_projectedPoints[j];
                } else {
                    if (self_projectedPoints[j].X < self_smallestPoint->X) self_smallestPoint = &self_projectedPoints[j];
                    if (self_projectedPoints[j].X > self_biggestPoint->X) self_biggestPoint = &self_projectedPoints[j];
                }
			}
            selfProjection = *self_biggestPoint - *self_smallestPoint;
			crossProjectionPoints.push_back(self_smallestPoint);
			crossProjectionPoints.push_back(self_biggestPoint);
			if (selfProjection / (middleObjectsVec.projectOnVector(linesForProjection[i])) < 0) selfProjection *= -1;
		//
		//#pragma endregion
		//#pragma region getting other-projection
		//
			std::vector<Vector2f> other_projectedPoints;
			for (uint j = 0; j < gm.vertexes.size(); j++)
			{
				other_projectedPoints.push_back((gm.position + gm.vertexes[j]).projectOnVector(linesForProjection[i]));
				//re::graphic::draw_line((gm.position.X + gm.vertexes[j].X) * 25, (gm.position.Y + gm.vertexes[j].Y) * 25,
				//					   other_projectedPoints[j].X * 25 + 200, other_projectedPoints[j].Y * 25 + 200, re::BLACK);
				//re::graphic::draw_rectangle(other_projectedPoints[j].X * 25 + 190, other_projectedPoints[j].Y * 25 + 190, 20, 20, re::BLACK);
				//std::cout << '>' << j << ' ' << other_projectedPoints[j].X << ' ' << other_projectedPoints[j].Y << std::endl;
			}
			
            Vector2f *other_smallestPoint = &other_projectedPoints[0], *other_biggestPoint = &other_projectedPoints[0];
			for (uint j = 1; j < other_projectedPoints.size(); j++)
			{
                if (linesForProjection[i].X == 0)
                {
                    if (other_projectedPoints[j].Y < other_smallestPoint->Y) other_smallestPoint = &other_projectedPoints[j];
                    if (other_projectedPoints[j].Y > other_biggestPoint->Y) other_biggestPoint = &other_projectedPoints[j];
                } else {
                    if (other_projectedPoints[j].X < other_smallestPoint->X) other_smallestPoint = &other_projectedPoints[j];
                    if (other_projectedPoints[j].X > other_biggestPoint->X) other_biggestPoint = &other_projectedPoints[j];
                }
			}
            otherProjection = *other_biggestPoint - *other_smallestPoint;
			crossProjectionPoints.push_back(other_smallestPoint);
			crossProjectionPoints.push_back(other_biggestPoint);
			if (otherProjection / (middleObjectsVec.projectOnVector(linesForProjection[i])) < 0) otherProjection *= -1;
		//
		//#pragma endregion
		//#pragma region getting cross-projection
		if ((*crossProjectionPoints[0] - *crossProjectionPoints[3]).Length() > 
		    (*crossProjectionPoints[2] - *crossProjectionPoints[1]).Length())
			crossProjection = *crossProjectionPoints[0] - *crossProjectionPoints[3];
		else
			crossProjection = *crossProjectionPoints[2] - *crossProjectionPoints[1];
		if (crossProjection / (middleObjectsVec.projectOnVector(linesForProjection[i])) < 0) crossProjection *= -1;
		//#pragma endregion
        //std::cout << "cross " << crossProjection.X << " " << crossProjection.Y << "\n";
        //std::cout << "self " << selfProjection.X << " " << selfProjection.Y << '\n';
        //std::cout << "other " << otherProjection.X << " " << otherProjection.Y << '\n';
		if (crossProjection.Length() - selfProjection.Length() - otherProjection.Length() > 0)
        {
            //std::cout << "(" << middleObjectsVec.X << ", " << middleObjectsVec.Y << ")\n";
            //std::cout << middleObjectsVec.Length() << '\n';
            //std::cout << (middleObjectsVec.projectOnVector(curLineForProjection)).Length() << "\n";
            //std::cout << "(" << otherProjection.X << ", " << otherProjection.Y << ")\n";
			return Vector2f(0, 0);
        }
		if ((outOfCollisionVector.Length() == 0) || ((crossProjection - selfProjection - otherProjection).Length() < outOfCollisionVector.Length()))
		{
			outOfCollisionVector = (crossProjection - selfProjection - otherProjection) / 2;
		}
		//
		//for (auto p : crossProjectionPoints)
		//{
		//	re::graphic::draw_rectangle(p->X * 25 + 190, p->Y * 25 + 190, 20, 20, re::RED);
		//}
		//Vector2f a = *crossProjectionPoints[0], b = *crossProjectionPoints[1], c = *crossProjectionPoints[2], d = *crossProjectionPoints[3];
		//std::cout << a.isCollinearTo(b - a) << a.isCollinearTo(c - a) << a.isCollinearTo(d - a) << '\n';
		//std::cout << "/////\n" << a.X << " " << a.Y << '\n' << b.X << " " << b.Y << '\n' << c.X << " " << c.Y << '\n' << d.X << " " << d.Y << '\n';
		//if (((c - a) / (b - a) < 0) || ((c - a) / (b - a) > 1) && 
		//    ((d - a) / (b - a) < 0) || ((d - a) / (b - a) > 1) &&
		//	((a - c) / (d - c) < 0) || ((a - c) / (d - c) > 1) && 
		//    ((b - c) / (d - c) < 0) || ((b - c) / (d - c) > 1))
		//	return Vector2f(0, 0);
		
		//outOfCollisionVector = Vector2f(10, 0);
	}
	//std::cout << "(" << outOfCollisionVector.X << ", " << outOfCollisionVector.Y << ")\n";
	return outOfCollisionVector;
}
void GameObject::updatePos()
{
	double deltaTime = 0.02;
	if (isRigidbodySimulated) 
	{
	    velocity += acceleration * deltaTime;
	    acceleration.X = 0;
	    acceleration.Y = 0;
	}
	position += velocity * deltaTime;
    newPosition = Vector2f(0, 0);
	rotate(rotationSpeed * deltaTime);
    if (!isRigidbodySimulated) 
	{
		if (acceleration.Length() != 0)
			acceleration = Vector2f(0, 0);
	}
}
void GameObject::addCollisionCallback(std::function<void(std::shared_ptr<GameObject>, std::shared_ptr<GameObject>, Vector2f)> callback)
{
	onCollisionEvents.push_back(callback);
}
void GameObject::addTriggerCallback(std::function<void(std::shared_ptr<GameObject>, std::shared_ptr<GameObject>)> callback)
{
	onTriggerEvents.push_back(callback);
}


void Game::updatePhysics()
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
			    	Vector2f outOfCollisionVector = world[i]->removeCollisionWith(*(world[j]));
			    	if (outOfCollisionVector.Length() > 1e-5)
			    	{
			    		//std::cout << "Collision detected.\n";
						// тута вызывать событие коллизии
						// вызовется даже если обрабатываемый объект не динамичен
						if (!world[i]->isTrigger)
							for (uint c = 0; c < world[i]->onCollisionEvents.size(); c++)
								world[i]->onCollisionEvents[c](world[i], world[j], outOfCollisionVector);
						else
							if (world[j]->isRigidbodySimulated)
								for (uint c = 0; c < world[i]->onTriggerEvents.size(); c++)
									world[i]->onTriggerEvents[c](world[i], world[j]);
        				if (world[i]->isRigidbodySimulated)
						{
			    			Vector2f reflected = world[i]->velocity.reflectFrom(outOfCollisionVector.getLeftNormal());
							Vector2f frictionProject;
							if (world[i]->friction == -1)
                        		frictionProject = reflected.projectOnVector(outOfCollisionVector.getLeftNormal()) * (1 - world[j]->friction);
							else
								frictionProject = reflected.projectOnVector(outOfCollisionVector.getLeftNormal()) * (1 - (world[i]->friction + world[j]->friction) / 2);
                        	Vector2f bouncinessProject = reflected.projectOnVector(outOfCollisionVector) * (world[i]->bounciness + world[j]->bounciness) / 2;
							Vector2f impulseVector = Vector2f(0, 0);
							if (!((world[j]->friction == 1) && (world[i]->friction == -1) && (world[j]->velocity.Length() != 0)))
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
        //if ((world[i]->newPosition.X != 0) && (world[i]->newPosition.Y != 0))
            world[i]->position += world[i]->newPosition;
			world[i]->newPosition = Vector2f(0, 0);
    }
}
void Game::update()
{
    while(isGame)
    {
		//std::cout << ticksAlive << ")\n";
        updatePhysics();
        ticksAlive++;
        //if (ticksAlive > 1000) isGame = false;
		//usleep(1000); 
    }
}
int Game::getTicksAlive() { return ticksAlive; }
std::vector<GameObjectPtr> Game::getWorld()
{
	return world;
}
Game::Game() { }
void Game::start()
{
    isGame = true;
    //std::thread t(&Game::update, this);
    //t.join();
	update();
}
void Game::addObject(GameObjectPtr obj)
{
    world.push_back(obj);
}
void Game::updateTick()
{
    if (ticksAlive == 0) isGame = true;
	//std::cout << ticksAlive << ")\n";
    updatePhysics();
    ticksAlive++;
    //usleep(1000);
    //if (ticksAlive > 1000) isGame = false;
}

GameObjectPtr Game::addTriangle(Vector2f pos, Vector2f p1, Vector2f p2, Vector2f p3)
{
	GameObjectPtr obj = std::make_shared<GameObject>(pos);
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
GameObjectPtr Game::addQuadrangle(Vector2f pos, Vector2f p1, Vector2f p2, Vector2f p3, Vector2f p4)
{
	GameObjectPtr obj = std::make_shared<GameObject>(pos);
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