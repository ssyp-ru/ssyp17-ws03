#include <RealEngine/baseApp.h>
#include <RealEngine/graphic.h>

#include <iostream>

///////////////////////////////////////////////////////////////////////////////
#include <string>
#include <map>
#include <vector>
#include <cmath>
#include <thread>

#include <unistd.h>

class Vector2f
{
public:
	double X, Y;
	Vector2f() 
    {
		X = 0;
		Y = 0;
	}
	Vector2f(double X, double Y)
	{
		this->X = X;
		this->Y = Y;
	}
	double Length()
	{
		if ((X == 0) && (Y == 0)) return 0;
		return sqrt(X*X+Y*Y);
	}
	void Normilize()
	{
		double len = Length();
		X /= len;
		Y /= len;
	}
	Vector2f Normilized()
	{
		return Vector2f(X / Length(), Y / Length());
	}
	Vector2f operator+(Vector2f vec)
	{
		return Vector2f(X + vec.X, Y + vec.Y);
	}
	Vector2f operator-(Vector2f vec)
	{
		return Vector2f(X - vec.X, Y - vec.Y);
	}
	Vector2f operator*(double val)
	{
		return Vector2f(X * val, Y * val);
	}
	Vector2f operator/(double val)
	{
		return Vector2f(X / val, Y / val);
	}
	void operator*=(double val)
	{
		X *= val;
		Y *= val;
	}
	void operator/=(double val)
	{
		X /= val;
		Y /= val;
	}
	void operator+=(Vector2f vec)
	{
		X += vec.X;
		Y += vec.Y;
	}
	void operator-=(Vector2f vec)
	{
		X -= vec.X;
		Y -= vec.Y;
	}
	bool operator!=(Vector2f vec)
	{
		return ((X != vec.X) || (Y != vec.Y));
	}
	bool operator==(Vector2f vec)
	{
		return ((X == vec.X) && (Y == vec.Y));
	}
	double operator*(Vector2f vec)
	{
		return X * vec.X + Y * vec.Y;
	}
	Vector2f projectOnVector(Vector2f vec)
	{
		// proj.x = ( dp / (b.x*b.x + b.y*b.y) ) * b.x;
		// proj.y = ( dp / (b.x*b.x + b.y*b.y) ) * b.y;
        if (vec.X == 0) return Vector2f(0, Y);
        if (vec.Y == 0) return Vector2f(X, 0);
		double scalarProduct = *this * vec;
		return Vector2f((scalarProduct / (vec.X * vec.X + vec.Y * vec.Y)) * vec.X, 
			            (scalarProduct / (vec.X * vec.X + vec.Y * vec.Y)) * vec.Y);
	}
	Vector2f getLeftNormal()
	{
		return Vector2f(Y, -X);
	}
	Vector2f getRightNormal()
	{
		return Vector2f(-Y, X);
	}
	double operator/(Vector2f vec)
	{
		if (vec.X != 0)
			return X / vec.X;
		else
			if (vec.Y != 0)
				return Y / vec.Y;
			else
				return 0;
	}
	Vector2f reflectFrom(Vector2f mirror)
	{
		return *this + (this->projectOnVector(mirror) - *this) * 2;
	}
    bool isCollinearTo(Vector2f vec)
    {
        if (((vec.X == 0) && (X == 0)) || ((vec.Y == 0) && (Y == 0))) return true;
        return (vec.X / vec.Y == X / Y);
    }
	void rotate(double radians)
	{
		double newX, newY;
		newX = X * cos(radians) + Y * sin(radians);
		newY = Y * cos(radians) - X * sin(radians);
		X = newX;
		Y = newY;
	}
	Vector2f rotated(double radians)
	{
		return Vector2f(X * cos(radians) + Y * sin(radians), Y * cos(radians) - X * sin(radians));
	}
};

class GameObject
{
public:
	class Edge
	{
	public:
		Vector2f *P1, *P2;
		Edge(Vector2f *P1, Vector2f *P2)
		{
			this->P1 = P1;
			this->P2 = P2;
		}
	};
	Vector2f position;
	double mass;
	double bounciness, friction;
    bool isRigidbodySimulated;
	std::vector<Edge> edges;
	std::vector<Vector2f> vertexes;

    std::vector<Edge> getEdges()
    {
        return edges;
    }

	GameObject()
	{
		GameObject(Vector2f(0, 0));
	}
	GameObject(Vector2f pos)
	{
		position = pos;
		circleRadius = 0;
        mass = 1;
		bounciness = 0.5;
		friction = 0.5;
        newPosition = Vector2f();
        isRigidbodySimulated = true;
		velocity = Vector2f();
		acceleration = Vector2f();
		massCenter = Vector2f();
		rotation = 0;
		rotationSpeed = 0;
	}
	void addForce(Vector2f force)
	{
		acceleration += force / mass;
	}
	void addImpulse(Vector2f impulse)
	{
		velocity += impulse / mass;
	}
	void addPoint(Vector2f point)
	{
		vertexes.push_back(point);
		if (point.Length() > circleRadius) circleRadius = point.Length();
		countMassCenter();
	}
	friend class Game;

	Vector2f velocity;
    Vector2f newPosition;
	Vector2f acceleration;
    Vector2f massCenter;
	int tag;
	double rotation;
	double rotationSpeed; // radians per sec
	double circleRadius;
    void countMassCenter() {
        double x = 0, y = 0;
        for (int i = 0; i < vertexes.size(); i++)
        {
            x += position.X + vertexes[i].X;
            y += position.Y + vertexes[i].Y;
        }
        massCenter = Vector2f(x/vertexes.size(), y/vertexes.size()) - position;
    }
	void rotate(double rotation)
	{
		for (int i = 0; i < vertexes.size(); i++)
		{
			vertexes[i].rotate(rotation);
		}
		this->rotation += rotation;
	}
    Vector2f getMassCenter()
    {
        return position+massCenter;
    }
	bool needTestWith(GameObject &gm)
	{
        //std::cout << (&gm == this);
		return (&gm == this)? false : (position - gm.position).Length() <= (circleRadius + gm.circleRadius);
	}
	Vector2f removeCollisionWith(GameObject &gm)
	{
		// ТУДУ: запилить таки проверку коллизий
		Vector2f outOfCollisionVector = Vector2f();

		Vector2f middleObjectsVec = (gm.getMassCenter() - getMassCenter()) * 2;

		//Vector2f curLineForProjection = ((getMassCenter() + *edges[i].P2) - (getMassCenter() + *edges[i].P1)).getRightNormal();
        std::vector<Vector2f> linesForProjection;
        for (int i = 0; i < edges.size(); i++)
        {
            bool needToAdd = true;
            Vector2f curLine = ((*edges[i].P1 + position) - (*edges[i].P2 + position)).getLeftNormal();
            for (int j = 0; j < linesForProjection.size(); j++)
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
        for (int i = 0; i < gm.edges.size(); i++)
        {
            bool needToAdd = true;
            Vector2f curLine = ((*gm.edges[i].P1 + gm.position) - (*gm.edges[i].P2 + gm.position)).getLeftNormal();
            for (int j = 0; j < linesForProjection.size(); j++)
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


		for (int i = 0; i < linesForProjection.size(); i++)
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

				for (int j = 0; j < vertexes.size(); j++)
				{
					self_projectedPoints.push_back((position + vertexes[j]).projectOnVector(linesForProjection[i]));
					//re::graphic::draw_line((position.X + vertexes[j].X) * 25, (position.Y + vertexes[j].Y) * 25,
					//					   self_projectedPoints[j].X * 25 + 200, self_projectedPoints[j].Y * 25 + 200, re::BLACK);
					//re::graphic::draw_rectangle(self_projectedPoints[j].X * 25 + 190, self_projectedPoints[j].Y * 25 + 190, 20, 20, re::BLACK);
					//std::cout << '>' << j << ' ' << self_projectedPoints[j].X << ' ' << self_projectedPoints[j].Y << std::endl;
				}
				
                Vector2f *self_smallestPoint = &self_projectedPoints[0], *self_biggestPoint = &self_projectedPoints[0];
				for (int j = 1; j < self_projectedPoints.size(); j++)
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

				for (int j = 0; j < gm.vertexes.size(); j++)
				{
					other_projectedPoints.push_back((gm.position + gm.vertexes[j]).projectOnVector(linesForProjection[i]));
					//re::graphic::draw_line((gm.position.X + gm.vertexes[j].X) * 25, (gm.position.Y + gm.vertexes[j].Y) * 25,
					//					   other_projectedPoints[j].X * 25 + 200, other_projectedPoints[j].Y * 25 + 200, re::BLACK);
					//re::graphic::draw_rectangle(other_projectedPoints[j].X * 25 + 190, other_projectedPoints[j].Y * 25 + 190, 20, 20, re::BLACK);
					//std::cout << '>' << j << ' ' << other_projectedPoints[j].X << ' ' << other_projectedPoints[j].Y << std::endl;
				}

				
                Vector2f *other_smallestPoint = &other_projectedPoints[0], *other_biggestPoint = &other_projectedPoints[0];
				for (int j = 1; j < other_projectedPoints.size(); j++)
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
	void updatePos()
	{
		if (isRigidbodySimulated) {
		    double deltaTime = 0.02;
		    velocity += acceleration * deltaTime;
		    acceleration.X = 0;
		    acceleration.Y = 0;
			position += velocity * deltaTime;
            newPosition = Vector2f(0, 0);

			rotate(rotationSpeed * deltaTime);
        } else {
			if (velocity.Length() != 0)
				velocity = Vector2f(0, 0);
			if (acceleration.Length() != 0)
				acceleration = Vector2f(0, 0);
		}
	}
};

class Game
{
public:
    bool isGame;
    int ticksAlive = 0;
	void updatePhysics()
	{
        //std::cout << "WTF\n";
		for (int i = 0; i < world.size(); i++)
			world[i]->updatePos();

		for (int i = 0; i < world.size(); i++)
		{
            if (world[i]->isRigidbodySimulated)
			    for (int j = 0; j < world.size(); j++)
			    {
                    if (world[i] != world[j])
			    	    if (world[i]->needTestWith(*(world[j]))) // is objects near to each other
			    	    {
			    	    	Vector2f outOfCollisionVector = world[i]->removeCollisionWith(*(world[j]));
			    	    	if (outOfCollisionVector.Length() > 1e-5)
			    	    	{
			    	    		//std::cout << "Collision detected.\n";

			    	    		//world[i]->position += outOfCollisionVector;

                                //usleep(1000000);

			    	    		Vector2f reflected = world[i]->velocity.reflectFrom(outOfCollisionVector.getLeftNormal());
                                Vector2f frictionProject = reflected.projectOnVector(outOfCollisionVector.getLeftNormal()) * (1 - (world[i]->friction + world[j]->friction) / 2);
                                Vector2f bouncinessProject = reflected.projectOnVector(outOfCollisionVector) * (world[i]->bounciness + world[j]->bounciness) / 2;
                                Vector2f impulseVector = frictionProject + bouncinessProject - world[i]->velocity;
                                world[i]->newPosition += outOfCollisionVector;
								if (world[j]->isRigidbodySimulated)
								{
                                	world[i]->velocity += impulseVector * (world[j]->mass / (world[i]->mass + world[j]->mass));
                                	world[j]->velocity -= impulseVector * (world[i]->mass / (world[i]->mass + world[j]->mass));
									//std::cout << "VELOCITY " << world[j]->mass << "/" <<world[i]->mass / (world[i]->mass + world[j]->mass);
								} else {
									world[i]->velocity += impulseVector;
								}
			    	    	}
			    	    }
			    }
		}

        for (int i = 0; i < world.size(); i++)
        {
            //if ((world[i]->newPosition.X != 0) && (world[i]->newPosition.Y != 0))
                world[i]->position += world[i]->newPosition;
				world[i]->newPosition = Vector2f(0, 0);
        }
    }
    std::vector<GameObject*> world;
	std::vector<GameObject*> getWorld()
	{
		return world;
	}
	Game()
    {
        
    }
    void start()
    {
        isGame = true;
        //std::thread t(&Game::update, this);

        //t.join();
		update();
    }
    void addObject(GameObject *obj)
    {
        world.push_back(obj);
    }
    void update()
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
    void updateTick()
    {
        if (ticksAlive == 0) isGame = true;
		//std::cout << ticksAlive << ")\n";
        updatePhysics();
        ticksAlive++;
        //usleep(1000);
        //if (ticksAlive > 1000) isGame = false;
    }
};
//////////////////////////////////////////////////////////////////

class MainApp : public re::IBaseApp{
public:
	Game mainGame;
    re::ImagePtr imgptr;

    void setup() override 
    {
/*
        GameObject *obj3 = new GameObject(Vector2f(8, 5));
		//obj3->rotationSpeed = 1;
        obj3->mass = 10;
        obj3->friction = 0.1;
        obj3->bounciness = 0.5;
	    obj3->addPoint(Vector2f(-0.2, 0));
	    obj3->addPoint(Vector2f(0, -2));
	    obj3->addPoint(Vector2f(0.2, 0));
	    obj3->edges.push_back(GameObject::Edge(&obj3->vertexes[0], &obj3->vertexes[1]));
	    obj3->edges.push_back(GameObject::Edge(&obj3->vertexes[1], &obj3->vertexes[2])); 
	    obj3->edges.push_back(GameObject::Edge(&obj3->vertexes[2], &obj3->vertexes[0]));
	    mainGame.addObject(obj3);*/

	    GameObject *obj1 = new GameObject(Vector2f(2, 4));
        obj1->mass = 10;
        obj1->friction = 0.0;
        obj1->bounciness = 0.5;
        obj1->addImpulse(Vector2f(1, 0));
	    obj1->addPoint(Vector2f(1, 1));
	    obj1->addPoint(Vector2f(1, -1));
	    obj1->addPoint(Vector2f(-1, -1));
	    obj1->addPoint(Vector2f(-1, 1));
	    obj1->edges.push_back(GameObject::Edge(&obj1->vertexes[0], &obj1->vertexes[1]));
	    obj1->edges.push_back(GameObject::Edge(&obj1->vertexes[1], &obj1->vertexes[2]));
	    obj1->edges.push_back(GameObject::Edge(&obj1->vertexes[2], &obj1->vertexes[3]));
	    obj1->edges.push_back(GameObject::Edge(&obj1->vertexes[3], &obj1->vertexes[0]));
	    mainGame.addObject(obj1);

		/*GameObject *obj5 = new GameObject(Vector2f(22, 4));
        obj5->mass = 10;
        obj5->friction = 0.0;
        obj5->bounciness = 0.5;
        obj5->addImpulse(Vector2f(-4, 0));
	    obj5->addPoint(Vector2f(1, 1));
	    obj5->addPoint(Vector2f(1, -1));
	    obj5->addPoint(Vector2f(-1, -1));
	    obj5->addPoint(Vector2f(-1, 1));
	    obj5->edges.push_back(GameObject::Edge(&obj5->vertexes[0], &obj5->vertexes[1]));
	    obj5->edges.push_back(GameObject::Edge(&obj5->vertexes[1], &obj5->vertexes[2]));
	    obj5->edges.push_back(GameObject::Edge(&obj5->vertexes[2], &obj5->vertexes[3]));
	    obj5->edges.push_back(GameObject::Edge(&obj5->vertexes[3], &obj5->vertexes[0]));
	    mainGame.addObject(obj5);*/

	    GameObject *obj2 = new GameObject(Vector2f(12, 15));
        obj2->isRigidbodySimulated = false;
        obj2->mass = 100;
        obj2->friction = -0.05;
        obj2->bounciness = 0.0;
	    obj2->addPoint(Vector2f(-11, 1));
	    obj2->addPoint(Vector2f(11, 1));
	    obj2->addPoint(Vector2f(11, -1));
	    obj2->addPoint(Vector2f(-11, -1));
	    obj2->edges.push_back(GameObject::Edge(&obj2->vertexes[0], &obj2->vertexes[1]));
	    obj2->edges.push_back(GameObject::Edge(&obj2->vertexes[1], &obj2->vertexes[2])); 
	    obj2->edges.push_back(GameObject::Edge(&obj2->vertexes[2], &obj2->vertexes[3]));
	    obj2->edges.push_back(GameObject::Edge(&obj2->vertexes[3], &obj2->vertexes[0]));
	    mainGame.addObject(obj2);

	    GameObject *obj3 = new GameObject(Vector2f(23, 16));
        obj3->isRigidbodySimulated = false;
        obj3->mass = 100;
        obj3->friction = 0.0;
        obj3->bounciness = 0.5;
	    obj3->addPoint(Vector2f(0, 0));
	    obj3->addPoint(Vector2f(0, -5));
	    obj3->addPoint(Vector2f(1, 0));
	    obj3->edges.push_back(GameObject::Edge(&obj3->vertexes[0], &obj3->vertexes[1]));
	    obj3->edges.push_back(GameObject::Edge(&obj3->vertexes[1], &obj3->vertexes[2])); 
	    obj3->edges.push_back(GameObject::Edge(&obj3->vertexes[2], &obj3->vertexes[0]));
	    mainGame.addObject(obj3);

		GameObject *obj4 = new GameObject(Vector2f(1, 16));
        obj4->isRigidbodySimulated = false;
        obj4->mass = 100;
        obj4->friction = 0.0;
        obj4->bounciness = 0.5;
	    obj4->addPoint(Vector2f(0, 0));
	    obj4->addPoint(Vector2f(0, -5));
	    obj4->addPoint(Vector2f(-1, 0));
	    obj4->edges.push_back(GameObject::Edge(&obj4->vertexes[0], &obj4->vertexes[1]));
	    obj4->edges.push_back(GameObject::Edge(&obj4->vertexes[1], &obj4->vertexes[2])); 
	    obj4->edges.push_back(GameObject::Edge(&obj4->vertexes[2], &obj4->vertexes[0]));
	    mainGame.addObject(obj4);

        //mainGame.start();
    }

    void update() override {
    }

    void display() override {
        mainGame.world[0]->addForce(Vector2f(0, 10 * mainGame.world[0]->mass));
        //mainGame.world[1]->addForce(Vector2f(0, 10 * mainGame.world[0]->mass));
        //mainGame.world[1]->addForce(Vector2f(0, 10 * mainGame.world[1]->mass));
        //mainGame.world[2]->addForce(Vector2f(0, -10 * mainGame.world[2]->mass));

        mainGame.updateTick();

        re::graphic::background(re::WHITE);
        for (int i = 0; i < mainGame.world.size(); i++)
        {
            for (int j = 0; j < mainGame.world[i]->vertexes.size(); j++)
            {
                re::graphic::draw_line((mainGame.getWorld()[i]->getEdges()[j].P1->X + mainGame.getWorld()[i]->position.X) * 25, 
                                       (mainGame.getWorld()[i]->getEdges()[j].P1->Y + mainGame.getWorld()[i]->position.Y) * 25,
                                       (mainGame.getWorld()[i]->getEdges()[j].P2->X + mainGame.getWorld()[i]->position.X) * 25, 
                                       (mainGame.getWorld()[i]->getEdges()[j].P2->Y + mainGame.getWorld()[i]->position.Y) * 25,
                                       re::BLACK);
				//re::graphic::draw_rectangle((mainGame.world[i]->vertexes[j].X + mainGame.world[i]->position.X) * 25 - 2, (mainGame.world[i]->vertexes[j].Y + mainGame.world[i]->position.Y) * 25 - 2, 4, 4, re::RED);
            }
			//Vector2f center = mainGame.world[i]->getMassCenter() * 25;
			//re::graphic::draw_rectangle(center.X - 5, center.Y - 5, 10, 10, re::RED);
        }
//        re::graphic::draw_rectangle(x, y, 50, 50, re::RED);
//        re::graphic::draw_rectangle(200, 200, 50, 50, re::RED);
//        re::graphic::draw_line( x, 50, 200, 200, re::BLACK );
//        re::graphic::draw_image( 0, 0, imgptr );
    }

    void on_key_pressed(re::Key key){
        if (key == re::Key::Escape){
            exit(0);
        } else if(key == (re::Key)112) {
            //re::graphic::goFullScreen();
        }
        std::cout << (int)key;
    }

    void on_mouse_move( int x0, int y0 )
    {
        x = x0 - 25;
        y = y0 + 25;
    }
private:
    int x;
    int y;
};

int main(){
    re::setWindowName( "RealEngine" );
    re::runApp( 640, 480, std::make_shared<MainApp>() );
    return 0;
}
