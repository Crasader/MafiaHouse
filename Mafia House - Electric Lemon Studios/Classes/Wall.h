#pragma once
#include "GameObject.h"
class Wall : public GameObject
{
public:
	Wall();
	~Wall();

	static Wall* create(const std::string& filename = "wall.png");

	void initObject(Vec2 startPos = Vec2(0, 0), Size size = Size(25, 25));//position, size
};
