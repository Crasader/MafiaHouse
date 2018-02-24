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

	std::vector<Item> items;//items the player is carrying
};
