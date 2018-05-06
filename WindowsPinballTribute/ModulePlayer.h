#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "p2SString.h"
#include "p2List.h"

class PhysBody;
class b2RevoluteJoint;
class b2PrismaticJoint;
struct SDL_Texture;

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

public:
	SDL_Texture* flipL_tex;
	SDL_Texture* flipR_tex;
	SDL_Texture* ball_tex;
	SDL_Texture* quicker_tex;

	uint start_game_sound;
	uint flippers_sound;
	uint quicker_sound;

	PhysBody* f_l;
	PhysBody* f_r;
	PhysBody* ball;
	p2List<PhysBody*> secondary_balls;
	PhysBody* quicker_box;

	b2RevoluteJoint* flip_l;
	b2RevoluteJoint* flip_r;
	b2PrismaticJoint* quicker;

	int life;
	int score;
	int last_score;
	p2SString title;
};