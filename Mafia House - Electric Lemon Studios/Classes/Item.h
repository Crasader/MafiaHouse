#pragma once
#include "GameObject.h"
class Item : public GameObject
{
public:
	Item();
	~Item() {}
	CREATE_SPRITE_FUNC(Item, "knife2.png");
	CREATE_WITH_FRAME(Item);
	CREATE_WITH_FRAME_NAME(Item, "items/knife.png");

	virtual void initObject(Vec2 startPos = Vec2(0,0));
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
	Effect getEffect() { return effect; }
	float getStartTime() { return startTime; }
	float getAttackTime() { return attackTime; }
	float getLagTime() { return lagTime; }
	int getRange() { return range; }
	int getRangeRadius() { return rangeRadius; }

	virtual void initHeldItem();
	virtual void initDroppedItem(Vec2 pos, bool flip);
	virtual void initGroundItem();

	virtual void breakItem();
	void used();

	void hitWall();

	virtual void prepareStab(float angle);
	virtual void prepareSwing(float angle);

	virtual void stabSequence(float angle, bool flip);
	virtual void swingSequence(float angle, bool flip);

	void checkSpeed();

	void playerInRange(Node* player);
	void hasMoved();
	bool enemyCanUse = false;

	int hp = 1;//keeps track fo how much item has been used
	float dmg = 0;
	float doorDmg = 0;
	Vec2 knockback = Vec2(0, 0);
	float hitstun = 0.0f;//amount of time attack prevents you from acting
	int priority = 0;//determines which attacks beat which
	int powerLevel = 0;

	bool canBreakDoor = false;
	bool isKey = false;

	bool didHitWall = false;

	bool enemyItem = false;//if it's being held by an enemy

protected:
	State state = GROUND;
	Effect effect = KILL;
	Node * pickUpRadius;
	//attack/use action parameters:
	AttackType attackType;
	float startTime;//startup time of attack
	float attackTime;//duration of active frames
	float lagTime;//enlag time of attack
	int range;//range of the weapons attack, for enemy ai
	float rangeRadius = 100;//for swinging weapons, possible for stabbing weapons once they have multi-directional attacks
};

class Fist : public Item//enemy attacking barehand, actually an invisible item
{
public:
	Fist();
	~Fist() {}
	CREATE_WITH_FRAME(Fist);
	CREATE_WITH_FRAME_NAME(Fist, "items/fist.png");
	void initObject(Vec2 startPos = Vec2(0, 0));
	void initHeldItem();
};

class Knife : public Item
{
public:
	Knife();
	~Knife() {}
	CREATE_SPRITE_FUNC(Knife, "knife2.png");
	CREATE_WITH_FRAME(Knife);
	CREATE_WITH_FRAME_NAME(Knife, "items/knife.png");
};

class Key : public Item
{
public:
	Key();
	~Key() {}
	CREATE_WITH_FRAME(Key);
	CREATE_WITH_FRAME_NAME(Key, "items/key.png");
};

class Hammer : public Item
{
public:
	Hammer();
	~Hammer() {}
	CREATE_WITH_FRAME(Hammer);
	CREATE_WITH_FRAME_NAME(Hammer, "items/hammer.png");
};