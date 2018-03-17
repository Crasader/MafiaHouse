#pragma once
#include "GameObject.h"
class Item : public GameObject
{
public:
	Item();
	~Item();
	CREATE_SPRITE_FUNC(Item, "knife.png");

	void initObject(Vec2 startPos = Vec2(0,0));
	void initRadius();

	enum AttackType {
		STAB,
		SWING
	};

	AttackType getAttackType() { return attackType; }
	float getStartTime() { return startTime; }
	float getAttackTime() { return attackTime; }
	float getLagTime() { return lagTime; }

	virtual void initHeldItem();
	virtual void initDroppedItem(Vec2 pos, bool flip);

	virtual void breakItem();
	void used();
	bool checkUsed() { return isUsed; }

	virtual void beginStab();
	virtual void beginSwing();

	virtual void stabSequence();
	virtual void swingSequence();

	bool enemyCanUse = true;

protected:
	//attack/use action parameters:
	AttackType attackType;
	float startTime;//startup time of attack
	float attackTime;//duration of active frames
	float lagTime;//enlag time of attack
	
	bool isUsed = false;//to check whether item has been used
};

class Knife : public Item
{
public:
	Knife();
	~Knife();
	CREATE_SPRITE_FUNC(Knife, "knife2.png");
};
