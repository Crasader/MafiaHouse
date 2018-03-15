#include "Enemy.h"

Enemy::Enemy()
{
	name = "enemy";
	tag = 30000;//enemies will be 30000 - 39999
	//sprite properties
	zOrder = 4;
	scale = 1.0f;
	//physics body properties
	dynamic = true;
	category = 4;
	collision = 11;

	maxSpeed = 50;

	suspicionLevel = 0;
}
Enemy::~Enemy(){
}

void Enemy::initObject(Vec2 startPos)
{
	GameObject::initObject(startPos);
}

void Enemy::walk(float time) {
	if (previousTurnTime == -1) {
		previousTurnTime = time;
	}

	if (time - previousTurnTime >= walkTime && stopTime == -1) {
		slowStop();
		stopTime = time;
	}
	else if (stopTime != -1 && time - stopTime >= waitTime) {
		flipX();
		previousTurnTime = time;
		stopTime = -1;
	}
	else if (stopTime == -1){
		move(Vec2(4.5f, 0));
	}
}

void Enemy::chase(Node* target) {
	Vec2 displacement = getPosition() - target->getPosition();
	Vec2 moveDirection = displacement.getNormalized();

	if (displacement.x < 0) {//player is to the left
		if (flippedX == true) {
			flipX();
		}
	}
	else if (displacement.x >= 0) {//player is to the lright
		if (flippedX == false) {
			flipX();
		}
	}

	move(moveDirection * 8);

	/*if (flippedX == true) {
		move(moveDirection * 8);
	}
	else {
		move(moveDirection * -8);
	}*/
}

void Enemy::visionRays(vector<Vec2> *points, Vec2* start)
{
	playerInVision = false;

	didRun = false;
	PhysicsRayCastCallbackFunc func = [this, points](PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data)->bool
	{
		visionContactTag = info.shape->getBody()->getTag();
		visionContactName = info.shape->getBody()->getName();

		//enemy vision is blocked by walls, doors
		if (visionContactName == "wall" || visionContactName == "door") {
			points->push_back(info.contact);
			didRun = true;
			return false;
		}
		//enemy sees the player
		else if (visionContactTag == 1){//1 = player, 2 = hidden player
			playerInVision = true;
			points->push_back(info.contact);
			didRun = true;
			return false;
		}
		//things to ingore collisions with
		else {
			return true;
		}
	};

	Vec2 startPoint;
	Vec2 endPoint;
	int direction;

	if (flippedX == false) {
		startPoint = getPosition() + Vec2(getContentSize().width - 5, 87);
		direction = 1;
	}
	else {
		startPoint = getPosition() + Vec2(5, 87);
		direction =  -1;
	}
	*start = startPoint;

	float angle = 0 - (visionDegrees / 2);
	for (int i = 0; i < visionDegrees; i++) {
		endPoint.x = startPoint.x + cosf((angle + i) * M_PI / 180) * visionRadius * direction;
		endPoint.y = startPoint.y + sinf((angle + i) * M_PI / 180) * visionRadius;
		director->getRunningScene()->getPhysicsWorld()->rayCast(func, startPoint, endPoint, nullptr);
		if (didRun == false) {
			points->push_back(endPoint);
		}
		didRun = false;
	}
}

void Enemy::changeSuspicion(int num) {
	suspicionLevel += num;
	suspicionLevel = suspicionLevel > 600 ? 600 : suspicionLevel;
	suspicionLevel = suspicionLevel < 0 ? 0 : suspicionLevel;
}

void Enemy::setSuspicion(int num) {
	suspicionLevel = num;
}

//Update Checking:
void Enemy::update(GameLayer* mainLayer, float time, Node* target) {
	newState = state->update(this, mainLayer, time, target);
	if (newState != NULL)
	{
		state->exit(this, mainLayer);

		if (prevState != NULL && newState != prevState) { delete prevState; }
		prevState = state;
		state = newState;
		newState = NULL;

		state->enter(this, mainLayer, time);
	}
}

//Enemy States
void Enemy::State::enter(Enemy* enemy, GameLayer* mainLayer, float time) {
}
Enemy::State* Enemy::State::update(Enemy* enemy, GameLayer* mainLayer, float time, Node* target) {
	return nullptr;
}
void Enemy::State::exit(Enemy* enemy, GameLayer* mainLayer) {
}

//Default State
void Enemy::DefaultState::enter(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->setSpeed(1.0f);
	enemy->setName("enemy");
}
Enemy::State* Enemy::DefaultState::update(Enemy* enemy, GameLayer* mainLayer, float time, Node* target) {
	//checking if enemy spotted player
	if (enemy->seeingPlayer() == true) {
		enemy->changeSuspicion(10);
	}
	else {
		enemy->changeSuspicion(-2);
	}
	//check if an enemy has become alerted
	if (enemy->suspicionLevel >= 600) {
		return new AlertState;
	} 
	enemy->walk(time);
	if (enemy->touchedPlayer == true) {
		enemy->setSuspicion(580);
		enemy->chase(target);
		enemy->touchedPlayer = false;
	}
	return nullptr;
}

//Alert State
void Enemy::AlertState::enter(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->setSpeed(2.0f);
	enemy->setName("enemy_alert");
}
Enemy::State* Enemy::AlertState::update(Enemy* enemy, GameLayer* mainLayer, float time, Node* target) {
	//checking if enemy spotted player
	if (enemy->seeingPlayer() == true) {
		enemy->changeSuspicion(10);
	}
	else {
		enemy->changeSuspicion(-2);
	}
	//check if an enemy has become unalerted
	if (enemy->suspicionLevel <= 0) {
		return new DefaultState;
	}
	enemy->chase(target);
	return nullptr;
}