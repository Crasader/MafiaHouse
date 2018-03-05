#pragma once
#include "GameObject.h"

class StairData {
public:
	StairData() {}
	~StairData() {}

	int pairNum = 0;//the number of the pair of stairways this stairway belongs to; ex. stairways 1A and 1B are both part of the number 1 pair
	int type = 0;//whether the Stairway is A or B: 0 = No Stairway, 1 = A, 2 = B
	Vec2 position = Vec2(0, 0);//position of the stairwayin the room
};

class Stair : public GameObject
{
public:
	Stair();
	~Stair();

	static Stair* create(const std::string& filename = "stair.png");

	void initObject();

	int pairNum = 0;//the number of the pair of stairways this stairway belongs to; ex. stairways 1A and 1B are both part of the number 1 pair
	int type = 0;//whether the Stairway is A or B: 0 = No Stairway, 1 = A, 2 = B

	const Size stairSize = Size(45, 105);
};
