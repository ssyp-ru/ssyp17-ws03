#pragma once
#include <vector>
#include <memory>

namespace re {

class Vector2f
{
public:
	double X, Y;
	Vector2f();
	Vector2f(double X, double Y);
	double Length();
	void Normilize();
	Vector2f Normilized();
	Vector2f operator+(Vector2f vec);
	Vector2f operator-(Vector2f vec);
	Vector2f operator*(double val);
	Vector2f operator/(double val);
	void operator*=(double val);
	void operator/=(double val);
	void operator+=(Vector2f vec);
	void operator-=(Vector2f vec);
	bool operator!=(Vector2f vec);
	bool operator==(Vector2f vec);
	double operator*(Vector2f vec);
	Vector2f projectOnVector(Vector2f vec);
	Vector2f getLeftNormal();
	Vector2f getRightNormal();
	double operator/(Vector2f vec);
	Vector2f reflectFrom(Vector2f mirror);
    bool isCollinearTo(Vector2f vec);
	void rotate(double radians);
	Vector2f rotated(double radians);
};

// Connects two points (by pointers) in edge
class Edge 
{ 
public:
	Vector2f *P1, *P2;
	Edge(Vector2f *P1, Vector2f *P2);
};

// INFO: vertexes must be setted in LOCAL space of figure(in other case you will get too big or broken object)
class GameObject
{
public:
	int tag; // Just useful variable, you can use it as you want
	GameObject(); // Makes empty gameobject in (0, 0) position
	GameObject(Vector2f pos); // Makes empty gameobject in your position
	void addForce(Vector2f force); // Adds a force to this gameobject
	void addImpulse(Vector2f impulse); // Adds a impulse to this gameobject
	Vector2f* addPoint(Vector2f point); // Adds a vertex in gameobject, returns pointer to this point (so you can save it and use in making edges)
	void addEdge(int a, int b); // Adds a edge to gameobject (a, b - indexes of vertexes in gameobject)
	std::vector<Vector2f>* getVertexes(); // Returns pointer to array with vertexes
	std::vector<Edge>* getEdges(); // Returns copy of array with edges
	void rotate(double rotation); // Turns gameobject in counter-clock-wise by 'rotation' radians
	double getMass(); // Returns current mass of object
	void setMass(double value); // Sets mass of gameobject(must be >= 0 (seriosly))
	double getBounciness(); // Returns current bounciness of gameobject
	void setBounciness(double value); // Sets bounciness of gameobject(must be >= 0, recommends also <= 1)
	double getFriction(); // Returns current friction of gameobject
	void setFriction(double value); // Sets friction of gameobject(recommended >= 0 and must be <= 1)
	bool getRigidbodySimulated(); // Returns true if gameobject is dynamic, false if static
	void setRigidbodySimulated(bool value); // Sets gameobject to be dynamic(true) or static(false)
	double getRotationSpeed(); // Returns rotation speed of gameobject
	void setRotationSpeed(double value); // Sets rotation speed of gameobject
	Vector2f getPosition(); // Returns current gameobject position
    void setPosition(Vector2f pos); // Sets position to gameobject
	Vector2f getVelocity();
	void setVelocity(Vector2f vec);
	friend class Game;
private:
    // just don't touch it.
	double mass;
	double bounciness, friction;
    bool isRigidbodySimulated;
	std::vector<Edge> edges;
	std::vector<Vector2f> vertexes;
	Vector2f position;
	Vector2f velocity;
    Vector2f newPosition;
	Vector2f acceleration;
    Vector2f massCenter;
	double rotation;
	double rotationSpeed; // radians per sec
	double circleRadius;
    void countMassCenter();
    Vector2f getMassCenter();
	bool needTestWith(GameObject &gm);
	Vector2f removeCollisionWith(GameObject &gm);
	void updatePos();
};
typedef std::shared_ptr<GameObject> GameObjectPtr;

// Main physic core class, contents all real world and makes all physic
class Game
{
private: 
    // just don't touch it.
    std::vector<GameObjectPtr> world;
    bool isGame;
    int ticksAlive = 0;
	void updatePhysics();
    void update();
public:
	int getTicksAlive(); // Returns count of ticks world exists
	std::vector<GameObjectPtr> getWorld(); // Returns copy of array of objects
	Game(); // Empty constructor... yep, too lazy to delete it.
    void start(); // Starts game by sending thread(that calles method) in
    void addObject(GameObjectPtr obj); // Adding object(GameObject) to world
    void updateTick(); // Makes one physic tick, use it if you don't want to send thread off by method 'start()'
};

} // namespace re