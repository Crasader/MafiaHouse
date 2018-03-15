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

	void visionRays(vector<Vec2> *points, Vec2* start);//casts a bunch of rays; the enemies vision cone

	bool seeingPlayer() { return playerInVision; }

	int suspicionLevel = 0;//100 is max

protected:
	//Stuff for Vision Fields:
	bool didRun;
	int visionDegrees = 65;//width of angle of vision
	int visionRadius = 130;//how far vision reaches
	
	bool playerInVision = false;

	string visionContactName;
	int visionContactTag;

	//stuff for walking:
	float walkTime = 6.0f;
	float waitTime = 2.0f;
	float previousTurnTime = -1;
	float stopTime = -1;
};
