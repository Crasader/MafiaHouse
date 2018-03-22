#pragma once
#include "GameObject.h"
#include "Item.h"

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
	CREATE_WITH_FRAME(Stair);
	CREATE_WITH_FRAME_NAME(Stair, "objects/stair.png");
	void initObject();

	void playerInRange();

	void use(GameObject* user, Node* mainLayer);

	int pairNum = 0;//the number of the pair of stairways this stairway belongs to; ex. stairways 1A and 1B are both part of the number 1 pair
	int type = 0;//whether the Stairway is A or B: 0 = No Stairway, 1 = A, 2 = B

	const Size stairSize = Size(45, 105);

	//for pathfinding:
	float pathDistance = 0;//the total distance so far to the stair from the enemy
	float foundDepth;//the depth the pathfinding function was at when it found the stair
};

//Door Class
class Door : public GameObject
{
public:
	Door();
	~Door();
	CREATE_SPRITE_FUNC(Door, "door.png");
	CREATE_WITH_FRAME(Door);
	CREATE_WITH_FRAME_NAME(Door, "objects/door/001.png");

	virtual void initObject(Vec2 startPos = Vec2(0, 0));

	virtual void initObject(int orient, Vec2 startPos = Vec2(0, 0));

	void playerInRange();

	void updateColour();

	void itemHit(Item* item);

	void breakDoor();
	void updateBroken();

	bool checkLock() { return locked; }
	bool checkBroken() { return broken; }

	bool use();

	void unlock();

	void lock();

	bool checkOpen() { return isOpen; }

	float radius = 54.0f;//12px on each side of door

	Size size = Size(20, 110);

	Size useBox;

	bool defaultLocked = false;//whether enemies should lock the door or not, also if it starts locked

	const float maxHP = 100;
	float hp = maxHP;//for items to deal dmg to doors, to break them open if locked

protected:
	bool broken = false;

	bool locked = false;

	bool isOpen = false;
};

class Vent : public Door
{
public:
	Vent();
	~Vent();
	CREATE_SPRITE_FUNC(Vent, "vent.png");
	CREATE_WITH_FRAME(Vent);
	CREATE_WITH_FRAME_NAME(Vent, "objects/vent/001.png");

	void initObject(int orient, Vec2 startPos = Vec2(0, 0));
};