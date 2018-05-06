#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include "Globals.h"

class PhysBody;
class b2RevoluteJoint;
class b2PrismaticJoint;
class ModuleSceneIntro;

enum SensorType
{
	circle_yellow,
	circle_blue,
	circle_bouncer,
};

struct Sensor
{
	Sensor() : body(NULL), texture(NULL), sound(0), light(false), collision(false)
	{}

	Sensor(ModuleSceneIntro* physics, int x, int y, SensorType type, float restitution = 0.0f, bool sensor = true);

	PhysBody* body;
	int x;
	int y;
	SensorType type;
	SDL_Texture* texture;
	uint sound;
	bool light;
	bool collision;
	Uint32 timer;
};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:

	PhysBody* sensor;
	bool sensed;

	SDL_Texture* background;
	SDL_Texture* frontground;
	SDL_Texture* pusher;

	SDL_Texture* texture_circle_yellow_sensor;
	SDL_Texture* texture_circle_blue_sensor;
	SDL_Texture* texture_circle_bouncer_sensor;

	SDL_Texture* ball_left1;
	SDL_Texture* ball_left2;
	SDL_Texture* ball_left3;

	SDL_Texture* block1_tex;
	SDL_Texture* block2_tex;

	uint sound_circle_yellow_sensor;
	uint sound_circle_blue_sensor;
	uint sound_circle_bouncer_sensor;

	uint start_game_sound;
	uint die_sound;
	uint loser_sound;

	Uint32 die_count;
	Uint32 loser_count;

	p2Point<int> ray;
	bool ray_on;
	p2DynArray<Sensor> sensors;
	p2DynArray<PhysBody*> body_to_destroy;

	bool collisioned;//to check if there are collision in last frame

	bool died;
	bool loser;
	bool restart;

	PhysBody* pusher1;
	PhysBody* pusher2;
	PhysBody* block1;
	PhysBody* block2;
	bool createBlock1, createBlock2;

	PhysBody* bonus;
	PhysBody* bonus_body;
	Uint32 bonus_count;
	bool bonus_first;
	uint bonus_sound;
	uint bonus_sound_shot;

	Uint32 startCollision;

private:
	bool show_back;
};
