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
	initialPos = startPos;
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
	Vec2 displacement = target->getPosition() - getPosition();
	Vec2 moveDirection = displacement.getNormalized();

	if (displacement.x < 0) {//player is to the left
		if (flippedX == false) {
			flipX();
		}
	}
	else if (displacement.x >= 0) {//player is to the right
		if (flippedX == true) {
			flipX();
		}
	}

	moveAbsolute(moveDirection * 4.5 * moveSpeed);
}
//a recursive function!!! searches for a path to get to a certain floor
Stair* search(GameLayer* mainLayer, int floorNum, vector<Stair*> stairs) {
	Stair* tempStair;
	vector<Stair*> newSearchStairs;

	for (int i = 0; i < stairs.size(); i++) {//searches for stair to take to get to a certain floor
		for (int j = 0; j < mainLayer->stairs.size(); j++) {//each loop checks for a partner stair that is on same floor as enemy
			if (mainLayer->stairs[j]->startRoom.y == mainLayer->getPartnerStair(stairs[i])->startRoom.y) {//stair is on same floor as the partner stair
				if (mainLayer->getPartnerStair(mainLayer->stairs[j])->startRoom.y == floorNum) {//partner stair of this stair is on same floor as player
					return mainLayer->getPartnerStair(mainLayer->stairs[j]);//take this stair
				}
				else {
					tempStair = mainLayer->stairs[i];
					newSearchStairs.push_back(tempStair);
				}
			}
		}
		search(mainLayer, mainLayer->getPartnerStair(stairs[i])->startRoom.y, newSearchStairs);
	}
	return nullptr;//fails to find a successful path
}

void Enemy::pathTo(GameLayer* mainLayer, float positionX, int floorNum) {
	float displacement = positionX - getPosition().x;
	Stair* takeStair = NULL;

	Stair* tempStair;
	vector<Stair*> searchStairs;

	if (floorNum != startRoom.y) {//destination is on different floor

		for (int i = 0; i < mainLayer->stairs.size(); i++) {
			if (mainLayer->stairs[i]->startRoom.y == floorNum) {//stair is on destination floor
				if (mainLayer->getPartnerStair(mainLayer->stairs[i])->startRoom.y == currentFloor) {//partner stair is on your floor
					takeStair = mainLayer->getPartnerStair(mainLayer->stairs[i]);//take this stair
				}
				else {
					tempStair = mainLayer->stairs[i];
					searchStairs.push_back(tempStair);
				}
			}
		}

		takeStair = search(mainLayer, floorNum, searchStairs);

		//if they are not at location of stairs yet:
		if (!(getPositionX() + getContentSize().width >= takeStair->getPositionX() && getPositionX() <= takeStair->getPositionX() + takeStair->getContentSize().width)) {
			moveTo(takeStair->getPositionX());//move towards stairs
		}
		//if they are at location of stairs:
		else {
			takeStair->use(this, mainLayer);//use the stairs
		}
	}
	else {//already on the right floor
		moveTo(positionX);
	}
}

void Enemy::moveTo(float positionX) {
	float displacement = positionX - getPositionX();
	Vec2 moveDirection = Vec2(displacement / displacement, 0);

	if (displacement < 0) {//player is to the left
		if (flippedX == false) {
			flipX();
		}
	}
	else if (displacement >= 0) {//player is to the right
		if (flippedX == true) {
			flipX();
		}
	}

	moveAbsolute(moveDirection * 4.5 * moveSpeed);
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

void Enemy::changeSuspicion(float num) {
	suspicionLevel += num;
	suspicionLevel = suspicionLevel > maxSuspicion ? maxSuspicion : suspicionLevel;
	suspicionLevel = suspicionLevel < 0 ? 0 : suspicionLevel;
}

void Enemy::setSuspicion(float num) {
	suspicionLevel = num;
}

//Update Checking:
void Enemy::update(GameLayer* mainLayer, float time, Node* target) {
	updateFloor(mainLayer);
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
	enemy->moveSpeed = 1.0f;
	enemy->setSpeed(enemy->moveSpeed);
	enemy->setName("enemy");
}
Enemy::State* Enemy::DefaultState::update(Enemy* enemy, GameLayer* mainLayer, float time, Node* target) {
	//checking if enemy spotted player
	if (enemy->seeingPlayer() == true) {
		enemy->changeSuspicion(enemy->maxSuspicion / 1 SECONDS);//increases 1/60th of max every frame, takes 60 frames to alert guard
	}
	else {
		enemy->changeSuspicion(-enemy->maxSuspicion / 4 SECONDS);//takes 240 frames to drop back to 0 from max
	}
	//check if an enemy has become alerted
	if (enemy->suspicionLevel >= enemy->maxSuspicion) {
		return new AlertState;
	}
	else if (enemy->suspicionLevel >= enemy->maxSuspicion / 2) {
		//return new SuspectState;
	}

	if (enemy->returning == false) {//enemy is following default path
		//enemy->followPath(time);
		enemy->walk(time);
	}
	else {//enemy is returining to start position
		//enemy->moveTo(enemy->initialPos);
	}

	if (enemy->touchedPlayer == true) {
		enemy->setSuspicion(enemy->maxSuspicion - (enemy->maxSuspicion / 1 SECONDS));
		enemy->chase(target);
		enemy->touchedPlayer = false;
	}
	return nullptr;
}

//Suspect State
void Enemy::SuspectState::enter(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->moveSpeed = 2.0f;
	enemy->setSpeed(enemy->moveSpeed);
	enemy->setName("enemy_alert");
}
Enemy::State* Enemy::SuspectState::update(Enemy* enemy, GameLayer* mainLayer, float time, Node* target) {
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

//Alert State
void Enemy::AlertState::enter(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->setSpeed(2.0f);
	enemy->setName("enemy_alert");
}
Enemy::State* Enemy::AlertState::update(Enemy* enemy, GameLayer* mainLayer, float time, Node* target) {
	//checking if enemy spotted player
	if (enemy->seeingPlayer() == true) {
		enemy->changeSuspicion(enemy->maxSuspicion / 1 SECONDS);
	}
	else {
		enemy->changeSuspicion(-enemy->maxSuspicion / 4 SECONDS);
	}
	//check if an enemy has become unalerted
	if (enemy->suspicionLevel <= 0) {
		return new DefaultState;
	}
	else if (enemy->suspicionLevel >= enemy->maxSuspicion / 2) {
		//return new SuspectState;
	}
	enemy->chase(target);
	return nullptr;
}