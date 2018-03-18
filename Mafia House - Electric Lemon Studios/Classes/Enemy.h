#pragma once
#include "Character.h"
#include "Item.h"
#include "Stair.h"
#include "EnvObject.h"
#include "GameLayer.h"
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
	~Enemy();
	CREATE_SPRITE_FUNC(Enemy, "guard.png");
	void initObject(Vec2 startPos = Vec2(0,0));//will be deprecated one enemies have animations
	void flipX();

	void openDoor();
	void closeDoor();
	void breakDoor(float time);//alert enemies will break down locked doors

	//actions for enemies:
	void pause(float time);
	void turnOnSpot(float time);//enemy stands still and turns around
	void walk(float time);//enemies that do not have a path to follow walk back and forth
	void followPath(GameLayer* mainLayer, float time);

	bool pathTo(GameLayer* mainLayer, float positionX, int floorNum);//find path to location, return true = reached location
	void moveTo(float positionX);
	bool moveToObject(Node* target);

	void visionRays(vector<Vec2> *points, Vec2* start);//casts a bunch of rays; the enemies vision cone

	void changeSuspicion(float num);//increase/decrease suspicion
	void setSuspicion(float num);//set it to a specific value instantly

	void playerTouch() { isTouched = true; }
	void hitWall() { didHitWall = true; }

	//getters:
	bool seeingPlayer() { return playerInVision; }
	bool checkHit() { return isHit; }
	string getPathTag() { return pathTag; }
	void setPathTag(string pathtag) { pathTag = pathtag; }
	
	//for keeping track of player that has been detected by the enemy:
	Player* detectedPlayer = NULL;
	int detectedTag = -1;

	virtual void update(GameLayer* mainLayer, float time);

	vector<PathNode*> pathNodes;

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
	State* state = new DefaultState;
	State* newState = NULL;
	State* prevState = NULL;

	//for suspicion indicators
	Sprite* qMark;
	Sprite* exMark;

	//for going to noises, going to bodies
	GameObject* noiseLocation = NULL;
	GameObject* bodySeen = NULL;
	bool reachedLocation = false;

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
	float turnTime = 6.0f;

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

	//for locking/unlocking doors
	bool hasKey = true;
	//for breakin doors:
	float startBreakTime = -1;
	float breakTime = 8.0f;//time in seconds it takes for an enemy to break down a door

	//for using doors:
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
};
