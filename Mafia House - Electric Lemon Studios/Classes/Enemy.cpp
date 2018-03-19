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
	category = 2;
	collision = 13;

	maxSpeed = 50;
}
Enemy::~Enemy(){
}

void Enemy::flipX() {
	GameObject::flipX();
	if (flippedX == true) {
		qMark->setFlippedX(true);
	}
	else {
		qMark->setFlippedX(false);
	}
}

void Enemy::initObject(Vec2 startPos)
{
	GameObject::initObject(startPos);
	Texture2D::TexParams texParams = { GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };
	exMark = Sprite::create("exMark.png");
	exMark->setAnchorPoint(Vec2(0.5, 0));
	exMark->setPositionNormalized(Vec2(0.5, 1.05));
	exMark->getTexture()->setTexParameters(texParams);
	exMark->setVisible(false);
	addChild(exMark);
	qMark = Sprite::create("qMark.png");
	qMark->setAnchorPoint(Vec2(0.5, 0));
	qMark->setPositionNormalized(Vec2(0.5, 1.05));
	qMark->getTexture()->setTexParameters(texParams);
	qMark->setVisible(false);
	addChild(qMark);
	if (pathTag == "STAND_LEFT" || pathTag == "LEFT") {
		flipX();
	}
}

void Enemy::openDoor() {
	if (hasKey == true) {
		doorToUse->unlock();//try to unlock all doors, nothing happens if it isn't locked
	}
	doorToUse->use();
}
void Enemy::closeDoor() {
	doorToUse->use();
	if (hasKey == true) {
		if (doorToUse->defaultLocked == true) {//only lock doors that are set to be locked in the level
			doorToUse->lock();
		}
	}
}

void Enemy::breakDoor(float time) {
	/*if (startBreakTime == -1) {
		startBreakTime = time;
	}
	if (time - startBreakTime >= breakTime) {
		doorToUse->breakDoor();//break the door after a certain amount of time
		doorToUse->use();
		startBreakTime = -1;
	}*/
	//if (heldItem != NULL) {
	//	doorToUse->itemHit(heldItem);
	//}
	//else {
		doorToUse->hp = doorToUse->hp - (10.0f / (6.0f SECONDS));
	//}

	if (doorToUse->hp <= 0) {
		doorToUse->breakDoor();
		doorToUse->use();
	}
}

void Enemy::pause(float time) {
	if (startPauseTime == -1) {
		stop();
		startPauseTime = time;
	}
	if (startPauseTime != -1 && time - startPauseTime >= timeToPauseFor) {
		paused = false;
		prevPauseTime = time;
		startPauseTime = -1;
		pauseTime = time - startPauseTime;
		//adding pausetime to other time trackers
		if (previousTurnTime != -1) {
			previousTurnTime += pauseTime;
		}
		if (stopTime != -1 && stopTime != -3) {
			stopTime += pauseTime;
		}
		//setting enemy back to direction they were facing before they paused
		if (pathTag == "NONE" || pathTag == "LEFT") {
			if (wasFlipped == true && flippedX == false) {
				flipX();
			}
			else if (wasFlipped == false && flippedX == true) {
				flipX();
			}
		}
	}
}

void Enemy::turnOnSpot(float time) {
	if (previousTurnTime == -1) {
		previousTurnTime = time;
	}
	if (time - previousTurnTime >= turnTime) {
		flipX();
		previousTurnTime = time;
	}
}

void Enemy::walk(float time) {
	//check if they hit a wall and make them turn around right away
	if (didHitWall == true) {
		if (hitWalltime == -1) {
			hitWalltime = time;
			stopTime = -3;//so they will turn straight away
		}
		else if (hitWalltime != -1 && time - hitWalltime >= hitWallDelay) {
			stopTime = -3;//only happens again after delay interval
			hitWalltime = time;
		}
	}

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

void Enemy::followPath(GameLayer* mainLayer, float time) {
	if (reachedNode == false) {
		if (pathTo(mainLayer, pathNodes.at(pathNum)->getPositionX(), pathNodes.at(pathNum)->startRoom.y) == true) {//if player has reached current path node
			slowStop();
			reachedNode = true;
			reachedNodeTime = time;
		}
	}
	else if (reachedNode == true){//reached node == true
		if (((time - reachedNodeTime) >= (pathNodes.at(pathNum)->waitTime)) && (pathNum < (pathNodes.size() - 1)) && (pathNum > 0)) {//if this is not the last node in the list
			pathNum+= pathIterator;
			reachedNode = false;
			reachedNodeTime = -1;
		}
		else if (time - reachedNodeTime >= pathNodes.at(pathNum)->waitTime && pathNum >= (pathNodes.size() - 1)){//if the node reached is the last node in the list
			pathIterator = -1;//make it so you start going backwards through the list
			pathNum += pathIterator;
			reachedNode = false;
			reachedNodeTime = -1;
		}
		else if (time - reachedNodeTime >= pathNodes.at(pathNum)->waitTime && pathNum <= 0) {//if the node reached is the first
			pathIterator = 1;//make it so you start going forwards through the list
			pathNum += pathIterator;
			reachedNode = false;
			reachedNodeTime = -1;
		}
	}
}

//a recursive function!!! searches for a path to get to a certain floor
Stair* pathSearch(GameLayer* mainLayer, int currentFloor, vector<Stair*> stairs) {
	Stair* tempStair;
	vector<Stair*> newSearchStairs;

	for (int i = 0; i < stairs.size(); i++) {//searches for stair to take to get to a certain floor
		for (int j = 0; j < mainLayer->stairs.size(); j++) {//each loop checks for a partner stair that is on same floor as enemy
			if (mainLayer->stairs[j]->startRoom.y == mainLayer->getPartnerStair(stairs[i])->startRoom.y) {//stair is on same floor as the partner stair
				if (mainLayer->getPartnerStair(mainLayer->stairs[j])->startRoom.y == currentFloor) {//partner stair of this stair is on same floor as target
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
			takeStair = pathSearch(mainLayer, currentFloor, searchStairs);
		}

		if (takeStair != NULL) {
			//if they are not at location of stairs yet:
			if (!(getPositionX() + getContentSize().width >= takeStair->getPositionX() && getPositionX() <= takeStair->getPositionX() + takeStair->getContentSize().width)) {
				moveTo(takeStair->getPositionX());//move towards stairs
			}
			//if they are at location of stairs:
			else {
				takeStair->use(this, mainLayer);//use the stairs
			}
		}
	}
	else {//already on the right floor
		moveTo(positionX);
		if (getPositionX() + (getContentSize().width / 2) >= positionX - 50 && getPositionX() + (getContentSize().width / 2) <= positionX + 50) {
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

bool Enemy::moveToObject(Node* target) {
	if (!(getPositionX() + getContentSize().width >= target->getPositionX() && getPositionX() <= target->getPositionX() + target->getContentSize().width)) {
		moveTo(target->getPositionX());//move towards stairs
		return false;
	}
	return true;
}

void Enemy::visionRays(vector<Vec2> *points, Vec2* start)
{
	detectedTag = -1;
	playerInVision = false;
	didRun = false;

	PhysicsRayCastCallbackFunc func = [this, points](PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data)->bool
	{
		int visionContactTag = info.shape->getBody()->getNode()->getTag();
		string visionContactName = info.shape->getBody()->getNode()->getName();
		Node* visionContact = info.shape->getBody()->getNode();

		//enemy vision is blocked by walls, doors
		if (visionContactName == "wall" || visionContactName == "floor" || visionContactName == "door") {
			points->push_back(info.contact);
			didRun = true;
			return false;
		}
		//vision blocked by other enemies
		if ((visionContactTag != getTag()) && (visionContactName == "enemy" || visionContactName == "enemy_alert")) {
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
		//enemy sees an item
		else if (visionContactName == "item" && state->type != "alert") {//enemy won't pick up seen items when alert
			if (static_cast<Item*>(visionContact)->enemyCanUse == true) {
				itemToPickUp = static_cast<Item*>(visionContact);//enemy can pick item up
				return false;
			}
			return true;//enemy cannot pick item up
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
	//updateFloor(mainLayer->floors);//checking if floor has changed
	newState = state->update(this, mainLayer, time);
	if (newState != NULL)
	{
		state->exit(this, mainLayer, time);

		if (prevState != NULL && newState != prevState) { delete prevState; }
		prevState = state;
		state = newState;
		newState = NULL;

		state->enter(this, mainLayer, time);
	}
	//resetting collision checks:
	didHitWall = false;
	itemToPickUp = NULL;
	//updating color of question mark, turns more red as suspicion increases
	float BluePercentage = suspicionLevel / maxSuspicion;
	float GreenPercentage = abs(BluePercentage - 1);//inverts the percentage
	qMark->setColor(ccc3(255, 255 * GreenPercentage, 255 * BluePercentage));
}

//Enemy States:
void Enemy::State::enter(Enemy* enemy, GameLayer* mainLayer, float time) {
}
Enemy::State* Enemy::State::update(Enemy* enemy, GameLayer* mainLayer, float time) {
	return nullptr;
}
void Enemy::State::exit(Enemy* enemy, GameLayer* mainLayer, float time) {
}

//Default State:
void Enemy::DefaultState::enter(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->exMark->setVisible(false);
	enemy->paused = false;
	enemy->moveSpeed = 1.0f;
	enemy->setSpeed(enemy->moveSpeed);
	enemy->setName("enemy");
	enemy->visionDegrees = enemy->defaultDegrees;
	enemy->visionRadius = enemy->defaultRadius;
}
Enemy::State* Enemy::DefaultState::update(Enemy* enemy, GameLayer* mainLayer, float time) {
	//check if enemy is walking into a door
	if (enemy->doorToUse != NULL) {
		return new UseDoorState;
	}
	//check if enemy has seen an item
	if (enemy->itemToPickUp != NULL) {
		return new GetItemState;
	}
	//checking if enemy spotted player
	if (enemy->seeingPlayer() == true) {
		enemy->changeSuspicion(enemy->maxSuspicion / (0.6f SECONDS));//increases 1/45th of max every frame, takes 45 frames to alert guard
	}
	else {
		enemy->changeSuspicion(-enemy->maxSuspicion / (60 SECONDS));//takes 30 seconds to drop from half to 0
	}
	//check if player bumped enemy
	if (enemy->isTouched == true) {
		enemy->startPauseTime = -1;
		enemy->timeToPauseFor = 4.0f;
		enemy->paused = true;
		enemy->wasFlipped = enemy->flippedX;
		enemy->setSuspicion(enemy->maxSuspicion / 1.75f);
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
		return new SuspectState;
	}
	//setting visibility of question mark
	if (enemy->suspicionLevel > 0) {
		enemy->qMark->setVisible(true);
	}
	else {
		enemy->qMark->setVisible(false);
	}

	//Default Movement:
	if (enemy->paused != true) {
		//enemy has no path to follow
		if (enemy->pathTag == "NONE" || enemy->pathTag == "LEFT") {
			if (enemy->returning == false) {
				enemy->walk(time);
			}
			else {
				if (enemy->pathTo(mainLayer, enemy->initialPos.x, enemy->startRoom.y) == true) {
					enemy->returning = false;
				}
			}
		}
		//enemy does not move
		else if (enemy->pathTag == "STAND_LEFT" || enemy->pathTag == "STAND_RIGHT" || enemy->pathTag == "STAND_SWITCH") {
			if (enemy->returning == true) {
				if (enemy->pathTo(mainLayer, enemy->initialPos.x, enemy->startRoom.y) == true) {
					enemy->returning = false;
					enemy->slowStop();
					if (enemy->pathTag == "STAND_LEFT") {
						if (enemy->flippedX == false) { enemy->flipX(); }
					}
					else if (enemy->pathTag == "STAND_RIGHT") {
						if (enemy->flippedX == true) { enemy->flipX(); }
					}
				}
			}
			else {//not returning
				if (enemy->pathTag == "STAND_SWITCH") {
					enemy->turnOnSpot(time);
				}
			}
		}
		//enemy has a path to follow
		else {
			enemy->returning = false;
			enemy->followPath(mainLayer, time);
		}
	}
	else {
		enemy->pause(time);
	}
	return nullptr;
}

//Suspicious State:
void Enemy::SuspectState::enter(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->prevPauseTime = time;
	enemy->exMark->setVisible(false);
	enemy->qMark->setVisible(true);
	enemy->moveSpeed = 1.65f;
	enemy->setSpeed(enemy->moveSpeed);
	enemy->walkTime *= 0.65f;
	enemy->waitTime *= 0.65f;
	enemy->setName("enemy");
	enemy->turnTime *= 0.65f;
	enemy->visionDegrees = enemy->defaultDegrees * 1.1;
	enemy->visionRadius = enemy->defaultRadius * 1.3;
}
Enemy::State* Enemy::SuspectState::update(Enemy* enemy, GameLayer* mainLayer, float time) {
	//check if enemy is walking into a door
	if (enemy->doorToUse != NULL) {
		return new UseDoorState;
	}
	//check if enemy has seen an item
	if (enemy->itemToPickUp != NULL) {
		return new GetItemState;
	}
	//check if enemy has heard a noise
	if (enemy->noiseLocation != NULL) {
		return new SearchState;
	}
	//checking if enemy spotted player
	if (enemy->seeingPlayer() == true) {
		enemy->changeSuspicion(enemy->maxSuspicion / (1.1f SECONDS));
	}
	else {
		enemy->changeSuspicion(-1 * enemy->maxSuspicion / (22 SECONDS));
	}
	//check if player bumped enemy
	if (enemy->isTouched == true) {
		enemy->timeToPauseFor = 5.0f;
		enemy->paused = true;
		enemy->wasFlipped = enemy->flippedX;
		enemy->pathTo(mainLayer, enemy->detectedPlayer->getPositionX(), enemy->detectedPlayer->currentFloor);
		enemy->changeSuspicion(enemy->maxSuspicion / (22 SECONDS));
		enemy->isTouched = false;
		enemy->detectedPlayer = NULL;
	}
	//check if an enemy has become alerted
	if (enemy->suspicionLevel >= enemy->maxSuspicion) {
		enemy->detectedPlayer = static_cast<Player*>(mainLayer->getChildByTag(enemy->detectedTag));
		return new AlertState;
	}
	//check if an enemy has become unalerted
	else if (enemy->suspicionLevel <= 0) {
		return new DefaultState;
	}
	//random chance of turning around and pausing
	if (enemy->paused == false && time - enemy->prevPauseTime >= enemy->minPauseInterval) {
		if (randNum(1, 100) % 25 == 0) {//4% chance to stop, 4/100 nums are divisible by 25
			enemy->wasFlipped = enemy->flippedX;
			enemy->timeToPauseFor = static_cast<float>(randNum(1, 6));//will pause for between 1 and 6 seconds
			enemy->startPauseTime = -1;
			enemy->paused = true;
			enemy->flipX();
		}
	}

	//Same as Default Movement
	if (enemy->paused != true) {
		//enemy has no path to follow
		if (enemy->pathTag == "NONE" || enemy->pathTag == "LEFT") {
			enemy->walk(time);
		}
		//enemy does not move
		else if (enemy->pathTag == "STAND_LEFT" || enemy->pathTag == "STAND_RIGHT" || enemy->pathTag == "STAND_SWITCH") {
			if (enemy->pathTag == "STAND_SWITCH") {
				enemy->turnOnSpot(time);
			}
		}
		//enemy has a path to follow
		else {
			enemy->followPath(mainLayer, time);
		}
	}
	else {
		enemy->pause(time);
	}

	return nullptr;
}
void Enemy::SuspectState::exit(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->walkTime /= 0.65f;
	enemy->waitTime /= 0.65f;
	enemy->turnTime /= 0.65f;
	enemy->returning = true;
}

//Alerted State:
void Enemy::AlertState::enter(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->paused = false;
	enemy->startPauseTime = -1;
	enemy->qMark->setVisible(false);
	enemy->exMark->setVisible(true);
	enemy->setSpeed(2.09f);
	enemy->setName("enemy_alert");
}
Enemy::State* Enemy::AlertState::update(Enemy* enemy, GameLayer* mainLayer, float time) {
	//check if enemy has seen an item
	if (enemy->itemToPickUp != NULL && enemy->heldItem == NULL) {
		enemy->pickUpItem(mainLayer);
	}
	//check if enemy is walking into a door
	if (enemy->doorToUse != NULL) {
		return new UseDoorState;
	}
	//checking if enemy spotted player
	if (enemy->seeingPlayer() == true) {
		enemy->changeSuspicion(enemy->maxSuspicion / (1.0f SECONDS));
	}
	else {
		enemy->changeSuspicion(-enemy->maxSuspicion / (28 SECONDS));
	}
	//enemy has somehow dropped straight to 0 supicion
	if (enemy->suspicionLevel <= 0) {
		return new DefaultState;
	}
	//check if an enemy has become unalerted
	else if (enemy->suspicionLevel <= enemy->maxSuspicion / 2) {
		return new SuspectState;
	}
	enemy->pathTo(mainLayer,enemy->detectedPlayer->getPositionX(), enemy->detectedPlayer->currentFloor);
	return nullptr;
}
void Enemy::AlertState::exit(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->returning = true;
	enemy->slowStop();
	enemy->previousTurnTime = -1;
	enemy->stopTime = -1;
}

//Use Door State:
void Enemy::UseDoorState::enter(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->paused = false;
	enemy->startPauseTime = -1;
	enemy->doorStartTime = time;
	if (enemy->doorToUse->checkOpen() == false) {
		enemy->openDoor();//only use the door if it's not open yet
	}
	if (enemy->flippedX == false) {
		enemy->doorUsePos = enemy->getPositionX() + enemy->getContentSize().width;
	}
	else {
		enemy->doorUsePos = enemy->getPositionX() - enemy->getContentSize().width;
	}
}
Enemy::State* Enemy::UseDoorState::update(Enemy* enemy, GameLayer* mainLayer, float time) {
	if (enemy->doorToUse == NULL) {
		return enemy->prevState;
	}
	if (enemy->prevState->type != "alert") {//enemy is not coming from alert state
		if (enemy->doorToUse->checkLock() == true) {//they couldn't actually open the door, if they have a key they will use it automatically to unlock the door
			enemy->changeSuspicion(enemy->maxSuspicion / 2);//increases suspicion by a half
			enemy->stop();
			enemy->reachedNode = true;
			enemy->reachedNodeTime = time;
			if (enemy->pathIterator == 1) {//turn around if they can't open the door
				enemy->pathIterator = -1;
			}
			else if (enemy->pathIterator == -1) {
				enemy->pathIterator = 1;
			}
			enemy->flipX();
			if (enemy->flippedX == true) {
				enemy->setPosition(enemy->getPosition() + Vec2(-5, 0));//to ensure they do not try to open the door again right away
			}
			else {
				enemy->setPosition(enemy->getPosition() + Vec2(5, 0));//to ensure they do not try to open the door again right away
			}
			return new DefaultState;
		}

		if (enemy->paused == false) {
			if (abs(enemy->getPositionX() - enemy->doorUsePos) > enemy->doorToUse->radius + 1) {//enemy has walked through door
				return enemy->prevState;
			}
			else {
				enemy->move(Vec2(4.5 * enemy->moveSpeed, 0));
			}
		}
		else {
			enemy->pause(time);
		}

		//checking if enemy spotted player
		if (enemy->seeingPlayer() == true) {
			enemy->changeSuspicion(enemy->maxSuspicion / (0.6f SECONDS));//increases 1/45th of max every frame, takes 45 frames to alert guard
		}
		else {
			enemy->changeSuspicion(-enemy->maxSuspicion / (60 SECONDS));//takes 30 seconds to drop from half to 0
		}
		//check if player bumped enemy
		if (enemy->isTouched == true) {
			enemy->startPauseTime = -1;
			enemy->timeToPauseFor = 3.0f;
			enemy->paused = true;
			enemy->wasFlipped = enemy->flippedX;
			enemy->pathTo(mainLayer, enemy->detectedPlayer->getPositionX(), enemy->detectedPlayer->currentFloor);
			enemy->changeSuspicion(enemy->maxSuspicion / (22 SECONDS));
			enemy->isTouched = false;
			enemy->detectedPlayer = NULL;
		}
		//check if an enemy has become alerted
		if (enemy->suspicionLevel >= enemy->maxSuspicion) {
			enemy->detectedPlayer = static_cast<Player*>(mainLayer->getChildByTag(enemy->detectedTag));
			return new AlertState;
		}
	}
	else if (enemy->prevState->type == "alert"){//enemy was in alert state, just open door and run
		if (enemy->doorToUse->checkLock() == true) {//they didn't actually open the door
			enemy->breakDoor(time);
		}
		else {//if door is unlocked
			if (abs(enemy->getPositionX() - enemy->doorUsePos) > enemy->doorToUse->radius + 1) {//enemy has walked through door
				return enemy->prevState;
			}
			else {
				enemy->move(Vec2(4.5 * enemy->moveSpeed, 0));
			}
		}
	}
	return nullptr;
}
void Enemy::UseDoorState::exit(Enemy* enemy, GameLayer* mainLayer, float time) {
	if (enemy->prevState->type != "alert") {//alert enemies don't close doors behind them, even if they were locked before
		enemy->closeDoor();
	}
	enemy->doorToUse = NULL;
	//adding door use time to other time trackers
	enemy->doorUseTime = time - enemy->doorStartTime;
	if (enemy->previousTurnTime != -1) {
		enemy->previousTurnTime += enemy->doorUseTime;
	}
	if (enemy->stopTime != -1 && enemy->stopTime != -3) {
		enemy->stopTime += enemy->doorUseTime;
	}
}

//Get Item State:
void Enemy::GetItemState::enter(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->paused = false;
	//adding item pick up time to other time trackers
	enemy->pickupStartTime = time;
	enemy->changeSuspicion(enemy->maxSuspicion / 3);//seeing an item increases their suspicion by a third
	enemy->qMark->setVisible(true);

}
Enemy::State* Enemy::GetItemState::update(Enemy* enemy, GameLayer* mainLayer, float time) {
	if (enemy->paused == false) {
		if (enemy->itemToPickUp == NULL) {
			return new DefaultState;
		}
		if (enemy->moveToObject(enemy->itemToPickUp) == true) {
			enemy->pickUpItem(mainLayer);
			if (enemy->prevState->type == "use_door") {//if they are coming from a use door state
				return new DefaultState;//go to default state instead of previous state
			}
			return enemy->prevState;
		}
		//check if enemy is walking into a door
		if (enemy->doorToUse != NULL) {
			return new UseDoorState;
		}
	}
	else {
		enemy->pause(time);
	}

	//checking if enemy spotted player
	if (enemy->seeingPlayer() == true) {
		enemy->changeSuspicion(enemy->maxSuspicion / (0.6f SECONDS));//increases 1/45th of max every frame, takes 45 frames to alert guard
	}
	else {
		enemy->changeSuspicion(-enemy->maxSuspicion / (60 SECONDS));//takes 30 seconds to drop from half to 0
	}
	//check if player bumped enemy
	if (enemy->isTouched == true) {
		enemy->startPauseTime = -1;
		enemy->timeToPauseFor = 3.0f;
		enemy->paused = true;
		enemy->wasFlipped = enemy->flippedX;
		enemy->pathTo(mainLayer, enemy->detectedPlayer->getPositionX(), enemy->detectedPlayer->currentFloor);
		enemy->changeSuspicion(enemy->maxSuspicion / (22 SECONDS));
		enemy->isTouched = false;
		enemy->detectedPlayer = NULL;
	}
	//check if an enemy has become alerted
	if (enemy->suspicionLevel >= enemy->maxSuspicion) {
		enemy->detectedPlayer = static_cast<Player*>(mainLayer->getChildByTag(enemy->detectedTag));
		return new AlertState;
	}
	return nullptr;
}
void Enemy::GetItemState::exit(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->itemToPickUp = NULL;
	//adding item pickup time to other time trackers
	enemy->itemPickupTime = time - enemy->pickupStartTime;
	if (enemy->previousTurnTime != -1) {
		enemy->previousTurnTime += enemy->itemPickupTime;
	}
	if (enemy->stopTime != -1 && enemy->stopTime != -3) {
		enemy->stopTime += enemy->itemPickupTime;
	}
}


//Search State:
void Enemy::SearchState::enter(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->paused = false;
	enemy->startPauseTime = -1;
	enemy->qMark->setVisible(true);
}
Enemy::State* Enemy::SearchState::update(Enemy* enemy, GameLayer* mainLayer, float time) {
	if (enemy->paused == false) {
		if (enemy->reachedLocation == false) {
			if (enemy->pathTo(mainLayer, enemy->noiseLocation->getPositionX(), enemy->noiseLocation->startRoom.y) == true) {
				enemy->reachedLocation = true;
			}
		}
		else {//they have reached location
			enemy->walk(time);//start patrolling the area
			enemy->changeSuspicion(-1 * enemy->maxSuspicion / (22 SECONDS));
		}
		//check if enemy is walking into a door
		if (enemy->doorToUse != NULL) {
			return new UseDoorState;
		}
	}
	else {
		enemy->pause(time);
	}

	//checking if enemy spotted player
	if (enemy->seeingPlayer() == true) {
		enemy->changeSuspicion(enemy->maxSuspicion / (0.6f SECONDS));//increases 1/45th of max every frame, takes 45 frames to alert guard
	}
	//check if player bumped enemy
	if (enemy->isTouched == true) {
		enemy->startPauseTime = -1;
		enemy->timeToPauseFor = 3.0f;
		enemy->paused = true;
		enemy->wasFlipped = enemy->flippedX;
		enemy->pathTo(mainLayer, enemy->detectedPlayer->getPositionX(), enemy->detectedPlayer->currentFloor);
		enemy->changeSuspicion(enemy->maxSuspicion / (22 SECONDS));
		enemy->isTouched = false;
		enemy->detectedPlayer = NULL;
	}
	//check if an enemy has become alerted
	if (enemy->suspicionLevel >= enemy->maxSuspicion) {
		enemy->detectedPlayer = static_cast<Player*>(mainLayer->getChildByTag(enemy->detectedTag));
		return new AlertState;
	}
	//enemy has dropped to 0 supicion
	else if (enemy->suspicionLevel <= 0) {
		return new DefaultState;
	}
	return nullptr;
}
void Enemy::SearchState::exit(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->noiseLocation = NULL;
	enemy->returning = true;
}

//Seen Body State:
void Enemy::SeenBodyState::enter(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->paused = false;
	enemy->startPauseTime = -1;
	enemy->qMark->setVisible(true);
}
Enemy::State* Enemy::SeenBodyState::update(Enemy* enemy, GameLayer* mainLayer, float time) {
	if (enemy->paused == false) {
		if (enemy->reachedLocation == false) {
			if (enemy->moveToObject(enemy->bodySeen)== true) {
				enemy->reachedLocation = true;
			}
		}
		else {//they have reached location
			enemy->turnOnSpot(time);//stay where body is and look around
			enemy->changeSuspicion(enemy->maxSuspicion / (40 SECONDS));//suspicion will steadily increase until the become alerted
		}
		//check if enemy is walking into a door
		if (enemy->doorToUse != NULL) {
			return new UseDoorState;
		}
	}
	else {
		enemy->pause(time);
	}

	//checking if enemy spotted player
	if (enemy->seeingPlayer() == true) {
		enemy->setSuspicion(enemy->maxSuspicion);//will instantly detect the player
	}
	//check if player bumped enemy
	if (enemy->isTouched == true) {
		enemy->startPauseTime = -1;
		enemy->timeToPauseFor = 3.0f;
		enemy->paused = true;
		enemy->wasFlipped = enemy->flippedX;
		enemy->pathTo(mainLayer, enemy->detectedPlayer->getPositionX(), enemy->detectedPlayer->currentFloor);
		enemy->setSuspicion(enemy->maxSuspicion);
		enemy->isTouched = false;
		enemy->detectedPlayer = NULL;
	}
	//check if an enemy has become alerted
	if (enemy->suspicionLevel >= enemy->maxSuspicion) {
		enemy->detectedPlayer = static_cast<Player*>(mainLayer->getChildByTag(enemy->detectedTag));
		return new AlertState;
	}
	//enemy has dropped to 0 supicion somehow...
	else if (enemy->suspicionLevel <= 0) {
		return new DefaultState;
	}
	return nullptr;
}
void Enemy::SeenBodyState::exit(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->bodySeen = NULL;
	enemy->returning = true;
}