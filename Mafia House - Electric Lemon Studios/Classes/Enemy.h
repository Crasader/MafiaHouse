#pragma once
#include "GameObject.h"
USING_NS_CC;

class Enemy : public GameObject
{
public:
	Enemy();
	~Enemy();
	CREATE_SPRITE_FUNC(Enemy, "guard.png");

	void initObject(Vec2 startPos = Vec2(0,0));

	void walk(float time);

	//Stuff for Vision Fields:
	void visionRays(vector<Vec2> *points, Vec2* start);//casts a bunch of rays; the enemies vision cone
	bool didRun;
	int visionDegrees = 65;//width of angle of vision
	int visionRadius = 130;//how far vision reaches

	string visionContactName;
	int visionContactTag;

	bool playerInVision = false;

protected:
	float walkTime = 6.0f;
	float previousTurnTime = -1;
};
