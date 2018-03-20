#pragma once
#include "GameObject.h"
class Item : public GameObject
{
public:
	Item();
	~Item();
	CREATE_SPRITE_FUNC(Item, "knife2.png");
	CREATE_WITH_FRAME(Item);
	CREATE_WITH_FRAME_NAME(Item, "items/knife.png");

	void initObject(Vec2 startPos = Vec2(0,0));
	void initRadius();

	enum AttackType {
		STAB,
		SWING
	};
	enum Effect {
		NONE,
		KNOCKOUT,
		KILL
	};
	enum State {
		GROUND,
		HELD,
		THROWN
	};

	State getState() { return state; }

	AttackType getAttackType() { return attackType; }
	float getStartTime() { return startTime; }
	float getAttackTime() { return attackTime; }
	float getLagTime() { return lagTime; }

	virtual void initHeldItem();
	virtual void initDroppedItem(Vec2 pos, bool flip);
	virtual void initGroundItem();

	virtual void breakItem();
	void used();

	void hitWall();

	virtual void beginStab();
	virtual void beginSwing();

	virtual void stabSequence();
	virtual void swingSequence();

	void checkSpeed();

	void playerInRange(Node* player);
	void hasMoved();
	bool enemyCanUse = false;

	int hp = 1;//keeps track fo how much item has been used
	int dmg = 10;
	bool canBreakDoor = false;
	bool isKey = false;

	bool didHitWall = false;

protected:
	State state = GROUND;
	Effect effect = KILL;
	Node * pickUpRadius;
	//attack/use action parameters:
	AttackType attackType;
	float startTime;//startup time of attack
	float attackTime;//duration of active frames
	float lagTime;//enlag time of attack
};

class Knife : public Item
{
public:
	Knife();
	~Knife();
	CREATE_SPRITE_FUNC(Knife, "knife2.png");
	CREATE_WITH_FRAME(Knife);
	CREATE_WITH_FRAME_NAME(Knife, "items/knife.png");
};

class Key : public Item
{
public:
	Key();
	~Key();
	CREATE_WITH_FRAME(Key);
	CREATE_WITH_FRAME_NAME(Key, "items/key.png");
};

class Hammer : public Item
{
public:
	Hammer();
	~Hammer();
	CREATE_WITH_FRAME(Hammer);
	CREATE_WITH_FRAME_NAME(Hammer, "items/hammer.png");
};