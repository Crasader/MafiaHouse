#pragma once
#include "GameObject.h"
#include "Item.h"
#include <vector>
USING_NS_CC;

class Player : public GameObject
{
public:
	Player();
	~Player();

	static Player* create(const std::string& filename = "player.png");

	void initObject(Vec2 startPos = Vec2(0, 0));

	//functions for player actions:
	void pickUpItem();

	int itemToPickUp = -1;

private:
	Item* heldItem;
	std::vector<Item*> items;//items the player is carrying
};
