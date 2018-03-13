#pragma once
#include "GameObject.h"
#include "Item.h"
USING_NS_CC;

enum Input {
	PICKUP,
	DROP,
	USE_ITEM,
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

class Player : public GameObject
{
public:
	Player();
	~Player();
	CREATE_SPRITE_FUNC(Player, "player.png");
	void initObject(Vec2 startPos = Vec2(0, 0));

	void initAnimations();

	void noclip();
	bool clip = false;

	//functions for player actions:
	void update(float time, Node* mainLayer);
	void handleInput(Input input, Node* mainLayer);

	void resetActionChecks();//resets variables used to track what objects/items player will interact with/use

	void walk(Input input);

	void pickUpItem(Node* mainLayer);
	void dropItem(Node* mainLayer);

	void useItem();

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

	Item* heldItem = NULL;//for using held item

	bool hidden = false;

	enum Profile {
		STAND,
		CROUCH
	};

private:
	class PlayerState {
	public:
		virtual ~PlayerState() {}
		virtual void enter(Player* player, Node* mainLayer);
		virtual PlayerState* update(Player* player, float time, Node* mainLayer);
		virtual PlayerState* handleInput(Player* player, Input input, Node* MainLayer);
		virtual void exit(Player* player, Node* mainLayer);
	};

	class NeutralState : public PlayerState {
	public:
		PlayerState* handleInput(Player* player, Input input, Node* mainLayer);
	};

	class HideState : public PlayerState {
	public:
		void enter(Player* player, Node* mainLayer);
		PlayerState* update(Player* player, float time, Node* mainLayer);
		PlayerState* handleInput(Player* player, Input input, Node* MainLayer);
		void exit(Player* player, Node* mainLayer);
	};

	class AttackState : public PlayerState {
	public:
		void enter(Player* player, Node* mainLayer);
	};

	class NoClipState : public PlayerState {
	public:
		void enter(Player* player, Node* mainLayer);
		PlayerState* handleInput(Player* player, Input input, Node* MainLayer);
		void exit(Player* player, Node* mainLayer);
	};

	PlayerState* state = new NeutralState;
	PlayerState* newState = NULL;
	PlayerState* prevState = NULL;

	//animations:
	Animation* standAnimation;
	Animation* walkAnimation;
	Animation* crouchAnimation;
	Animation* crawlAnimation;
	Animation* stabAnimation;
	Animation* swingAnimation;
	//Animation* readyThrowAnimation;
	//Animation* throwAnimation;
	Animation* crouchStabAnimation;
	Animation* crouchSwingAnimation;
	//Animation* crouchReadyThrowAnimation;
	//Animation* crouchThrowAnimation;
	//Animation* stairAnimation;
	//Animation* hideAnimation;
	//Animation* pickupAnimation;
	//Animation* interactAnimation;

	Profile profile = STAND;

	std::vector<Item*> inventory;//items the player is carrying

	bool turned = false;//used for walking

	bool hideStart = false;//used for setting the object you are hiding behind transparent
};