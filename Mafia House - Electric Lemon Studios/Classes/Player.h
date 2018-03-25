#pragma once
#include "Character.h"

class Player : public Character
{
public:
	Player();
	~Player();
	CREATE_SPRITE_FUNC(Player, "player.png");
	CREATE_WITH_FRAME(Player);
	CREATE_WITH_FRAME_NAME(Player, "player/stand/001.png");

	void initObject(Vec2 startPos = Vec2(0, 0));

	//functions for player actions:
	void resetCollisionChecks();//resets variables used to track what objects/items player will interact with/use

	void walkPrepareAttack(Input input, float time);
	void walk(Input input, float time);
	void crouchWalk(Input input, float time);

	void jump();

	void pickUpItem(GameLayer* mainLayer);
	void dropItem(GameLayer* mainLayer);

	void useDoor();
	void useStair(GameLayer* mainLayer);

	void hide();
	void hiding();

	void wasHit(Item* item, float time);//being hit by an enemy attack
	
	//for player staying behind obejcts without causing earthquakes, doesn't use set position
	void stayWithin(HideObject* object);
	//getters:
	bool isHidden() { return hidden; }

	//pointers for interacting with objects:
	HideObject* objectToHideBehind = NULL;//the tag of the object the player can hide behind
	PhysObject* objectToClimb = NULL;
	bool isHidingUnder = false;
	//DeadBody* bodyToPickUp = NULL;

	void update(GameLayer* mainLayer, float time);
	void handleInput(GameLayer* mainLayer, float time, Input input);

	bool inVision = false;
	bool wasSeen = false;

	bool isHit = false;
	bool touchingFloor = true;
	bool isCrouched = false;

	//for standing on physical objects
	//Node* feet;
	//PhysicsBody* feetBody;
	bool noclip = false;

private:
	class State {
	public:
		string type = "default";
		virtual ~State() {}
		virtual void enter(Player* player, GameLayer* mainLayer, float time);
		virtual State* update(Player* player, GameLayer* mainLayer, float time);
		virtual State* handleInput(Player* player, GameLayer* mainLayer, float time, Input input);
		virtual void exit(Player* player, GameLayer* mainLayer);
	};
	class NeutralState : public State {
	public:
		NeutralState() { type = "neutral"; }
		void enter(Player* player, GameLayer* mainLayer, float time);
		State* update(Player* player, GameLayer* mainLayer, float time);
		State* handleInput(Player* player, GameLayer* MainLayer, float time, Input input);
	};
	class CrouchState : public State {
	public:
		CrouchState() { type = "crouch"; }
		void enter(Player* player, GameLayer* mainLayer, float time);
		State* update(Player* player, GameLayer* mainLayer, float time);
		State* handleInput(Player* player, GameLayer* MainLayer, float time, Input input);
	};
	class HideState : public State {
	public:
		HideState() { type = "hide"; }
		void enter(Player* player, GameLayer* mainLayer, float time);
		State* update(Player* player, GameLayer* mainLayer, float time);
		State* handleInput(Player* player, GameLayer* MainLayer, float time, Input input);
		void exit(Player* player, GameLayer* mainLayer);
	};
	class ClimbState : public State {
	public:
		void enter(Player* player, GameLayer* mainLayer, float time);
		State* update(Player* player, GameLayer* mainLayer, float time);
		void exit(Player* player, GameLayer* mainLayer);
	};
	class AttackState : public State {
	public:
		void enter(Player* player, GameLayer* mainLayer, float time);
		State* handleInput(Player* player, GameLayer* MainLayer, float time, Input input);
		State* update(Player* player, GameLayer* mainLayer, float time);
		void exit(Player* player, GameLayer* mainLayer);
	};
	class DeathState : public State {
	public:
		void enter(Player* player, GameLayer* mainLayer, float time);
		State * update(Player* player, GameLayer* mainLayer, float time);
		void exit(Player* player, GameLayer* mainLayer, float time);
	};
	class NoClipState : public State {
	public:
		void enter(Player* player, GameLayer* mainLayer, float time);
		State* handleInput(Player* player, GameLayer* MainLayer, float time, Input input);
		void exit(Player* player, GameLayer* mainLayer);
	};
	//initial state
	State* state = new NeutralState;
	State* newState = NULL;
	State* prevState = NULL;

	//for crouching (and climbing)
	PhysicsBody* crouchBody;
	Size standSize;
	Size crouchSize;

	//for moonwalking
	bool moonwalking = false;
	float prevStopTime = -1;
	float stopDelay = 4 FRAMES;//preventing stop inputs from being registered 5 frames from eachother, give moonwalk sliding a 5 frame window to perform

	//for the timing of attacks/using items:
	bool attackRelease = false;
	bool throwRelease = false;

	//for hiding:
	bool hidden = false;
	bool hittingLeft = false;
	bool hittingRight = false;

	//for walking:
	bool turned = false;
	int moveDirection = 0;

	//for playing audio:
	unsigned walkingSound;

	HideObject* hideObject = NULL;//object player is hiding behind

	//animations:
	GameAnimation moonwalk;
	GameAnimation crouch;
	GameAnimation standup;
	GameAnimation crouchwalk;
	//GameAnimation crouchStabAnimation;
	//GameAnimation crouchSwingAnimation;
	//GameAnimation crouchThrowAnimation;
	//GameAnimation hideAnimation;//reverse for uniding
	//GameAnimation climbAnimation;//reverse for climbing down
};