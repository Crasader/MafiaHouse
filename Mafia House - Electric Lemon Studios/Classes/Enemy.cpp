#include "Enemy.h"
#include "Wall.h"

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

//a recursive function!!! searches for a path to get to a certain floor
Stair* pathSearch(GameLayer* mainLayer, int floorNum, vector<Stair*> stairs) {
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
		pathSearch(mainLayer, mainLayer->getPartnerStair(stairs[i])->startRoom.y, newSearchStairs);
	}
	return nullptr;//fails to find a successful path
}

bool Enemy::pathTo(GameLayer* mainLayer, float positionX, int floorNum) {
	float displacement = positionX - getPosition().x;
	Stair* takeStair = NULL;

	Stair* tempStair;
	vector<Stair*> searchStairs;

	if (floorNum != currentFloor) {//destination is on different floor
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

		if (takeStair == NULL) {
			takeStair = pathSearch(mainLayer, floorNum, searchStairs);
		}

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
		if (getPositionX() + (getContentSize().width / 2) >= positionX - 10 && getPositionX() + (getContentSize().width / 2) <= positionX + 10) {
			return true;
		}
	}
	return false;
}

void Enemy::moveTo(float positionX) {
	float displacement = positionX - getPositionX();
	Vec2 moveDirection;

	if (displacement < 0) {//player is to the left
		moveDirection = Vec2(-1, 0);
		if (flippedX == false) {
			flipX();
		}
	}
	else if (displacement >= 0) {//player is to the right
		moveDirection = Vec2(1, 0);
		if (flippedX == true) {
			flipX();
		}
	}

	moveAbsolute(moveDirection * 4.5 * moveSpeed);
}

void Enemy::visionRays(vector<Vec2> *points, Vec2* start)
{
	detectedTag = -1;
	playerInVision = false;
	didRun = false;

	PhysicsRayCastCallbackFunc func = [this, points](PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data)->bool
	{
		int visionContactTag = info.shape->getBody()->getTag();
		string visionContactName = info.shape->getBody()->getName();

		//enemy vision is blocked by walls, doors
		if (visionContactName == "wall" || visionContactName == "door") {
			points->push_back(info.contact);
			didRun = true;
			return false;
		}
		//enemy sees the player
		else if (visionContactTag == 1){//1 = player, 2 = hidden player
			detectedTag = visionContactTag;
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
		startPoint = getPosition() + Vec2(getContentSize().width - 12, 87);
		*start = startPoint + Vec2(6, 0);//shift visuals forward a bit
		direction = 1;
	}
	else {
		startPoint = getPosition() + Vec2(12, 87);
		*start = startPoint + Vec2(-6, 0);//shift visuals forward a bit
		direction =  -1;
	}

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

void Enemy::update(GameLayer* mainLayer, float time) {
	updateFloor(mainLayer->floors);//checking if floor has changed
	newState = state->update(this, mainLayer, time);
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

//Enemy States:
void Enemy::State::enter(Enemy* enemy, GameLayer* mainLayer, float time) {
}
Enemy::State* Enemy::State::update(Enemy* enemy, GameLayer* mainLayer, float time) {
	return nullptr;
}
Enemy::State* Enemy::State::handleInput(Enemy* enemy, GameLayer* mainLayer, float time, Input input) {
	return nullptr;
}
void Enemy::State::exit(Enemy* enemy, GameLayer* mainLayer) {
}

//Default State:
void Enemy::DefaultState::enter(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->returning = true;
	enemy->moveSpeed = 1.0f;
	enemy->setSpeed(enemy->moveSpeed);
	enemy->setName("enemy");
}
Enemy::State* Enemy::DefaultState::update(Enemy* enemy, GameLayer* mainLayer, float time) {
	//enemy is following default path
	if (enemy->returning == false) {
		//enemy->followPath(time);
		enemy->walk(time);
	}
	//enemy is returining to start position
	else {
		if (enemy->pathTo(mainLayer, enemy->initialPos.x, enemy->startRoom.y) == true) {
			enemy->returning = false;
		}
	}

	//checking if enemy spotted player
	if (enemy->seeingPlayer() == true) {
		enemy->changeSuspicion(enemy->maxSuspicion / (1 SECONDS));//increases 1/60th of max every frame, takes 60 frames to alert guard
	}
	else {
		enemy->changeSuspicion(-enemy->maxSuspicion / (4 SECONDS));//takes 240 frames to drop back to 0 from max
	}
	//check if player bumped enemy
	if (enemy->isTouched == true) {
		enemy->setSuspicion(enemy->maxSuspicion - (enemy->maxSuspicion / (1 SECONDS)));
		enemy->pathTo(mainLayer, enemy->detectedPlayer->getPositionX(), enemy->detectedPlayer->currentFloor);
		enemy->isTouched = false;
		enemy->detectedPlayer = NULL;
	}
	//check if an enemy has become alerted
	if (enemy->suspicionLevel >= enemy->maxSuspicion) {
		enemy->detectedPlayer = static_cast<Player*>(mainLayer->getChildByTag(enemy->detectedTag));
		return new AlertState;
	}
	else if (enemy->suspicionLevel >= enemy->maxSuspicion / 2) {
		//return new SuspectState;
	}
	return nullptr;
}

//Suspicious State:
void Enemy::SuspectState::enter(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->moveSpeed = 1.5f;
	enemy->setSpeed(enemy->moveSpeed);
	enemy->setName("enemy_alert");
}
Enemy::State* Enemy::SuspectState::update(Enemy* enemy, GameLayer* mainLayer, float time) {
	//checking if enemy spotted player
	if (enemy->seeingPlayer() == true) {
		enemy->changeSuspicion(enemy->maxSuspicion / (1 SECONDS));
	}
	else {
		enemy->changeSuspicion(-1 * enemy->maxSuspicion / (10 SECONDS));
	}
	//check if an enemy has become unalerted
	if (enemy->suspicionLevel <= 0) {
		return new DefaultState;
	}
	return nullptr;
}

//Alerted State:
void Enemy::AlertState::enter(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->setSpeed(2.09f);
	enemy->setName("enemy_alert");
}
Enemy::State* Enemy::AlertState::update(Enemy* enemy, GameLayer* mainLayer, float time) {
	//checking if enemy spotted player
	if (enemy->seeingPlayer() == true) {
		enemy->changeSuspicion(enemy->maxSuspicion / (1 SECONDS));
	}
	else {
		enemy->changeSuspicion(-enemy->maxSuspicion / (10 SECONDS));
	}
	//check if an enemy has become unalerted
	if (enemy->suspicionLevel <= 0) {
		return new DefaultState;
	}
	else if (enemy->suspicionLevel <= enemy->maxSuspicion / 2) {
		//return new SuspectState;
	}
	enemy->pathTo(mainLayer,enemy->detectedPlayer->getPositionX(), enemy->detectedPlayer->currentFloor);
	return nullptr;
}