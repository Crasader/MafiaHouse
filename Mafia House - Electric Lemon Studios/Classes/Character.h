#pragma once
#include "GameObject.h"
//#include "Item.h"
#include "DeadBody.h"
#include "Stair.h"
#include "EnvObject.h"
#include "GameLayer.h"

enum Input {
	PICKUP,
	DROP,
	USE_ITEM,
	USE_RELEASE,
	THROW_ITEM,
	THROW_RELEASE,
	USE_STAIR,
	USE_DOOR,
	HIDE,
	ROLL,
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_DOWN,
	AIM_UP,
	AIM_UP_LEFT,
	AIM_UP_RIGHT,
	AIM_DOWN,
	AIM_DOWN_LEFT,
	AIM_DOWN_RIGHT,
	AIM_LEFT,
	AIM_RIGHT,
	STOP,
	NO_CLIP
};

class Character : public GameObject
{
public:
	virtual void initObject(Vec2 startPos = Vec2(0, 0));

	virtual void flipX();

	float getPositionX();
	Vec2 getPosition();
	void setPosition(Vec2 pos);
	void setPositionX(float posX);
	void setRoomPosition(Vec2 roomPos, Vec2 position);//set the objects position relative to the room it is generated inside

	void updateRoom(vector<RoomData*> rooms);

	Size getSize() { return bodySize; }

	//functions for character actions:
	virtual void pickUpItem(GameLayer* mainLayer);
	virtual void dropItem(GameLayer* mainLayer);
	virtual void throwItem(GameLayer* mainLayer, float time);
	virtual void breakItem(GameLayer* mainLayer);

	virtual void beginThrowItem();
	virtual void beginUseItem(float angle);
	virtual void useItem(float angle);
	virtual void finishUseItem();

	virtual void useDoor();
	virtual void useStair(GameLayer* mainLayer);

	bool isReallyDead() { return isDead; }
	bool checkDead();

	bool touchingWall = false;

	//for Interacting with objects:
	Door* doorToUse = NULL;//the tag of the door the player can open/close
	Stair* stairToUse = NULL;//the tag of the stairway the player can use

	Stair* usedStair = NULL;

	//for Picking Up items:
	Item* itemToPickUp = NULL;//the the item the player can pick up
	//for storing held item:
	Item* heldItem = NULL;

	Item* itemHitBy = NULL;

	Item* thrownItem = NULL;

	Item* itemBumpedBy = NULL;//an item that has hit the enemy, but not hard enough to kill them
	Vec2 directionHitFrom;

	std::vector<Item*> inventory;//items the character is carrying

	//standing animation
	GameAnimation stand;

protected:
	//VERY IMPORTANT: the number of px the actual character is from the very left side of their frames
	float FRAME_OFFSET = 18;
	//for physics body:
	Size bodySize;//main character hitbox

	//to check if character has been hit:
	bool isDead = false;

	//for aiming
	float aimAngle = 0;//-90 is up, +90 is down
	//for throwing items
	float thrownItemDelay = 60 FRAMES;//the time after you throw an item during which you cannot be hit by it

	//for movement:
	float moveSpeed = 1.0f;

	//for getting hit:
	float hitStunStart = -1.;
	float hitStunTime = 0.0f;
	bool wasInHitStun = false;

	//for hitting walls/doors
	bool hitWallWithItem = false;

	//for combat
	float maxHP = 100;
	float hp = maxHP;

	//variables used for the timing of attacking/using items:
	float attackPrepareTime = -1.0f;//time character begins to prepare and attack
	float attackStartTime = -1.0f;//time character actually begins the attack after release
	float attackEndTime = -1.0f;//time attack ends and englag begins

	//animations:
	GameAnimation walking;
	GameAnimation stab;
	GameAnimation swing;
	GameAnimation throwing;
	GameAnimation stairuse;
	GameAnimation dying;
	//GameAnimation interact;
};