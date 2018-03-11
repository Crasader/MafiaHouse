#pragma once
#include "GameObject.h"
#include "Item.h"
USING_NS_CC;

class Player : public GameObject
{
public:
	Player();
	~Player();
	CREATE_SPRITE_FUNC(Player, "player.png");

	void initObject(Vec2 startPos = Vec2(0, 0));

	//functions for player actions:
	void pickUpItem();

	int doorToUse = -1;//the tag of the door the player is opening/closing

	int itemToPickUp = -1;//the tag of the item the player is picking up

	int objectHidingBehind = -1;//the tag of the object the player is hiding behind

	bool hidden = false;
	bool hideStart = false;//used for setting the object you are hiding behind transparent

	int stairEntered = -1;//the tag of the stairway the player has used

	Item* heldItem = NULL;

	bool turned = false;

	void noclip();
	bool clip = false;

private:
	std::vector<Item*> items;//items the player is carrying
};
