#pragma once
#include "GameObject.h"
#include "Item.h"
USING_NS_CC;

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

class Player : public GameObject
{
public:
	Player();
	~Player();
	CREATE_SPRITE_FUNC(Player, "player.png");
	CREATE_WITH_FRAME(Player);
	CREATE_WITH_FRAME_NAME(Player, "player/stand/001.png");
	void initObject(Vec2 startPos = Vec2(0, 0));

	void initAnimations();

	void noclip();
	bool clip = false;

	//functions for player actions:
	void update(Node* mainLayer, float time);
	void handleInput(Node* mainLayer, float time, Input input);

	void resetActionChecks();//resets variables used to track what objects/items player will interact with/use

	void walk(Input input);

	void pickUpItem(Node* mainLayer);
	void dropItem(Node* mainLayer);

	void beginUseItem();
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
	bool caught = false;

	enum Profile {
		STAND,
		CROUCH
	};

private:
	class PlayerState {
	public:
		virtual ~PlayerState() {}
		virtual void enter(Player* player, Node* mainLayer, float time);
		virtual PlayerState* update(Player* player, Node* mainLayer, float time);
		virtual PlayerState* handleInput(Player* player, Node* MainLayer, float time, Input input);
		virtual void exit(Player* player, Node* mainLayer);
	};

	class NeutralState : public PlayerState {
	public:
		void enter(Player* player, Node* mainLayer, float time);
		PlayerState* handleInput(Player* player, Node* MainLayer, float time, Input input);
	};

	class HideState : public PlayerState {
	public:
		void enter(Player* player, Node* mainLayer, float time);
		PlayerState* update(Player* player, Node* mainLayer, float time);
		PlayerState* handleInput(Player* player, Node* MainLayer, float time, Input input);
		void exit(Player* player, Node* mainLayer);
	};

	class AttackState : public PlayerState {
	public:
		void enter(Player* player, Node* mainLayer, float time);
		PlayerState* handleInput(Player* player, Node* MainLayer, float time, Input input);
		PlayerState* update(Player* player, Node* mainLayer, float time);
		void exit(Player* player, Node* mainLayer);
	};

	class NoClipState : public PlayerState {
	public:
		void enter(Player* player, Node* mainLayer, float time);
		PlayerState* handleInput(Player* player, Node* MainLayer, float time, Input input);
		void exit(Player* player, Node* mainLayer);
	};

	PlayerState* state = new NeutralState;
	PlayerState* newState = NULL;
	PlayerState* prevState = NULL;

	//animations:
	GameAnimation standing = GameAnimation(STAND, "player/stand/%03d.png", 1, 10 FRAMES);
	GameAnimation walking = GameAnimation(WALK, "player/walk/%03d.png", 6, 10 FRAMES);
	GameAnimation moonwalking = GameAnimation(MOONWALK, "player/walk2/%03d.png", 7, 8 FRAMES);
	//GameAnimation crouchAnimation;
	//GameAnimation crawlAnimation;
	GameAnimation stabbing = GameAnimation(WALK, "player/stab/%03d.png", 2, 10 FRAMES);;
	//GameAnimation swingAnimation;
	//GameAnimation readyThrowAnimation;
	//GameAnimation throwAnimation;
	//GameAnimation crouchStabAnimation;
	//GameAnimation crouchSwingAnimation;
	//GameAnimation crouchReadyThrowAnimation;
	//GameAnimation crouchThrowAnimation;
	//GameAnimation stairAnimation;
	//GameAnimation hideAnimation;
	//GameAnimation pickupAnimation;
	//GameAnimation interactAnimation;

	Profile profile = STAND;

	std::vector<Item*> inventory;//items the player is carrying

	bool turned = false;//used for walking
	bool hideStart = false;//used for setting the object you are hiding behind transparent

	float moveSpeed = 1.0f;
	int moveDirection = 0;

	//variables used for the timing of attacking/using items:
	float attackPrepareTime = -1.0f;//time player begins to prepare and attack
	float attackStartTime = -1.0f;//time player actually begins the attack after release
	float attackEndTime = -1.0f;//time attack ends and englag begins
	bool attackRelease = false;
};