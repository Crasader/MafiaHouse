#pragma once
#include "GameObject.h"
class Item : public GameObject
{
public:
	Item();
	~Item();

	static Item* create(const std::string& filename = "knife.png");

	void initObject(Vec2 startPos = Vec2(0,0));

	void initHeldItem(int itemTag);
};

class Knife : public Item
{
public:
	Knife();
	~Knife();

	static Knife* create(const std::string& filename = "knife.png");
};