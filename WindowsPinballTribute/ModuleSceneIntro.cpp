#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	background = frontground = NULL;
	pusher1 = pusher2 = bonus = bonus_body = NULL;
	ray_on = false;
	sensed = false;
	show_back = true;
	collisioned = false;
	died = loser = restart = bonus_first = false;
	ball_left1 = ball_left2 = ball_left3 = NULL;
	block1_tex = block2_tex = NULL;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	frontground = App->textures->Load("pinball/frontground.png");
	background = App->textures->Load("pinball/windows_pinball.png");
	ball_left1 = App->textures->Load("pinball/1balls_left.png");
	ball_left2 = App->textures->Load("pinball/2balls_left.png");
	ball_left3 = App->textures->Load("pinball/3balls_left.png");
	block1_tex = App->textures->Load("pinball/blocker1.png");
	block2_tex = App->textures->Load("pinball/blocker2.png");

	App->audio->PlayMusic("pinball/PINBALL.MID");

	start_game_sound = App->audio->LoadFx("pinball/SOUND1.wav");
	App->audio->PlayFx(start_game_sound);

	die_sound = App->audio->LoadFx("pinball/SOUND27.wav");
	loser_sound = App->audio->LoadFx("pinball/SOUND3.wav");
	bonus_sound = App->audio->LoadFx("pinball/SOUND25.wav");
	bonus_sound_shot = App->audio->LoadFx("pinball/SOUND34.wav");

	App->renderer->camera.x = App->renderer->camera.y = 0;

	//--------------------------------
	int pinball_1[52] = {
		572, 123,
		558, 110,
		536, 97,
		511, 88,
		515, 90,
		536, 99,
		558, 112,
		584, 138,
		593, 153,
		603, 181,
		604, 213,
		575, 292,
		576, 365,
		567, 387,
		577, 370,
		600, 380,
		590, 400,
		602, 402,
		609, 437,
		614, 436,
		605, 337,
		618, 250,
		613, 198,
		606, 170,
		596, 150,
		586, 136
	};

	int pinball_2[60] = {
		715, 692,
		641, 181,
		613, 124,
		573, 91,
		528, 69,
		480, 58,
		418, 58,
		373, 67,
		346, 54,
		306, 58,
		286, 75,
		282, 103,
		300, 125,
		271, 159,
		262, 193,
		267, 250,
		275, 298,
		314, 360,
		310, 421,
		246, 439,
		219, 466,
		195, 640,
		233, 640,
		240, 591,
		328, 660,
		328, 691,
		164, 692,
		275, 28,
		630, 29,
		741, 690
	};

	int pinball_3[12] = {
		367, 131,
		410, 118,
		409, 98,
		396, 91,
		388, 93,
		366, 111
	};

	int pinball_4[32] = {
		539, 244,
		564, 213,
		567, 188,
		557, 156,
		540, 134,
		505, 111,
		492, 116,
		500, 121,
		501, 129,
		505, 121,
		517, 129,
		517, 140,
		522, 136,
		546, 157,
		552, 191,
		530, 236
	};

	int pinball_5[20] = {
		301, 173,
		294, 172,
		290, 206,
		297, 234,
		308, 259,
		331, 282,
		371, 255,
		366, 248,
		307, 221,
		300, 203
	};

	int pinball_6[12] = {
		250, 498,
		255, 498,
		254, 537,
		368, 626,
		366, 628,
		248, 537
	};

	int pinball_7[8] = {
		284, 479,
		290, 479,
		286, 520,
		281, 520
	};

	int pinball_8[12] = {
		626, 562,
		619, 479,
		615, 479,
		621, 561,
		540, 624,
		543, 626
	};

	int pinball_9[6] = {
		325, 481,
		319, 550,
		357, 576
	};

	int pinball_10[6] = {
		580, 482,
		585, 550,
		547, 573
	};

	int pinball_11[8] = {
		436, 114,
		436, 101,
		440, 101,
		440, 114
	};

	int pinball_12[8] = {
		464, 115,
		464, 101,
		469, 101,
		469, 115
	};

	int pinball_13[22] = {
		420, 234,
		465, 246,
		471, 241,
		472, 224,
		464, 230,
		454, 234,
		445, 233,
		436, 227,
		432, 221,
		431, 214,
		420, 230
	};

	int pinball_14[16] = {
		576, 691,
		576, 661,
		625, 622,
		629, 639,
		665, 638,
		630, 367,
		633, 340,
		677, 692
	};

	//--------------------------------
	App->physics->CreateChain(0, 0, pinball_1, 52, b2_staticBody, 1.0f, 0.25f);
	App->physics->CreateChain(0, 0, pinball_2, 60, b2_staticBody, 1.0f, 0.25f);
	App->physics->CreateChain(0, 0, pinball_3, 12, b2_staticBody, 1.0f, 0.25f);
	App->physics->CreateChain(0, 0, pinball_4, 32, b2_staticBody, 1.0f, 0.25f);
	App->physics->CreateChain(0, 0, pinball_5, 20, b2_staticBody, 1.0f, 0.25f);
	App->physics->CreateChain(0, 0, pinball_6, 12, b2_staticBody, 1.0f, 0.25f);
	App->physics->CreateChain(0, 0, pinball_7, 8, b2_staticBody), 1.0f, 0.25f;
	App->physics->CreateChain(0, 0, pinball_8, 12, b2_staticBody, 1.0f, 0.25f);
	App->physics->CreateChain(0, 0, pinball_9, 6, b2_staticBody), 1.0f, 0.25f;
	App->physics->CreateChain(0, 0, pinball_10, 6, b2_staticBody, 1.0f, 0.25f);
	App->physics->CreateChain(0, 0, pinball_11, 8, b2_staticBody, 1.0f, 0.25f);
	App->physics->CreateChain(0, 0, pinball_12, 8, b2_staticBody, 1.0f, 0.25f);
	App->physics->CreateChain(0, 0, pinball_13, 22, b2_staticBody, 1.0f, 0.25f);
	App->physics->CreateChain(0, 0, pinball_14, 16, b2_staticBody, 1.0f, 0.25f);

	//bounds
	int bound_1[8] = {
		324, 477,
		328, 479,
		360, 572,
		358, 576
	};

	int bound_2[8] = {
		581, 481,
		574, 482,
		545, 572,
		549, 575
	};

	App->physics->CreateChain(0, 0, bound_1, 8, b2_staticBody, 0.0f, 1.5f);
	App->physics->CreateChain(0, 0, bound_2, 8, b2_staticBody, 0.0f, 1.5f);

	int bound_3[6] = {
		573, 365,
		570, 318,
		575, 315
	};

	App->physics->CreateChain(0, 0, bound_3, 6, b2_staticBody, 0.0f, 1.5f);

	int bound_4[6] = {
		319, 417,
		320, 372,
		314, 372
	};

	App->physics->CreateChain(0, 0, bound_4, 6, b2_staticBody, 0.0f, 1.5f);

	int bound_5[8] = {
		528, 251,
		537, 255,
		541, 248,
		530, 243
	};

	App->physics->CreateChain(0, 0, bound_5, 8, b2_staticBody, 0.0f, 1.5f);

	//Create a box so the ball at the begining doesn't fall
	PhysBody* box = App->physics->CreateRectangle(695, 637, 17, 1, b2_staticBody);
	box->SetPosition(695, 637, -5.0f);

	//sensors for score
	texture_circle_yellow_sensor = App->textures->Load("pinball/light_yellow.png");
	sound_circle_yellow_sensor = App->audio->LoadFx("pinball/SOUND26.wav");

	texture_circle_blue_sensor = App->textures->Load("pinball/light_blue.png");
	sound_circle_blue_sensor = App->audio->LoadFx("pinball/SOUND16.wav");

	texture_circle_bouncer_sensor = App->textures->Load("pinball/light_bouncer.png");
	sound_circle_bouncer_sensor = App->audio->LoadFx("pinball/SOUND12.wav");

	sensors.PushBack(Sensor(this, 418, 101, SensorType::circle_yellow));
	sensors.PushBack(Sensor(this, 446, 99, SensorType::circle_yellow));
	sensors.PushBack(Sensor(this, 475, 103, SensorType::circle_yellow));

	sensors.PushBack(Sensor(this, 350, 78, SensorType::circle_yellow));
	sensors.PushBack(Sensor(this, 293, 94, SensorType::circle_yellow));
	sensors.PushBack(Sensor(this, 327, 71, SensorType::circle_yellow));

	sensors.PushBack(Sensor(this, 315, 189, SensorType::circle_yellow));
	sensors.PushBack(Sensor(this, 315, 205, SensorType::circle_yellow));
	sensors.PushBack(Sensor(this, 317, 218, SensorType::circle_yellow));

	sensors.PushBack(Sensor(this, 360, 159, SensorType::circle_yellow));
	sensors.PushBack(Sensor(this, 379, 155, SensorType::circle_yellow));
	sensors.PushBack(Sensor(this, 394, 148, SensorType::circle_yellow));
	sensors.PushBack(Sensor(this, 411, 140, SensorType::circle_yellow));

	sensors.PushBack(Sensor(this, 517, 161, SensorType::circle_yellow));
	sensors.PushBack(Sensor(this, 524, 175, SensorType::circle_yellow));
	sensors.PushBack(Sensor(this, 528, 187, SensorType::circle_yellow));

	sensors.PushBack(Sensor(this, 518, 140, SensorType::circle_yellow));

	sensors.PushBack(Sensor(this, 545, 303, SensorType::circle_yellow));
	sensors.PushBack(Sensor(this, 538, 325, SensorType::circle_yellow));
	sensors.PushBack(Sensor(this, 538, 350, SensorType::circle_yellow));
	sensors.PushBack(Sensor(this, 546, 372, SensorType::circle_yellow));

	sensors.PushBack(Sensor(this, 329, 377, SensorType::circle_yellow));
	sensors.PushBack(Sensor(this, 328, 394, SensorType::circle_yellow));
	sensors.PushBack(Sensor(this, 325, 410, SensorType::circle_yellow));

	sensors.PushBack(Sensor(this, 237, 477, SensorType::circle_yellow));
	sensors.PushBack(Sensor(this, 266, 528, SensorType::circle_yellow));
	sensors.PushBack(Sensor(this, 303, 475, SensorType::circle_yellow));

	sensors.PushBack(Sensor(this, 584, 475, SensorType::circle_yellow));
	sensors.PushBack(Sensor(this, 618,475, SensorType::circle_yellow));

	sensors.PushBack(Sensor(this, 445, 653, SensorType::circle_yellow));

	sensors.PushBack(Sensor(this, 444, 384, SensorType::circle_blue));
	sensors.PushBack(Sensor(this, 469, 388, SensorType::circle_blue));
	sensors.PushBack(Sensor(this, 489, 399, SensorType::circle_blue));
	sensors.PushBack(Sensor(this, 505, 414, SensorType::circle_blue));
	sensors.PushBack(Sensor(this, 515, 434, SensorType::circle_blue));

	sensors.PushBack(Sensor(this, 518, 457, SensorType::circle_blue));
	sensors.PushBack(Sensor(this, 509, 480, SensorType::circle_blue));
	sensors.PushBack(Sensor(this, 495, 501, SensorType::circle_blue));
	sensors.PushBack(Sensor(this, 471, 514, SensorType::circle_blue));
	sensors.PushBack(Sensor(this, 444, 519, SensorType::circle_blue));

	sensors.PushBack(Sensor(this, 419, 387, SensorType::circle_blue));
	sensors.PushBack(Sensor(this, 399, 396, SensorType::circle_blue));
	sensors.PushBack(Sensor(this, 383, 413, SensorType::circle_blue));
	sensors.PushBack(Sensor(this, 373, 432, SensorType::circle_blue));

	sensors.PushBack(Sensor(this, 371, 456, SensorType::circle_blue));
	sensors.PushBack(Sensor(this, 378, 479, SensorType::circle_blue));
	sensors.PushBack(Sensor(this, 393, 499, SensorType::circle_blue));
	sensors.PushBack(Sensor(this, 415, 512, SensorType::circle_blue));


	sensors.PushBack(Sensor(this, 324, 96, SensorType::circle_bouncer, 1.5f, false));
	sensors.PushBack(Sensor(this, 423, 182, SensorType::circle_bouncer, 1.5f, false));
	sensors.PushBack(Sensor(this, 454, 226, SensorType::circle_bouncer, 1.5f, false));
	sensors.PushBack(Sensor(this, 487, 170, SensorType::circle_bouncer, 1.5f, false));

	int bonus_points[8] = {
		590, 384,
		602, 375,
		581, 365,
		576, 377
	};
	bonus = App->physics->CreateChain(0, 0, bonus_points, 8, b2_staticBody, true);
	bonus->listener = this;

	//sensor for lose the game
	sensor = App->physics->CreateRectangle(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50, b2_staticBody, 0.0f, true);
	sensor->listener = this;

	//Pushers in each side of the pinball
	
	pusher1 = App->physics->CreateRectangle(215, 640, 40, 20, b2_staticBody, 3.0f);
	pusher1->listener = this;
	pusher2 = App->physics->CreateRectangle(642, 640, 40, 20, b2_staticBody, 3.0f);
	pusher2->listener = this;

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	App->textures->Unload(frontground);
	App->textures->Unload(background);
	App->textures->Unload(ball_left1);
	App->textures->Unload(ball_left2);
	App->textures->Unload(ball_left3);
	App->textures->Unload(block1_tex);
	App->textures->Unload(block2_tex);
	App->textures->Unload(texture_circle_blue_sensor);
	App->textures->Unload(texture_circle_bouncer_sensor);
	App->textures->Unload(texture_circle_yellow_sensor);
	App->textures->Unload(pusher);

	return true;
}

//reinitialize for check collisions
update_status ModuleSceneIntro::PreUpdate()
{
	if (collisioned == false)
	{
		for (uint i = 0; i < sensors.Count(); i++)
		{
			if (sensors[i].collision == true)
					sensors[i].collision = false;
		}
	}

	for (uint i = 0; i < sensors.Count(); i++)
	{
		if (sensors[i].type == circle_bouncer && sensors[i].light == true)
		{
			if ((SDL_GetTicks() - sensors[i].timer) > 150)
				sensors[i].light = false;
		}
	}

	collisioned = false;

	if (bonus_body != NULL)
	{
		if (bonus_first == false)
		{
			bonus_count = SDL_GetTicks();

			b2Vec2 speed(-10, 6);
			bonus_body->body->SetLinearVelocity(speed);
			bonus_body->body->SetAngularVelocity(0.0f);
			bonus_body->SetPosition(505, 140);

			App->audio->PlayFx(bonus_sound);
			App->audio->PlayFx(bonus_sound_shot);

			bonus_first = true;
		}

		else if (bonus_first && (SDL_GetTicks() - bonus_count) > 2100)
		{
			bonus_body = App->player->secondary_balls.add(App->physics->CreateCircle(505, 140, 10, b2_dynamicBody, 0.0f, true))->data;
			b2Vec2 speed(-10, 6);
			bonus_body->body->SetLinearVelocity(speed);
			bonus_body->body->SetAngularVelocity(0.0f);
			bonus_body->SetPosition(505, 140);

			App->audio->PlayFx(bonus_sound_shot);

			bonus_body = NULL;
			bonus_first = false;
		}
	}

	if (died)
	{
		if (loser == false)
		{
			b2Vec2 speed(0, 0);
			App->player->ball->body->SetLinearVelocity(speed);
			App->player->ball->body->SetAngularVelocity(0.0f);
			App->player->ball->SetPosition(680, 600);
		}

		App->audio->PlayFx(die_sound);
		die_count = SDL_GetTicks();

		died = false;
	}

	if (loser && (SDL_GetTicks() - die_count) > 1250)
	{
		for (uint i = 0; i < sensors.Count(); i++)
		{
			if (sensors[i].light == true)
				sensors[i].light = false;
		}

		block1->DeleteBody(block1);
		block1 = NULL;
		block2->DeleteBody(block2);
		block2 = NULL;

		App->audio->PlayFx(loser_sound);
		loser_count = SDL_GetTicks();

		loser = false;
		restart = true;
	}

	if (restart && (SDL_GetTicks() - loser_count) > 2400)
	{
		b2Vec2 speed(0, 0);
		App->player->ball->body->SetLinearVelocity(speed);
		App->player->ball->body->SetAngularVelocity(0.0f);
		App->player->ball->SetPosition(680, 600);

		restart = false;
	}

	if (body_to_destroy.Count() != 0)
	{
		for (int i = 0; i < body_to_destroy.Count(); i++)
		{
			App->physics->DeleteBody(body_to_destroy[i]);
		}
		body_to_destroy.Clear();
	}

	return UPDATE_CONTINUE;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)
		App->player->secondary_balls.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 10, b2_dynamicBody, 0.0f, true));

	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
		show_back = !show_back;

	if (show_back == true)
	{
		App->renderer->Blit(background, 0, 0);

		for (uint i = 0; i < sensors.Count(); i++)
		{
			if (sensors[i].type != circle_bouncer && sensors[i].light == true)
				App->renderer->Blit(sensors[i].texture, sensors[i].x, sensors[i].y);
		}

		App->renderer->Blit(App->player->flipL_tex, 360, 600, NULL, 1.0f, App->player->f_l->GetRotation(), 12, 39);
		App->renderer->Blit(App->player->flipR_tex, 475, 600, NULL, 1.0f, App->player->f_r->GetRotation(), 56, 41);

		//Render the ball
		int ball_x, ball_y;
		App->player->ball->GetPosition(ball_x, ball_y);
		int quicker_x, quicker_y;
		App->player->quicker_box->GetPosition(quicker_x, quicker_y);

		for (p2List_item<PhysBody*>* tmp = App->player->secondary_balls.getFirst(); tmp != NULL; tmp = tmp->next)
		{
			int secondary_ball_x, secondary_ball_y;
			tmp->data->GetPosition(secondary_ball_x, secondary_ball_y);
			App->renderer->Blit(App->player->ball_tex, secondary_ball_x, secondary_ball_y, NULL, 1.0f);
		}
		App->renderer->Blit(App->player->ball_tex, ball_x, ball_y, NULL, 1.0f);
		App->renderer->Blit(App->player->quicker_tex, quicker_x + 676, quicker_y + 647);
		App->renderer->Blit(frontground, 0, 0);


		if (loser == false && restart == false)
		{
			switch (App->player->life)
			{
			case 3:
				App->renderer->Blit(ball_left3, 1035, 264);
				break;
			case 2:
				App->renderer->Blit(ball_left2, 1035, 264);
				break;
			case 1:
				App->renderer->Blit(ball_left1, 1035, 264);
				break;
			default:
				break;
			}
		}

		if (block1 != NULL)
		{
			App->renderer->Blit(block1_tex, 209, 563);
		}
		if (block2 != NULL)
		{
			App->renderer->Blit(block2_tex, 623, 597);
		}


		for (uint i = 0; i < sensors.Count(); i++)
		{
			if (sensors[i].type == circle_bouncer && sensors[i].light == true)
				App->renderer->Blit(sensors[i].texture, sensors[i].x - 10, sensors[i].y - 20);
		}
	}


	// Prepare for raycast ------------------------------------------------------

	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	/*p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
	int x, y;
	c->data->GetPosition(x, y);
	if(c->data->Contains(App->input->GetMouseX(), App->input->GetMouseY()))
	App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
	c = c->next;
	}

	c = boxes.getFirst();
	*/


	// ray -----------------
	if (ray_on == true)
	{
		fVector destination(mouse.x - ray.x, mouse.y - ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if (normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
		App->player->flip_l->EnableMotor(true);

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
		App->player->flip_l->EnableMotor(false);

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
		App->player->flip_r->EnableMotor(true);

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
		App->player->flip_r->EnableMotor(false);

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->player->quicker->EnableMotor(true);

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_UP)
		App->player->quicker->EnableMotor(false);

	if (createBlock1 == true && (SDL_GetTicks() - startCollision) >= 150)
	{
		int block_1[8] = {
			209, 568,
			209, 563,
			242, 587,
			242, 590
		};
		
		block1 = App->physics->CreateChain(0, 0, block_1, 8, b2_staticBody, 1.0f, 0.25f);
		createBlock1 = false;
	}
	if (createBlock2 == true && (SDL_GetTicks() - startCollision) >= 150)
	{
		int block_2[8] = {
			623, 623,
			623, 618,
			655, 591,
			655, 596
		};
		block2 = App->physics->CreateChain(0, 0, block_2, 8, b2_staticBody, 1.0f, 0.25f);
		createBlock2 = false;
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	//App->audio->PlayFx(bonus_fx);

	/*
	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}*/
	if (bodyA == pusher1 || bodyB == pusher1) 
	{
		//App->player->ball->Push(0.0f, -200.0f);
		startCollision = SDL_GetTicks();
		createBlock1 = true;
	}
	if (bodyA == pusher2 || bodyB == pusher2)
	{
		//App->player->ball->Push(0.0f, -200.0f);
		startCollision = SDL_GetTicks();
		createBlock2 = true;
	}

	if (bodyA == sensor && bodyB != sensor)
	{
		if (App->player->life > 0 && bodyB == App->player->ball)
		{
			if (App->player->secondary_balls.count() != 0)
			{
				body_to_destroy.PushBack(bodyB);

				App->player->ball = App->player->secondary_balls.getLast()->data;
				App->player->secondary_balls.del(App->player->secondary_balls.getLast());
			}

			else
			{
				App->player->life--;
				died = true;
			}
		}
		else
		{
			body_to_destroy.PushBack(bodyB);
			App->player->secondary_balls.del(App->player->secondary_balls.findNode(bodyB));
		}

		if (App->player->life <= 0)
		{
			App->player->last_score = App->player->score;
			App->player->score = 0;
			loser = true;
			App->player->life = 3;

			if (died == false)
				died = true;
		}
	}

	for (uint i = 0; i < sensors.Count(); i++)
	{
		if (bodyA == sensors[i].body)
		{
			if (sensors[i].collision == false)
			{
				App->audio->PlayFx(sensors[i].sound); 
				sensors[i].collision = true;

				if (sensors[i].type == circle_bouncer)
				{
					sensors[i].light = true;
					sensors[i].timer = SDL_GetTicks();
				}

				else
					sensors[i].light = !sensors[i].light;

				if (sensors[i].light)
				{
					switch (sensors[i].type)
					{
					case circle_yellow:
						App->player->score += 20;
						break;

					case circle_blue:
						App->player->score += 10;
						break;

					case circle_bouncer:
						App->player->score += 25;
						break;

					default:
						break;
					}
				}
			}
		}
	}

	if (bodyA == bonus && bonus_body == NULL)
	{
		if (bodyB == App->player->ball)
			bonus_body = bodyB;
		
		else if (App->player->secondary_balls.count() != 0)
		{
			for (p2List_item<PhysBody*>* tmp = App->player->secondary_balls.getFirst(); tmp != NULL; tmp = tmp->next)
			{
				if (bodyB == tmp->data)
					bonus_body = tmp->data;
			}
		}
		App->player->score += 100;
	}

	collisioned = true;
}

//function for create sensors for score
Sensor::Sensor(ModuleSceneIntro* scene, int x, int y, SensorType type, float restitution, bool sensor)
{
	this->x = x;
	this->y = y;
	this->type = type;
	light = false;
	collision = false;
	int radius = 0;

	switch (type)
	{
	case circle_yellow:
		radius = 6;
		texture = scene->texture_circle_yellow_sensor;
		sound = scene->sound_circle_yellow_sensor;
		body = scene->App->physics->CreateCircle(x + radius, y + radius, radius, b2_staticBody, restitution, false, sensor);
		break;

	case circle_blue:
		radius = 7;
		texture = scene->texture_circle_blue_sensor;
		sound = scene->sound_circle_blue_sensor;
		body = scene->App->physics->CreateCircle(x + radius, y + radius, radius, b2_staticBody, restitution, false, sensor);
		break;

	case circle_bouncer:
		radius = 15;
		texture = scene->texture_circle_bouncer_sensor;
		sound = scene->sound_circle_bouncer_sensor;
		body = scene->App->physics->CreateCircle(x, y, radius, b2_staticBody, restitution, false, sensor);
		break;
	}

	body->listener = scene;
}
