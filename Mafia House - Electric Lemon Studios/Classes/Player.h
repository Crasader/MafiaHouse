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
	void resetActionChecks();//resets variables used to track what objects/items player will interact with/use
	void pickUpItem(Node* mainLayer);
	void dropItem(Node* mainLayer);

	void useDoor(Node* mainLayer);
	void useStair(Node* mainLayer);

	void hide(Node* mainLayer);
	void hiding(Node* mainLayer);

	//for Interacting with objects:
	int doorToUse = -1;//the tag of the door the player can open/close
	int stairToUse = -1;//the tag of the stairway the player can use
	int objectToHideBehind = -1;//the tag of the object the player can hide behind

	//for Picking Up items:
	int itemToPickUp = -1;//the tag of the item the player can picking up
	int bodyToPickUp = -1;

	//for using held item:
	Item* heldItem = NULL;

	bool hidden = false;
	bool hideStart = false;//used for setting the object you are hiding behind transparent


	bool turned = false;

	void noclip();
	bool clip = false;

private:
	std::vector<Item*> inventory;//items the player is carrying
};

class PlayerState {
public:
	virtual ~PlayerState() {}
	virtual void update() {}
};