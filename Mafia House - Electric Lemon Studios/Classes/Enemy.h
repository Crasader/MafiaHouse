#pragma once
#include "GameObject.h"
#include "GameLayer.h"

class Enemy : public GameObject
{
public:
	Enemy();
	~Enemy();
	CREATE_SPRITE_FUNC(Enemy, "guard.png");

	void initObject(Vec2 startPos = Vec2(0,0));

	void update(GameLayer* mainLayer, float time, GameObject* target);

	void walk(float time);

	void chase(Node* target);

	bool pathTo(GameLayer* mainLayer, float positionX, int floorNum);//find path to location, return true = reached location
	void moveTo(float positionX);

	void changeSuspicion(float num);
	void setSuspicion(float num);

	void visionRays(vector<Vec2> *points, Vec2* start);//casts a bunch of rays; the enemies vision cone

	bool seeingPlayer() { return playerInVision; }

	bool touchedPlayer = false;

protected:
	class State {
	public:
		virtual ~State() {}
		virtual void enter(Enemy* enemy, GameLayer* mainLayer, float time);
		virtual State* update(Enemy* enemy, GameLayer* mainLayer, float time, GameObject* target);
		virtual void exit(Enemy* enemy, GameLayer* mainLayer);
	};
	class DefaultState : public State {
	public:
		void enter(Enemy* enemy, GameLayer* mainLayer, float time);
		State* update(Enemy* enemy, GameLayer* mainLayer, float time, GameObject* target);
		//void exit(Enemy* enemy, GameLayer* mainLayer);
	};
	class SuspectState : public State {
	public:
		void enter(Enemy* enemy, GameLayer* mainLayer, float time);
		State * update(Enemy* enemy, GameLayer* mainLayer, float time, GameObject* target);
		//void exit(Enemy* enemy, GameLayer* mainLayer);
	};
	class AlertState : public State {
	public:
		void enter(Enemy* enemy, GameLayer* mainLayer, float time);
		State * update(Enemy* enemy, GameLayer* mainLayer, float time, GameObject* target);
		//void exit(Enemy* enemy, GameLayer* mainLayer);
	};

	State* state = new DefaultState;
	State* newState = NULL;
	State* prevState = NULL;

	float suspicionLevel = 0;
	float maxSuspicion = 1200;

	//Stuff for Vision Fields:
	bool didRun;
	int visionDegrees = 65;//width of angle of vision
	int visionRadius = 130;//how far vision reaches
	
	bool playerInVision = false;

	string visionContactName;
	int visionContactTag;

	//stuff for walking:
	float walkTime = 6.0f;
	float waitTime = 2.0f;
	float previousTurnTime = -1;
	float stopTime = -1;

	float moveSpeed = 1.0f;

	Vec2 initialPos;
	bool returning = false;
};
