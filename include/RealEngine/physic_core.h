#pragma once
#include <RealEngine/math.h>
#include <vector>
#include <memory>
#include <string>
#include <map>
#include <functional>

namespace re {

class Metadata
{
private:
	// just don't touch it
	std::map<std::string, int> int_values;
	std::map<std::string, double> double_values;
	std::map<std::string, std::string> string_values;
	std::map<std::string, bool> bool_values;
public:
	bool hasKey(std::string name); // returns true if this object have variable with that name

	void setInt(std::string name, int value); // sets value to variable with that name, if no this variable, creates it
	void setDouble(std::string name, double value); // sets value to variable with that name, if no this variable, creates it
	void setString(std::string name, std::string value); // sets value to variable with that name, if no this variable, creates it
	void setBool(std::string name, bool value); // sets value to variable with that name, if no this variable, creates it

	int getInt(std::string name); // get value from variable with that name, if map haven't this variable - returns 0
	double getDouble(std::string name); // get value from variable with that name, if map haven't this variable - returns 0.0
	std::string getString(std::string name); // get value from variable with that name, if map haven't this variable - returns ""
	bool getBool(std::string name); // get value from variable with that name, if map haven't this variable - returns false
};

//class Vector2f
//{
//public:
//	double X, Y;
//	Vector2f();
//	Vector2f(double X, double Y);
//	double Length();
//	void Normalize();
//	Vector2f Normalized();
//	Vector2f operator+(Vector2f vec);
//	Vector2f operator-(Vector2f vec);
//	Vector2f operator*(double val);
//	Vector2f operator/(double val);
//	void operator*=(double val);
//	void operator/=(double val);
//	void operator+=(Vector2f vec);
//	void operator-=(Vector2f vec);
//	bool operator!=(Vector2f vec);
//	bool operator==(Vector2f vec);
//	double operator*(Vector2f vec);
//	Vector2f projectOnVector(Vector2f vec);
//	Vector2f getLeftNormal();
//	Vector2f getRightNormal();
//	double operator/(Vector2f vec);
//	Vector2f reflectFrom(Vector2f mirror);
//    bool isCollinearTo(Vector2f vec);
//	void rotate(double radians);
//	Vector2f rotated(double radians);
//	double angleBetween(Vector2f vec);
//};

// Connects two points (by pointers) in edge
class Edge 
{ 
public:
	re::Point2f *P1, *P2;
	Edge(re::Point2f *P1, re::Point2f *P2);
};

// INFO: vertexes must be setted in LOCAL space of figure(in other case you will get too big or broken object)
class PhysicObject
{
public:
	Metadata metadata; // Just useful variable, read class Metadata
	std::vector<std::shared_ptr<PhysicObject>> *worldContainer;
	PhysicObject(); // Makes empty gameobject in (0, 0) position
	PhysicObject(re::Point2f pos); // Makes empty gameobject in your position
	void addForce(re::Point2f force); // Adds a force to this gameobject
	void addImpulse(re::Point2f impulse); // Adds a impulse to this gameobject
	re::Point2f* addPoint(re::Point2f point); // Adds a vertex in gameobject, returns pointer to this point (so you can save it and use in making edges)
	void addEdge(int a, int b); // Adds a edge to gameobject (a, b - indexes of vertexes in gameobject)
	std::vector<re::Point2f>* getVertexes(); // Returns pointer to array with vertexes
	std::vector<Edge>* getEdges(); // Returns copy of array with edges
	void rotate(double rotation); // Turns gameobject in counter-clock-wise by 'rotation' radians
	//#pragma region getters-and-setters
	double getMass(); // Returns current mass of object
	void setMass(double value); // Sets mass of gameobject(must be >= 0 (seriosly))
	double getBounciness(); // Returns current bounciness of gameobject
	void setBounciness(double value); // Sets bounciness of gameobject(must be >= 0, recommends also <= 1)
	double getFriction(); // Returns current friction of gameobject
	void setFriction(double value); // Sets friction of gameobject(recommended >= 0 and must be <= 1), if friction will be -1, will be counted only from other object.
	bool getRigidbodySimulated(); // Returns true if gameobject is dynamic, false if static
	void setRigidbodySimulated(bool value); // Sets gameobject to be dynamic(true) or static(false)
	bool getIsTrigger(); // Returns true if gameobject is trigger, false if object
	void setIsTrigger(bool value); // Sets gameobject to be trigger(true)(means it will not be collided with something) or to be object(false)
	double getRotationSpeed(); // Returns rotation speed of gameobject
	void setRotationSpeed(double value); // Sets rotation speed of gameobject
	re::Point2f getPosition(); // Returns current gameobject position
    void setPosition(re::Point2f pos); // Sets position to gameobject
	re::Point2f getVelocity(); 
	void setVelocity(re::Point2f vec);
	bool getCanChangeSpeed();
	void setCanChangeSpeed(bool value);
	//#pragma endregion
	void destroy();
	virtual void onCollisionStay(std::shared_ptr<PhysicObject> to, re::Point2f vec) { }
	virtual void onTriggerStay(std::shared_ptr<PhysicObject> obj) { }
	virtual ~PhysicObject() { }
	friend class PhysicWorld;
protected:
    // just don't touch it.
	bool canChangeSpeed = true;
	bool destroyNextFrame;
	double mass;
	double bounciness, friction;
    bool isRigidbodySimulated;
	bool isTrigger;
	std::vector<Edge> edges;
	std::vector<re::Point2f> vertexes;
	re::Point2f position;
	re::Point2f velocity;
    re::Point2f newPosition;
	re::Point2f acceleration;
    re::Point2f massCenter;
	double rotation;
	double rotationSpeed; // radians per sec
	double circleRadius;
	std::vector<std::function<void(std::shared_ptr<PhysicObject>, std::shared_ptr<PhysicObject>)>> onTriggerEvents; // PhysicObjectPtr trigger, PhysicObjectPtr object
	std::vector<std::function<void(std::shared_ptr<PhysicObject>, std::shared_ptr<PhysicObject>, re::Point2f)>> onCollisionEvents;
    void countMassCenter();
    re::Point2f getMassCenter();
	bool needTestWith(PhysicObject &gm);
	re::Point2f removeCollisionWith(PhysicObject &gm);
	void updatePos();
};
typedef std::shared_ptr<PhysicObject> PhysicObjectPtr;

// Main physic core class, contents all real world and makes all physic
class PhysicWorld
{
private: 
    // just don't touch it.
    std::vector<PhysicObjectPtr> world;
    bool isGame;
    int ticksAlive = 0;
	void updatePhysics();
    void update();
public:
	int getTicksAlive(); // Returns count of ticks world exists
	std::vector<PhysicObjectPtr> getWorld(); // Returns copy of array of objects
	PhysicWorld(); // Empty constructor... yep, too lazy to delete it.
    void start(); // Starts game by sending thread(that calles method) in
    void addObject(PhysicObjectPtr obj); // Adds object(PhysicObject) to world
    void updateTick(); // Makes one physic tick, use it if you don't want to send thread off by method 'start()'
	PhysicObjectPtr addTriangle(re::Point2f pos, re::Point2f p1, re::Point2f p2, re::Point2f p3); // Adds a triangle in world, pos - position, p1, p2, p3 - vertexes, returns pointer to this gameobject (vertexes in local space, remember it)
	PhysicObjectPtr addQuadrangle(re::Point2f pos, re::Point2f p1, re::Point2f p2, re::Point2f p3, re::Point2f p4); // same as triangle, but 4 vertexes
};

} // namespace re

// TODO: 
// +создание четырехугольников, треугольников
// +разобраться с трением
// +запилить карту метаданных
// +сделать триггеры (bool isTrigger)
// +переместить отключение обработки у нефизических объектов, что б они могли
// +вызывать события столкновения
// +сделать возможным перемещение у нефизических объектов
// +запилить события столкновений