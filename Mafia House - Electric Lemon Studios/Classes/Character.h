#pragma once
#include "GameObject.h"
#include "Item.h"
#include "Stair.h"
#include "EnvObject.h"
#include "GameLayer.h"

enum Input {
	PICKUP,
	DROP,
	USE_ITEM,
	USE_RELEASE,
	USE_STAIR,
	USE_DOOR,
	HIDE,
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_DOWN,
	STOP,
	NO_CLIP
};

class Character : public GameObject
{
public:
	virtual void initObject(Vec2 startPos = Vec2(0, 0));

	//functions for character actions:
	virtual void pickUpItem(GameLayer* mainLayer);
	virtual void dropItem(GameLayer* mainLayer);
	virtual void breakItem(GameLayer* mainLayer);

	virtual void beginUseItem();
	virtual void useItem();

	virtual void useDoor();
	virtual void useStair(GameLayer* mainLayer);

	virtual void hit(Item* item);

	//for Interacting with objects:
	Door* doorToUse = NULL;//the tag of the door the player can open/close
	Stair* stairToUse = NULL;//the tag of the stairway the player can use

	//for Picking Up items:
	Item* itemToPickUp = NULL;//the tag of the item the player can picking up
	//for storing held item:
	Item* heldItem = NULL;

	std::vector<Item*> inventory;//items the character is carrying

protected:
	//to check if character has been hit:
	bool isHit = false;

	//for movement:
	float moveSpeed = 1.0f;

	//variables used for the timing of attacking/using items:
	float attackPrepareTime = -1.0f;//time character begins to prepare and attack
	float attackStartTime = -1.0f;//time character actually begins the attack after release
	float attackEndTime = -1.0f;//time attack ends and englag begins

	//for physics body:
	Size bodySize;//main character hitbox

	//animations:
	GameAnimation standing;
	GameAnimation walking;
	GameAnimation stabbing;
	//GameAnimation swingAnimation;
	//GameAnimation throwAnimation;
	//GameAnimation stairAnimation;
	//GameAnimation pickupAnimation;
	//GameAnimation interactAnimation;
};