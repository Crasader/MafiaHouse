#pragma once
#include "Character.h"
#include "Item.h"
#include "Stair.h"
#include "EnvObject.h"
#include "GameLayer.h"
#include "Player.h"

class Enemy : public Character
{
public:
	Enemy();
	~Enemy();
	CREATE_SPRITE_FUNC(Enemy, "guard.png");
	void initObject(Vec2 startPos = Vec2(0,0));//will be deprecated one enemies have animations

	//actions for enemies:
	void walk(float time);//will be deprecated

	bool pathTo(GameLayer* mainLayer, float positionX, int floorNum);//find path to location, return true = reached location
	void moveTo(float positionX);

	void visionRays(vector<Vec2> *points, Vec2* start);//casts a bunch of rays; the enemies vision cone

	void changeSuspicion(float num);//increase/decrease suspicion
	void setSuspicion(float num);//set it to a specific value instantly

	void playerTouch() { isTouched = true; }

	//getters:
	bool seeingPlayer() { return playerInVision; }
	bool checkHit() { return isHit; }

	//for keeping track of player that has been detected by the enemy:
	Player* detectedPlayer = NULL;
	int detectedTag = -1;

	virtual void update(GameLayer* mainLayer, float time);

protected:
	class State {
	public:
		string type = "default";
		virtual ~State() {}
		virtual void enter(Enemy* enemy, GameLayer* mainLayer, float time);
		virtual State* update(Enemy* enemy, GameLayer* mainLayer, float time);
		virtual void exit(Enemy* enemy, GameLayer* mainLayer);
	};
	class DefaultState : public State {
	public:
		void enter(Enemy* enemy, GameLayer* mainLayer, float time);
		State* update(Enemy* enemy, GameLayer* mainLayer, float time);
		//void exit(Enemy* enemy, GameLayer* mainLayer);
	};
	class SuspectState : public State {
	public:
		void enter(Enemy* enemy, GameLayer* mainLayer, float time);
		State * update(Enemy* enemy, GameLayer* mainLayer, float time);
		//void exit(Enemy* enemy, GameLayer* mainLayer);
	};
	class AlertState : public State {
	public:
		AlertState() { type = "alert"; }
		void enter(Enemy* enemy, GameLayer* mainLayer, float time);
		State * update(Enemy* enemy, GameLayer* mainLayer, float time);
		//void exit(Enemy* enemy, GameLayer* mainLayer);
	};
	class UseDoorState : public State {
	public:
		void enter(Enemy* enemy, GameLayer* mainLayer, float time);
		State* update(Enemy* enemy, GameLayer* mainLayer, float time);
		void exit(Enemy* enemy, GameLayer* mainLayer);
	};
	State* state = new DefaultState;
	State* newState = NULL;
	State* prevState = NULL;

	//to check if enemy has been touched by player
	bool isTouched = false;

	//for suspicion level:
	float suspicionLevel = 0;
	float maxSuspicion = 1200;

	//Stuff for Vision Fields:
	bool didRun;
	int visionDegrees = 65;//width of angle of vision
	int visionRadius = 130;//how far vision reaches

	//for keepign track of if enemy spotted player:
	bool playerInVision = false;

	//stuff for walking:
	float walkTime = 6.0f;
	float waitTime = 2.0f;
	float previousTurnTime = -1;
	float stopTime = -1;

	//for returning to starting position:
	bool returning = false;

	//for using doors:
	float doorUsePos;
};
