#pragma once
#include "Character.h"
#include "Player.h"

class PathNode : public GameObject {
public:
	CREATE_FUNC(PathNode);
	int xPos;
	string pathTag;//tag coressponding to enemy with same pathTag
	int num;//number of node in sequence
	float waitTime;//amount of time enemy will pause at node's location, in seconds
	void initNode(int pos, int number, float wait_time, string tag) {
		xPos = pos;
		pathTag = tag;
		num = number;
		waitTime = wait_time;
	}
};

class Enemy : public Character
{
public:
	Enemy();
	~Enemy() {}
	CREATE_SPRITE_FUNC(Enemy, "guard.png");
	CREATE_WITH_FRAME(Enemy);
	CREATE_WITH_FRAME_NAME(Enemy, "enemy/thug/stand/001.png");
	void initObject(Vec2 startPos = Vec2(0,0));//will be deprecated one enemies have animations
	void initJoints();//initilaizing joints for physics bodies
	void flipX();
	Vec2 getPosition();

	void pickUpItem(GameLayer* mainLayer);
	void dropInventory(GameLayer* mainLayer);

	void openDoor();
	void closeDoor();

	//actions for enemies:
	void pause(float time);
	void turnOnSpot(float time);//enemy stands still and turns around
	void walk(float time);//enemies that do not have a path to follow walk back and forth
	void followPath(GameLayer* mainLayer, float time);

	Stair* pathSearch(GameLayer* mainLayer, vector<Stair*> stairs, float xPos, bool(*checkPathFunc)(GameLayer* mainlayer, int floorNum, int targetX, int searcherX, bool hasKey));
	bool pathTo(GameLayer* mainLayer, float positionX, int floorNum, int roomNum, float time, bool(*checkPathFunc)(GameLayer* mainlayer, int floorNum, int targetX, int searcherX, bool hasKey));//find path to location, return true = reached location
	void moveFrom(float positionX);
	void moveTo(float positionX);
	bool moveToObject(Node* target);
	bool moveToDoor(Node* target);
	void runaway(GameLayer* mainlayer, float time);
	Item* findClosestItem(GameLayer* mainLayer);
	Item* findBetterItem(GameLayer* mainLayer);
	Item* findMoreRange(GameLayer* mainLayer);

	void visionRays(vector<Vec2> *points, Vec2* start, float time);//casts a bunch of rays; the enemies vision cone

	void changeSuspicion(float num);//increase/decrease suspicion
	void setSuspicion(float num);//set it to a specific value instantly

	void playerTouch() { isTouched = true; }
	void hitWall() { didHitWall = true; }

	void gotHit(Item* item, float time);//function for when enemy is hit by player's attack
	bool isReallyDead() { return isDead; }
	bool checkKey() { return hasKey; }

	//getters:
	bool seeingPlayer() { return playerInVision; }
	string getPathTag() { return pathTag; }
	void setPathTag(string pathtag) { pathTag = pathtag; }
	
	//for keeping track of player that has been detected by the enemy:
	Player* detectedPlayer = NULL;
	int detectedTag = -1;
	Node* lastSeenLocation;

	//for going to noises
	GameObject* noiseLocation = NULL;
	//for seeing bodies/ knocked out dudes
	GameObject* bodySeen = NULL;

	//list of enemies
	vector<Enemy*> enemies;

	vector<PathNode*> pathNodes;

	virtual void update(GameLayer* mainLayer, float time);

protected:
	class State {
	public:
		string type = "default";
		virtual ~State() {}
		virtual void enter(Enemy* enemy, GameLayer* mainLayer, float time);
		virtual State* update(Enemy* enemy, GameLayer* mainLayer, float time);
		virtual void exit(Enemy* enemy, GameLayer* mainLayer, float time);
	};
	class DefaultState : public State {
	public:
		void enter(Enemy* enemy, GameLayer* mainLayer, float time);
		State* update(Enemy* enemy, GameLayer* mainLayer, float time);
		//void exit(Enemy* enemy, GameLayer* mainLayer, float time);
	};
	class SuspectState : public State {
	public:
		void enter(Enemy* enemy, GameLayer* mainLayer, float time);
		State * update(Enemy* enemy, GameLayer* mainLayer, float time);
		void exit(Enemy* enemy, GameLayer* mainLayer, float time);
	};
	class AlertState : public State {
	public:
		AlertState() { type = "alert"; }
		void enter(Enemy* enemy, GameLayer* mainLayer, float time);
		State * update(Enemy* enemy, GameLayer* mainLayer, float time);
		void exit(Enemy* enemy, GameLayer* mainLayer, float time);
	};
	class AttackState : public State {
	public:
		AttackState() { type = "attack"; }
		void enter(Enemy* enemy, GameLayer* mainLayer, float time);
		State* update(Enemy* enemy, GameLayer* mainLayer, float time);
		void exit(Enemy* enemy, GameLayer* mainLayer);
	};
	class UseDoorState : public State {
	public:
		UseDoorState() { type = "use_door"; }
		void enter(Enemy* enemy, GameLayer* mainLayer, float time);
		State* update(Enemy* enemy, GameLayer* mainLayer, float time);
		void exit(Enemy* enemy, GameLayer* mainLayer, float time);
	};
	class GetItemState : public State {
	public:
		void enter(Enemy* enemy, GameLayer* mainLayer, float time);
		State* update(Enemy* enemy, GameLayer* mainLayer, float time);
		void exit(Enemy* enemy, GameLayer* mainLayer, float time);
	};
	class SearchState : public State {
	public:
		void enter(Enemy* enemy, GameLayer* mainLayer, float time);
		State * update(Enemy* enemy, GameLayer* mainLayer, float time);
		void exit(Enemy* enemy, GameLayer* mainLayer, float time);
	};
	class SeenBodyState : public State {
	public:
		void enter(Enemy* enemy, GameLayer* mainLayer, float time);
		State * update(Enemy* enemy, GameLayer* mainLayer, float time);
		void exit(Enemy* enemy, GameLayer* mainLayer, float time);
	};
	class KnockOutState : public State {
	public:
		void enter(Enemy* enemy, GameLayer* mainLayer, float time);
		State * update(Enemy* enemy, GameLayer* mainLayer, float time);
		void exit(Enemy* enemy, GameLayer* mainLayer, float time);
	};
	class DeathState : public State {
	public:
		void enter(Enemy* enemy, GameLayer* mainLayer, float time);
		State * update(Enemy* enemy, GameLayer* mainLayer, float time);
		void exit(Enemy* enemy, GameLayer* mainLayer, float time);
	};
	State* state = new DefaultState;
	State* newState = NULL;
	State* prevState = NULL;
	State* toEnter = NULL;

	//animations:
	GameAnimation knockout;
	//GameAnimation sleeping;

	//for suspicion indicators
	Sprite* qMark;
	Sprite* exMark;
	Sprite* ZZZ;
	GameAnimation ZZZAnimation;

	//for pathfinding:
	vector<Stair*> prevSearched;
	vector<float> pathLengths;
	int shortestDepth;
	int depth = -1;
	bool firstEndFound = false;

	//for running away
	bool canRunAway = true;
	Stair* prevUsedStair = NULL;
	float prevUsedStairTime = -1;
	float prevUsedStairWaitTime = 5.0f;

	//for going to items while alerted:
	bool goingToFirstItem = false;
	bool goingToBetterItem = false;
	bool goingToMoreRange = false;

	//for seeing items:
	vector<Item*> seenItems;
	vector<float> seenTimes;
	float previousForgetTime = -1;
	float memoryTime = 45.0f;//number of seconds they remember seeing an item for

	//for locking/unlocking doors
	bool hasKey = false;

	//for attacking the player
	float distanceToPlayer;
	bool inAttackRange;
	//for attacking without a weapon
	Fist* fist;

	//for going to noises, going to bodies
	bool reachedLocation = false;

	//for being hit:
	float invicibilityTime = 0.5f;
	float hitTime = -1;
	bool invincible = false;

	//for being knocked out
	//int knockOutHP = 2;
	PhysicsBody* knockedOutBody;
	bool knockedOut = false;
	bool visionEnabled = true;
	float startKockOutTime = -1.0f;
	float baseKnockOutTime = 6.0f;
	float minKnockOuttime = 8.0f;
	float knockOutTime;

	//to check if enemy has been touched by player
	bool isTouched = false;

	//for suspicion level:
	float suspicionLevel = 0;
	float maxSuspicion = 1200;

	//Stuff for Vision Fields:
	bool didRun;
	int defaultDegrees = 65;
	int visionDegrees = defaultDegrees;//width of angle of vision
	int defaultRadius = 150;
	int visionRadius = defaultRadius;//how far vision reaches

	//for keepign track of if enemy spotted player:
	bool playerInVision = false;

	//check if enemy has walked into a wall
	bool didHitWall = false;
	float hitWalltime = -1;
	float hitWallDelay = 3;//time between wall hits to prevent continuous hitting

	//stuff for walking:
	float walkTime = 6.0f;
	float waitTime = 2.0f;
	float previousTurnTime = -1;
	float stopTime = -1;
	//for turning on spot
	float turnTime = 5.0f;

	//for pausing temporaroly
	bool paused = false;
	float pauseTime = 0;//how much time they were paused for
	float startPauseTime = -1;//when they paused
	float timeToPauseFor = 3.0f;//time in seconds to pause for
	bool wasFlipped;

	//for random pausing
	float prevPauseTime = -4;
	float minPauseInterval = 3.0f;//minimum time between random pauses

	//for returning to starting position:
	bool returning = false;

	//for running into locked doors
	float startWaitTime = -1;
	float doorWaitTime = 2.0f;
	//for breakin doors:
	float startBreakTime = -1;
	float breakTime = 6.0f;//time in seconds it takes for an enemy to break down a door
	float distanceToDoor;

	//for using doors:
	bool openedDoor = false;
	float doorUsePos;
	float doorUseTime;
	float doorStartTime = -1;
	//for picking up items:
	float itemPickupTime = -1;
	float pickupStartTime;

	//for following path:
	string pathTag = "NONE";
	int pathNum = 1;
	bool reachedNode = false;
	float reachedNodeTime = -1;
	int pathIterator = 1;

	//for chasing player
	bool lostPlayer = false;
	bool reachedLastSeen = false;
};
