#pragma once
#include "GameObject.h"
class Item : public GameObject
{
public:
	Item();
	~Item();
	CREATE_SPRITE_FUNC(Item, "knife.png");

	void initObject(Vec2 startPos = Vec2(0,0));

	virtual void initHeldItem(int itemTag);
};

class Knife : public Item
{
public:
	Knife();
	~Knife();
	CREATE_SPRITE_FUNC(Knife, "knife2.png");
};
