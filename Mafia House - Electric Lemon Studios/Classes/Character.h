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

	void startAnimation(AnimationTag tag, GameAnimation animation);
	void stopAnimation(AnimationTag tag);

	float getPositionX();
	Vec2 getPosition();
	void setPosition(Vec2 pos);
	void setPositionX(float posX);

	void updateRoom(vector<RoomData> rooms);

	Size getSize() { return bodySize; }

	//functions for character actions:
	virtual void pickUpItem(GameLayer* mainLayer);
	virtual void dropItem(GameLayer* mainLayer);
	virtual void breakItem(GameLayer* mainLayer);

	virtual void beginUseItem();
	virtual void useItem();

	virtual void useDoor();
	virtual void useStair(GameLayer* mainLayer);

	bool checkDead();

	//for Interacting with objects:
	Door* doorToUse = NULL;//the tag of the door the player can open/close
	Stair* stairToUse = NULL;//the tag of the stairway the player can use

	//for Picking Up items:
	Item* itemToPickUp = NULL;//the tag of the item the player can picking up
	//for storing held item:
	Item* heldItem = NULL;

	Item* itemHitBy = NULL;

	std::vector<Item*> inventory;//items the character is carrying

protected:
	//VERY IMPORTANT: the number of px the actual character is from the very left side of their frames
	float FRAME_OFFSET = 18;
	//for physics body:
	Size bodySize;//main character hitbox

	//to check if character has been hit:
	bool isDead = false;

	//for movement:
	float moveSpeed = 1.0f;

	//for combat
	int hp = 100;

	//variables used for the timing of attacking/using items:
	float attackPrepareTime = -1.0f;//time character begins to prepare and attack
	float attackStartTime = -1.0f;//time character actually begins the attack after release
	float attackEndTime = -1.0f;//time attack ends and englag begins

	//animations:
	GameAnimation stand;
	GameAnimation walking;
	GameAnimation stab;
	GameAnimation swing;
	//GameAnimation throw;
	//GameAnimation stair;
	//GameAnimation pickup;
	//GameAnimation interact;
	//GameAnimation death;
};