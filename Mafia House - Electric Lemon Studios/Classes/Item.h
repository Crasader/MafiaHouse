#pragma once
#include "GameObject.h"
class Item : public GameObject
{
public:
	Item();
	~Item();

	static Item* create(const std::string& filename = "knife.png");

	void initObject(Vec2 startPos);

	void initHeldItem(int itemTag);

protected:
	Node* pickUpRadius;//used to create invisible physics body to detect when player can pick up item
};

class Knife : public Item
{
public:
	Knife();
	~Knife();

	static Knife* create(const std::string& filename = "knife.png");
};