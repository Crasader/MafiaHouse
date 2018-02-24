#pragma once
#include "GameObject.h"
USING_NS_CC;

class Enemy : public GameObject
{
public:
	Enemy();
	~Enemy();

	static Enemy* create(const std::string& filename = "guard.png");

	void initObject(Vec2 startPos = Vec2(0, 0));

protected:
	Sprite* vision;
};