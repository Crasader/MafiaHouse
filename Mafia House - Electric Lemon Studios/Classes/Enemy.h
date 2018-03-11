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
	int visionDegrees = 65;
	int visionRadius = 130;

	GameObject* visionSprite;

	string visionContactName;
	int visionContactTag;

	Vec2 contactPoint;

protected:
	float walkTime = 6.0f;
	float previousTurnTime = -1;
};
