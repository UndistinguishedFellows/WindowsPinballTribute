#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}

	void GetPosition(int& x, int &y) const;
	void SetPosition(int x, int y, float angle = 0.0f);
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;
	void Push(float x, float y);
	void DeleteBody(PhysBody* physbody);

public:
	int width, height;
	b2Body* body;
	Module* listener;
};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener // TODO
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius, b2BodyType bodyType, float restitution = 0.0f, bool bullet = false, bool sensor = false);
	PhysBody* CreateRectangle(int x, int y, int width, int height, b2BodyType type = b2_staticBody, float restitution = 0.0f, bool sensor = false);
	PhysBody* CreateChain(int x, int y, int* points, int size, b2BodyType bodyType, float density = 1.0f, float restitution = 0.0f, bool sensor = false);
	PhysBody* CreatePolygon(int x, int y, int* points, int size, b2BodyType bodyType, float density = 1.0f, bool sensor = false);
	b2RevoluteJoint* CreateRevoluteJoint(const PhysBody* a, const PhysBody* b, const b2Vec2& Center_a, const b2Vec2 Center_b, const bool limit, const int lowAngle, const int upAngle, const int motorSpeed, const int maxTorque);
	b2PrismaticJoint* CreatePrismaticJoint(const PhysBody* a, const PhysBody* b);//Very specifick for our quicker

	void DeleteBody(PhysBody* body);
	void DeleteJoint(b2Joint* joint);

	// b2ContactListener ---
	void BeginContact(b2Contact* contact);

	b2World* GetWorld()
	{
		return world;
	}

private:

	bool debug;
	b2World* world;
	b2MouseJoint* mouse_joint;
	b2Body* ground;
	b2Body* body_clicked;
	b2Vec2 mouse_position;
};