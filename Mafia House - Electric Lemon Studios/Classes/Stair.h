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
	CREATE_SPRITE_FUNC(Stair, "stair.png");
	void initObject();

	void use(GameObject* user, Node* mainLayer);

	int pairNum = 0;//the number of the pair of stairways this stairway belongs to; ex. stairways 1A and 1B are both part of the number 1 pair
	int type = 0;//whether the Stairway is A or B: 0 = No Stairway, 1 = A, 2 = B

	const Size stairSize = Size(45, 105);
};

//Door Class
class Door : public GameObject
{
public:
	Door();
	~Door();
	CREATE_SPRITE_FUNC(Door, "door.png");

	virtual void initObject(Vec2 startPos = Vec2(0, 0));

	virtual void initObject(int orient, Vec2 startPos = Vec2(0, 0));

	void use();

	void unlock();

	bool checkOpen() { return isOpen; }

	float radius = 54.0f;//12px on each side of door

	Size size = Size(20, 110);

	Size useBox;

protected:
	bool locked = false;

	bool isOpen = false;
};

class Vent : public Door
{
public:
	Vent();
	~Vent();
	CREATE_SPRITE_FUNC(Vent, "door.png");

	void initObject(int orient, Vec2 startPos = Vec2(0, 0));
};