#pragma once
#include "Character.h"

class Player : public Character
{
public:
	Player();
	~Player();
	CREATE_EMPTY_SPRITE_FUNC(Player);
	CREATE_WITH_FRAME(Player);
	CREATE_WITH_FRAME_NAME(Player, "player/stand/001.png");

	void initObject(Vec2 startPos = Vec2(0, 0));

	void flipX();
	//functions for player actions:
	void resetCollisionChecks(float time);//resets variables used to track what objects/items player will interact with/use

	void walkPrepareAttack(Input input, float time);
	void walk(Input input, float time);
	void crouchWalk(Input input, float time);

	void jump();
	void fallAttack();

	void pickUpItem(GameLayer* mainLayer);
	void dropItem(GameLayer* mainLayer, float time);

	void pickUpBody(GameLayer* mainLayer);
	void dropBody(GameLayer* mainLayer, float time);
	void beginThrowBody(float time);
	void throwBody(GameLayer* mainLayer, float time);

	void useDoor();
	void useStair(GameLayer* mainLayer);

	void hide();
	void hiding();

	void beginThrowItem(float time);
	void throwItem(GameLayer* mainLayer, float time);
	void beginUseItem(float angle);
	void useItem(float angle);
	void finishUseItem();

	void wasHit(Item* item, float time);//being hit by an enemy attack

	void hearNoise(string name);//for playing audio of noises the player hears
	
	//for player staying behind obejcts without causing earthquakes, doesn't use set position
	void stayWithin(HideObject* object);
	//getters:
	bool isHidden() { return hidden; }
	float getHP() { return hp; }
	float getMaxHP() { return maxHP; }

	//pointers for interacting with objects:
	HideObject* objectToHideBehind = NULL;//the tag of the object the player can hide behind
	PhysObject* objectToClimb = NULL;
	bool isHidingUnder = false;

	DeadBody* bodyToPickUp = NULL;
	DeadBody* heldBody = NULL;

	void update(GameLayer* mainLayer, float time);
	void handleInput(GameLayer* mainLayer, float time, Input input);

	bool inVision = false;
	bool wasSeen = false;
	float seenTime = -1;//last time seen by enemy

	bool isHit = false;
	bool touchingFloor = true;
	bool isCrouched = false;
	bool wasFalling = false;
	bool wasClimbing = false;
	bool hasJumped = false;

	bool behindObject = false;

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
		virtual void exit(Player* player, GameLayer* mainLayer, float time);
	};
	class NeutralState : public State {
	public:
		NeutralState() { type = "neutral"; }
		void enter(Player* player, GameLayer* mainLayer, float time);
		State* update(Player* player, GameLayer* mainLayer, float time);
		State* handleInput(Player* player, GameLayer* MainLayer, float time, Input input);
		void exit(Player* player, GameLayer* mainLayer, float time);
	};
	class CrouchState : public State {
	public:
		CrouchState() { type = "crouch"; }
		void enter(Player* player, GameLayer* mainLayer, float time);
		State* update(Player* player, GameLayer* mainLayer, float time);
		State* handleInput(Player* player, GameLayer* MainLayer, float time, Input input);
	};
	class JumpState : public State {
	public:
		void enter(Player* player, GameLayer* mainLayer, float time);
		State* update(Player* player, GameLayer* mainLayer, float time);
		State* handleInput(Player* player, GameLayer* MainLayer, float time, Input input);
		void exit(Player* player, GameLayer* mainLayer, float time);
	};
	class FallState : public State {
	public:
		FallState() { type = "fall"; }
		void enter(Player* player, GameLayer* mainLayer, float time);
		State* update(Player* player, GameLayer* mainLayer, float time);
		State* handleInput(Player* player, GameLayer* MainLayer, float time, Input input);
		void exit(Player* player, GameLayer* mainLayer, float time);
	};
	class ClimbState : public State {
	public:
		ClimbState() { type = "climb"; }
		void enter(Player* player, GameLayer* mainLayer, float time);
		State* update(Player* player, GameLayer* mainLayer, float time);
		void exit(Player* player, GameLayer* mainLayer, float time);
	};
	class ThrowState : public State {
	public:
		ThrowState() { type = "throw"; }
		void enter(Player* player, GameLayer* mainLayer, float time);
		State* handleInput(Player* player, GameLayer* MainLayer, float time, Input input);
		State* update(Player* player, GameLayer* mainLayer, float time);
		void exit(Player* player, GameLayer* mainLayer, float time);
	};
	class AttackState : public State {
	public:
		AttackState() { type = "attack"; }
		void enter(Player* player, GameLayer* mainLayer, float time);
		State* handleInput(Player* player, GameLayer* MainLayer, float time, Input input);
		State* update(Player* player, GameLayer* mainLayer, float time);
		void exit(Player* player, GameLayer* mainLayer, float time);
	};
	class RollState : public State {
	public:
		RollState() { type = "roll"; }
		void enter(Player* player, GameLayer* mainLayer, float time);
		State* update(Player* player, GameLayer* mainLayer, float time);
		State* handleInput(Player* player, GameLayer* MainLayer, float time, Input input);
		void exit(Player* player, GameLayer* mainLayer, float time);
	};
	class HideState : public State {
	public:
		HideState() { type = "hide"; }
		void enter(Player* player, GameLayer* mainLayer, float time);
		State* update(Player* player, GameLayer* mainLayer, float time);
		State* handleInput(Player* player, GameLayer* MainLayer, float time, Input input);
		void exit(Player* player, GameLayer* mainLayer, float time);
	};
	class DeathState : public State {
	public:
		void enter(Player* player, GameLayer* mainLayer, float time);
		State * update(Player* player, GameLayer* mainLayer, float time);
		void exit(Player* player, GameLayer* mainLayer, float time);
	};
	class NoClipState : public State {
	public:
		NoClipState() { type = "noclip"; }
		void enter(Player* player, GameLayer* mainLayer, float time);
		State* handleInput(Player* player, GameLayer* MainLayer, float time, Input input);
		void exit(Player* player, GameLayer* mainLayer, float time);
	};
	//initial state
	State* state = new NeutralState;
	State* newState = NULL;
	State* prevState = NULL;

	//for picking up items
	Node* pickUpRadius;//hitbox for checking if player can pick up an item

	//for crouching (and climbing)
	PhysicsBody* crouchBody;
	Size standSize;
	Size crouchSize;
	bool climbComplete = false;

	//for climbing
	float startClimbTime = -1;
	float startClimbDelay = 0.5f;
	float prevMoveDirection;

	//for rolling
	float rollEndTime = -1;
	float rollLagTime = 30 FRAMES;

	//for jumping
	float startJumpTime = -1;
	float jumpTime = 1.0f;//time you are stuck in a jump for before falling

	//for falling attacks
	bool inFallingAttack = false;

	//for moonwalking
	bool moonwalking = false;
	float prevStopTime = -1;
	float stopDelay = 5 FRAMES;//preventing stop inputs from being registered 5 frames from eachother, give moonwalk sliding a 5 frame window to perform

	//for the timing of attacks/using items:
	bool attackRelease = false;
	bool throwRelease = false;

	//for hiding:
	bool hidden = false;
	bool hittingLeft = false;
	bool hittingRight = false;
	float visionResetTime = 1.0f;//time it takes for inVision to reset to false

	//for walking:
	int moveDirection = 0;

	//for playing audio:
	unsigned walkingSound;

	HideObject* hideObject = NULL;//object player is hiding behind
	PhysObject* climbObject = NULL;

	//animations:
	GameAnimation moonwalk;
	GameAnimation crouchwalk;
	GameAnimation climbing;
	GameAnimation jumping;
	GameAnimation falling;
	GameAnimation rolling;
	GameAnimation crouchstab;
	GameAnimation crouchswing;
	GameAnimation fallstab;
	GameAnimation fallswing;
	//GameAnimation crouchThrowAnimation;
	//GameAnimation hideAnimation;//reverse for uniding

	//indicators:
	Sprite* aimMarker;
};