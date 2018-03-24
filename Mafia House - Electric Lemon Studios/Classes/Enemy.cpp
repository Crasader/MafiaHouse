#include "Enemy.h"
#include "Wall.h"

Enemy::Enemy()
{
	FRAME_OFFSET = 18;//do not change
	bodySize = Size(35, 94);
	name = "enemy";
	tag = 30000;//enemies will be 30000 - 39999
	//sprite properties
	zOrder = 4;
	scale = 1.0f;
	//physics body properties
	dynamic = true;
	category = 2;
	collision = 13;
	//other proeprties
	baseSpeed = 50;
	//for attacking without a weapon
	fist = Fist::createWithSpriteFrameName();
	fist->initObject();
	//initializing animations:
	stand = GameAnimation(STAND, "enemy/thug/stand/%03d.png", 1, 10 FRAMES);
	walking = GameAnimation(WALK, "enemy/thug/walk/%03d.png", 7, 10 FRAMES);
	knockout = GameAnimation(KNOCKOUT, "enemy/thug/knockout/%03d.png", 1, 10 FRAMES);
	stab = GameAnimation(STAB, "enemy/thug/stab/%03d.png", 2, 10 FRAMES);
	swing = GameAnimation(SWING, "enemy/thug/swing/%03d.png", 2, 10 FRAMES);
	ZZZAnimation = GameAnimation(ZZZs, "icons/ZZZ/%03d.png", 4, 50 FRAMES);
}

Vec2 Enemy::getPosition() {
	return Node::getPosition() + Vec2(FRAME_OFFSET, 0);
}

void Enemy::flipX() {
	Character::flipX();
	if (flippedX == true) {
		qMark->setFlippedX(true);
		ZZZ->setFlippedX(true);
		ZZZ->setPositionNormalized(Vec2(-0.2, 1.05));
	}
	else {
		qMark->setFlippedX(false);
		ZZZ->setFlippedX(false);
		ZZZ->setPositionNormalized(Vec2(0.2, 1.05));
	}
}

void Enemy::initObject(Vec2 startPos)
{
	GameObject::initObjectNoPhysics(startPos);
	initBoxBody(bodySize);
	//initializing suspicion icons:
	Texture2D::TexParams texParams = { GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };
	exMark = Sprite::createWithSpriteFrameName("icons/exmark.png");
	exMark->setAnchorPoint(Vec2(0.5, 0));
	exMark->setPositionNormalized(Vec2(0.5, 1.05));
	exMark->getTexture()->setTexParameters(texParams);
	exMark->setGlobalZOrder(9);
	exMark->setVisible(false);
	addChild(exMark);
	qMark = Sprite::createWithSpriteFrameName("icons/qmark.png");
	qMark->setAnchorPoint(Vec2(0.5, 0));
	qMark->setPositionNormalized(Vec2(0.5, 1.05));
	qMark->getTexture()->setTexParameters(texParams);
	qMark->setGlobalZOrder(9);
	qMark->setVisible(false);
	addChild(qMark);
	ZZZ = Sprite::createWithSpriteFrameName("icons/ZZZ/zzz.png");
	ZZZ->setAnchorPoint(Vec2(0, 0));
	ZZZ->setPositionNormalized(Vec2(0.2, 1.05));
	ZZZ->getTexture()->setTexParameters(texParams);
	ZZZ->setGlobalZOrder(9);
	ZZZ->setVisible(false);
	addChild(ZZZ);

	if (pathTag == "STAND_LEFT" || pathTag == "LEFT") {
		flipX();
		fist->knockback *= -1;
	}
	lastSeenLocation = Node::create();
	addChild(lastSeenLocation);

	//initializing knocked out physics body
	//knockedOutBody = Node::create();
	knockedOutBody = PhysicsBody::createBox(Size(bodySize.width * 2, bodySize.height / 3));
	knockedOutBody->setContactTestBitmask(0xFFFFFFFF);
	knockedOutBody->setTag(555);
	knockedOutBody->setName("enemy");
	knockedOutBody->setCategoryBitmask(2);
	knockedOutBody->setCollisionBitmask(8);
	knockedOutBody->setDynamic(true);
	knockedOutBody->setRotationEnable(false);
	knockedOutBody->setMass(1000);
	knockedOutBody->retain();
}

void Enemy::initJoints() {
	//PhysicsJointFixed* joint = PhysicsJointFixed::construct(getPhysicsBody(), knockedOutBody->getPhysicsBody(), Vec2(0.5, 0.5));
	//joint->setCollisionEnable(false);
	//director->getRunningScene()->getPhysicsWorld()->addJoint(joint);
}

void Enemy::changeSuspicion(float num) {
	suspicionLevel += num;
	suspicionLevel = suspicionLevel > maxSuspicion ? maxSuspicion : suspicionLevel;
	suspicionLevel = suspicionLevel < 0 ? 0 : suspicionLevel;
}
void Enemy::setSuspicion(float num) {
	suspicionLevel = num;
}

void Enemy::dropInventory(GameLayer* mainLayer) {
	int a = 1;
	for (int i = 0; i < inventory.size(); i++) {
		inventory.at(i)->setRotation(randNum(0,360));
		inventory.at(i)->getPhysicsBody()->applyImpulse(Vec2(randNum(50, 100), randNum(50, 100)));
		inventory.at(i)->initDroppedItem(getPosition() + Vec2(a * 20, 60), flippedX);//drop all items at enemy position
		if (inventory.at(i)->isKey == true) {
			hasKey = false;//if they drop a key, they don't have a key anymore
		}
		inventory.at(i)->stopAllActions();
		removeChild(inventory.at(i), true);
		mainLayer->addChild(inventory.at(i));
		inventory.erase(inventory.begin() + i);
		i--;
		a++;
	}
	heldItem = NULL;
}

void Enemy::pickUpItem(GameLayer* mainLayer) {
	if (itemToPickUp->enemyItem != true) {
		Character::pickUpItem(mainLayer);
	}
	if (heldItem != NULL) {
		heldItem->enemyItem = true;
		if (heldItem->isKey == true) {
			hasKey = true;//if they pickup a key, they have a key to open locked doors
		}
	}
}

void Enemy::openDoor() {
	if (hasKey == true) {
		doorToUse->unlock();//try to unlock all doors, nothing happens if it isn't locked
	}
	if (doorToUse->use() == true) {
		openedDoor = true;
	}
}
void Enemy::closeDoor() {
	doorToUse->use();
	if (hasKey == true) {
		if (doorToUse->defaultLocked == true) {//only lock doors that are set to be locked in the level
			doorToUse->lock();
		}
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
		/*if (pathTag == "NONE" || pathTag == "LEFT") {
			if (wasFlipped == true && flippedX == false) {
				flipX();
			}
			else if (wasFlipped == false && flippedX == true) {
				flipX();
			}
		}*/
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

bool checkForPathWithLocks(GameLayer* mainlayer, int floorNum, int targetX, int searcherX, Enemy* enemy) {//always go Tip to Tail, put the thing you wanna get to first
	bool isPath = false;
	int roomDiff = targetX - searcherX;//getting number of rooms between stair and partner stair
	if (roomDiff == 0) {//stair is in same room as partner stair
		return true;
	}
	else {//stair is in different room from parnter stair
		if (roomDiff > 0) {//target is to the right of searcher
			for (int x = searcherX; x < targetX; x++) {
				if (mainlayer->floors[floorNum].rooms[x]->hasRightDoor == false) {//room doesn't have a right door
					return false;
				}
				else {//room has a right door
					isPath = true;
				}
			}
		}
		else if (roomDiff < 0) {//target is to the left of searcher
			for (int x = searcherX; x > targetX; x--) {
				if (mainlayer->floors[floorNum].rooms[x]->hasLeftDoor == false) {//room doesn't have a left door
					return false;
				}
				else {//room has a left door
					isPath = true;
				}
			}
		}
	}
	return isPath;
}

bool checkForPath(GameLayer* mainlayer, int floorNum, int targetX, int searcherX, Enemy* enemy) {//always go Tip to Tail, put the thing you wanna get to first
	bool isPath = false;
	int roomDiff = targetX - searcherX;//getting number of rooms between stair and partner stair
	if (roomDiff == 0) {//stair is in same room as partner stair
		return true;
	}
	else {//stair is in different room from parnter stair
		if (roomDiff > 0) {//target is to the right of searcher
			for (int x = searcherX; x < targetX; x++) {
				if (mainlayer->floors[floorNum].rooms[x]->hasRightDoor == false) {//room doesn't have a right door
					return false;
				}
				else {//room has a right door
					if (mainlayer->floors[floorNum].rooms[x]->rightLocked == true) {//but it's locked
						if (enemy->checkKey() == false) {//and they don't have a key...
							return false;
						}
					}
					isPath = true;
				}
			}
		}
		else if (roomDiff < 0) {//target is to the left of searcher
			for (int x = searcherX; x > targetX; x--) {
				if (mainlayer->floors[floorNum].rooms[x]->hasLeftDoor == false) {//room doesn't have a left door
					return false;
				}
				else {//room has a left door
					if (mainlayer->floors[floorNum].rooms[x]->leftLocked == true) {//but it's locked
						if (enemy->checkKey() == false) {//and they don't have a key...
							return false;
						}
					}
					isPath = true;
				}
			}
		}
	}
	return isPath;
}

void Enemy::followPath(GameLayer* mainLayer, float time) {
	if (reachedNode == false) {
		if (pathTo(mainLayer, pathNodes.at(pathNum)->getPositionX(), pathNodes.at(pathNum)->startRoom.y, pathNodes.at(pathNum)->startRoom.x, time, checkForPath) == true) {//if player has reached current path node
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

//a recursive function, searches for a path to get to a certain floor
Stair* Enemy::pathSearch(GameLayer* mainLayer, vector<Stair*> stairs, float xPos, bool(*checkPathFunc)(GameLayer* mainlayer, int floorNum, int targetX, int searcherX, Enemy* enemy)){
	depth++;//going down a level whenever it gets called
	vector<Stair*> possibleReturnStairs;
	Stair* returnStair = NULL;
	vector<float> distances;
	for (int i = 0; i < stairs.size(); i++) {
		prevSearched.push_back(stairs[i]);//adding search stairs to list of previously searched stairs to prevent infinite recursion
		
		float distance = abs(xPos - stairs[i]->getPositionX());//get the distance between new search doors and previous search door's parnter stair
		distances.push_back(distance);
	}

	for (int i = 0; i < stairs.size(); i++) {//searches for stair to take to get to a certain floor
		Stair* takeStair = NULL;
		bool foundStairs = false;
		vector<Stair*> possibleStairsToTake;
		vector<Stair*> newSearchStairs;
		Stair* partnerStair = mainLayer->getPartnerStair(stairs[i]);

		for (int j = 0; j < mainLayer->stairs.size(); j++) {//each loop checks for a partner stair that is on same floor as enemy
			Stair* tempStair;
			Stair* queryStair = mainLayer->stairs[j];

			if (queryStair->startRoom.y == partnerStair->startRoom.y && queryStair != partnerStair) {//stair is on same floor as the partner stair, and is not the partner stair itself
				//check if there's a path from the partner stair to the query stair
				if (checkPathFunc(mainLayer, queryStair->startRoom.y, queryStair->startRoom.x, partnerStair->startRoom.x, this) == true) {
					//get the distance between the query stair and the partner stair
					float distanceBetweenDoors = abs(partnerStair->getPositionX() - queryStair->getPositionX());

					if (mainLayer->getPartnerStair(queryStair)->startRoom.y == currentFloor) {//partner stair of this query stair is on same floor as you
						//check if there's a path from query's partner stair to you
						if (checkPathFunc(mainLayer, currentFloor, currentRoom, mainLayer->getPartnerStair(queryStair)->startRoom.x, this) == true) {
							if (firstEndFound == false) {//only happens the first time the function finds an end point
								firstEndFound = true;
								shortestDepth = depth;
								//get distance between query's partner stair and you
								float distanceFromDoorToEnemy = abs(mainLayer->getPartnerStair(queryStair)->getPositionX() - getPositionX());
								float totalDistance = distances[i] + distanceBetweenDoors + distanceFromDoorToEnemy;

								tempStair = mainLayer->getPartnerStair(mainLayer->stairs[j]);//take this stair
								tempStair->foundDepth = depth;
								tempStair->pathDistance = totalDistance;
								possibleStairsToTake.push_back(tempStair);
								foundStairs = true;
							}
							else if (firstEndFound == true && depth <= shortestDepth) {//it's already found one end point
								if (depth < shortestDepth) { shortestDepth = depth; }//update the shortest depth at which an end point was found
								//get distance between query's partner stair and you
								float distanceFromDoorToEnemy = abs(mainLayer->getPartnerStair(queryStair)->getPositionX() - getPositionX());
								float totalDistance = distances[i] + distanceBetweenDoors + distanceFromDoorToEnemy;

								tempStair = mainLayer->getPartnerStair(mainLayer->stairs[j]);//take this stair
								tempStair->foundDepth = depth;
								tempStair->pathDistance = totalDistance;
								possibleStairsToTake.push_back(tempStair);
								foundStairs = true;
							}
						}
						else {
							bool add = true;
							tempStair = mainLayer->stairs[j];
							for (int k = 0; k < prevSearched.size(); k++) {
								//check if this stair has already been searched before
								if (tempStair == prevSearched[k]) { add = false; }
							}
							if (add == true) { newSearchStairs.push_back(tempStair); }
						}
					}
					else {
						bool add = true;
						tempStair = mainLayer->stairs[j];
						for (int k = 0; k < prevSearched.size(); k++) {
							//check if this stair has already been searched before
							if (tempStair == prevSearched[k]) { add = false; }
						}
						if (add == true) { newSearchStairs.push_back(tempStair); }
					}
				}
			}
		}
		if (foundStairs == true) {
			takeStair = possibleStairsToTake[0];
			//getting the stair found with the shortest distance to the last branch point
			if (possibleStairsToTake.size() > 1) {
				float shortest = possibleStairsToTake[0]->pathDistance;
				for (int j = 1; j < possibleStairsToTake.size(); j++) {
					if (possibleStairsToTake[j]->pathDistance < shortest) {
						shortest = possibleStairsToTake[j]->pathDistance;
						takeStair = possibleStairsToTake[j];
					}
				}
			}
			possibleReturnStairs.push_back(takeStair);
		}
		else if (newSearchStairs.size() > 0) {
			takeStair =  pathSearch(mainLayer, newSearchStairs, partnerStair->getPositionX(), checkPathFunc);
			if (takeStair != NULL) {
				possibleReturnStairs.push_back(takeStair);
			}
		}
	}

	if (possibleReturnStairs.size() > 0) {
		returnStair = possibleReturnStairs[0];
		if (possibleReturnStairs.size() > 1) {
			float smallestDepth = possibleReturnStairs[0]->foundDepth;
			float shortestDistance = possibleReturnStairs[0]->pathDistance;
			for (int i = 1; i < possibleReturnStairs.size(); i++) {
				if (possibleReturnStairs[i]->foundDepth < smallestDepth) {
					smallestDepth = possibleReturnStairs[i]->foundDepth;
					shortestDistance = possibleReturnStairs[i]->pathDistance;
					returnStair = possibleReturnStairs[i];
				}
				else if (possibleReturnStairs[i]->foundDepth == smallestDepth) {
					if (possibleReturnStairs[i]->pathDistance < shortestDistance) {
						shortestDistance = possibleReturnStairs[i]->pathDistance;
						returnStair = possibleReturnStairs[i];
					}
				}
			}
		}
	}

	depth--;//going up one level whenever it returns
	return returnStair;//fails to find a successful path
}

bool Enemy::pathTo(GameLayer* mainLayer, float positionX, int floorNum, int roomNum, float time, bool (*checkPathFunc)(GameLayer* mainlayer, int floorNum, int targetX, int searcherX, Enemy* enemy)) {
	//float displacement = positionX - getPositionX();
	Stair* takeStair = NULL;
	vector<Stair*> searchStairs;
	bool directPathToTarget = false;
	vector<Stair*>possibleStairsToTake;

	//target is on same floor as you:
	if (floorNum == currentFloor) {
		//check if enemy has a path to the player
		directPathToTarget = checkPathFunc(mainLayer, currentFloor, roomNum, currentRoom, this);
	}
	//target is on different floor, or you can't get to them from the same floor as them:
	if (floorNum != currentFloor || directPathToTarget == false){
		if (floorNum != currentFloor) {
			for (int i = 0; i < mainLayer->stairs.size(); i++) {
				Stair* queryStair = mainLayer->stairs[i];

				if (queryStair->startRoom.y == floorNum) {//stair is on destination floor
					if (checkPathFunc(mainLayer, floorNum, roomNum, queryStair->startRoom.x, this) == true) {//check if you can get to the target from this stair
						Stair* partnerStair = mainLayer->getPartnerStair(queryStair);
						
						if (mainLayer->getPartnerStair(queryStair)->startRoom.y == currentFloor) {//partner stair is on your floor
							if (checkPathFunc(mainLayer, currentFloor, partnerStair->startRoom.x, currentRoom, this) == true) {//check if you can get to the partner stair
								takeStair = mainLayer->getPartnerStair(queryStair);//take this stair
								possibleStairsToTake.push_back(takeStair);
							}
							else {
								searchStairs.push_back(queryStair);
							}
						}
						else {
							searchStairs.push_back(queryStair);
						}
					}
				}
			}
		}
		else if (floorNum == currentFloor) {
			for (int i = 0; i < mainLayer->stairs.size(); i++) {
				Stair* queryStair = mainLayer->stairs[i];

				if (queryStair->startRoom.y == floorNum) {//stair is on destination floor
					if (checkPathFunc(mainLayer, floorNum, roomNum, queryStair->startRoom.x, this) == true) {//check if you can get to the target from this stair
						searchStairs.push_back(queryStair);
					}
				}
			}
		}
		//if there is more than one possible stair to get to the destination:
		if (possibleStairsToTake.size() > 1) {
			Stair* partnerStair = mainLayer->getPartnerStair(possibleStairsToTake[0]);
			takeStair = possibleStairsToTake[0];
			float smallestDiff = abs(positionX - partnerStair->getPositionX());
			for (int i = 1; i < possibleStairsToTake.size(); i++) {//find stair with the partner that is closest to the target
				partnerStair = mainLayer->getPartnerStair(possibleStairsToTake[i]);
				float diff = abs(positionX - partnerStair->getPositionX());
				if (diff < smallestDiff) {//if this stair's parnter is closer to the target
					smallestDiff = diff;
					takeStair = possibleStairsToTake[i];
				}
			}
		}
	}
	if (directPathToTarget == false) {
		//search for a path to the target
		if (takeStair == NULL && searchStairs.size() > 0) {
			prevSearched.clear();
			pathLengths.clear();
			firstEndFound = false;
			depth = -1;
			takeStair = pathSearch(mainLayer, searchStairs, positionX, checkPathFunc);
		}
		//move towards the stair found
		if (takeStair != NULL) {
			//if they are not at location of stairs yet:
			if (!(getPositionX() + bodySize.width >= takeStair->getPositionX() && getPositionX() <= takeStair->getPositionX() + takeStair->getContentSize().width)) {
				moveTo(takeStair->getPositionX());//move towards stairs
			}
			//if they are at location of stairs:
			else {
				takeStair->use(this, mainLayer);//use the stairs
			}
		}
		//takeStair is still NULL:
		else {//could not find a path to the player
			if (checkPathFunc != checkForPathWithLocks) {
				return pathTo(mainLayer, positionX, floorNum, roomNum, time, checkForPathWithLocks);
			}
			else {
				walk(time);
			}
		}
	}
	else if (directPathToTarget == true) {
		moveTo(positionX);
		if (getPositionX() + (bodySize.width / 2) >= positionX - 50 && getPositionX() + (bodySize.width / 2) <= positionX + 50) {
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
	if (!(getPositionX() + bodySize.width >= target->getPositionX() && getPositionX() <= target->getPositionX() + target->getContentSize().width)) {
		moveTo(target->getPositionX());//move towards stairs
		return false;
	}
	return true;
}

bool Enemy::moveToDoor(Node* target) {
	if (flippedX == false) {
		if (!(getPositionX() + bodySize.width >= target->getPositionX() - 2)) {
			moveTo(target->getPositionX());//move towards stairs
			return false;
		}
		else {
			return true;
		}
	}
	else if (flippedX == true) {
		if (getPositionX() <= (target->getPositionX() + target->getContentSize().width + 2)) {
			return true;
		}
		else {
			moveTo(target->getPositionX());//move towards stairs
			return false;
		}
	}
}

Item* Enemy::findClosestItem(GameLayer* mainLayer) {
	Item* foundItem = NULL;
	vector<Item*> reachableItems;
	for (int i = 0; i < mainLayer->items.size(); i++) {
		if (mainLayer->items[i]->getState() == Item::GROUND && mainLayer->items[i]->isKey == false) {//item is on ground and is not a key
			if (mainLayer->items[i]->currentFloor == currentFloor && mainLayer->items[i]->getPositionY() <= (getPositionY() + getSize().height)) {//item is on same floor as you, and not above you
				if (checkForPath(mainLayer, currentFloor, mainLayer->items[i]->currentRoom, currentRoom, this) == true) {//you can get to the item
					if (detectedPlayer->currentFloor == currentFloor && detectedPlayer->getPositionY() <= (getPositionY() + getSize().height)) {//player is on same floor as you, and not above you
						float displacement = detectedPlayer->getPositionX() - getPositionX();
						if (displacement < 0) {//player is to the left
							if (mainLayer->items[i]->getPositionX() < detectedPlayer->getPositionX()) {//item is left of the player
								continue;//move on to next item
							}
						}
						else if (displacement > 0) {//player is to the right
							if (mainLayer->items[i]->getPositionX() > detectedPlayer->getPositionX()) {//item is right of the player
								continue;//move on to next item
							}
						}
					}
					reachableItems.push_back(mainLayer->items[i]);
				}
			}
		}
	}

	if (reachableItems.size() > 0) {//at least one item was found
		foundItem = reachableItems[0];
		if (reachableItems.size() > 1) {//if more than one item was found
			float minDistance = reachableItems[0]->getPositionX() - getPositionX();//the distance between you and the first item found
			for (int i = 1; i < reachableItems.size(); i++) {//find item closest to you
				float distance = reachableItems[i]->getPositionX() - getPositionX();
				if (distance < minDistance) {
					minDistance = distance;
					foundItem = reachableItems[i];
				}
			}
		}
	}

	return foundItem;
}

Item* Enemy::findBetterItem(GameLayer* mainLayer) {
	Item* foundItem = NULL;
	vector<Item*> reachableItems;
	for (int i = 0; i < mainLayer->items.size(); i++) {
		if (mainLayer->items[i]->getState() == Item::GROUND && mainLayer->items[i]->isKey == false && mainLayer->items[i]->powerLevel > heldItem->powerLevel) {//item is on ground and is not a key, and is more powerful
			if (mainLayer->items[i]->currentFloor == currentFloor && mainLayer->items[i]->getPositionY() <= (getPositionY() + getSize().height)) {//item is on same floor as you, and not above you
				if (checkForPath(mainLayer, currentFloor, mainLayer->items[i]->currentRoom, currentRoom, this) == true) {//you can get to the item
					if (detectedPlayer->currentFloor == currentFloor && detectedPlayer->getPositionY() <= (getPositionY() + getSize().height)) {//player is on same floor as you, and not above you
						float displacement = detectedPlayer->getPositionX() - getPositionX();
						if (displacement < 0) {//player is to the left
							if (mainLayer->items[i]->getPositionX() < detectedPlayer->getPositionX()) {//item is left of the player
								continue;//move on to next item
							}
						}
						else if (displacement > 0) {//player is to the right
							if (mainLayer->items[i]->getPositionX() > detectedPlayer->getPositionX()) {//item is right of the player
								continue;//move on to next item
							}
						}
					}
					reachableItems.push_back(mainLayer->items[i]);
				}
			}
		}
	}

	if (reachableItems.size() > 0) {//at least one item was found
		foundItem = reachableItems[0];
		if (reachableItems.size() > 1) {//if more than one item was found
			float minDistance = reachableItems[0]->getPositionX() - getPositionX();//the distance between you and the first item found
			for (int i = 1; i < reachableItems.size(); i++) {//find item closest to you
				float distance = reachableItems[i]->getPositionX() - getPositionX();
				if (distance < minDistance) {
					minDistance = distance;
					foundItem = reachableItems[i];
				}
			}
		}
	}

	return foundItem;
}

Item* Enemy::findMoreRange(GameLayer* mainLayer) {
	Item* foundItem = NULL;
	vector<Item*> reachableItems;
	for (int i = 0; i < mainLayer->items.size(); i++) {
		if (mainLayer->items[i]->getState() == Item::GROUND && mainLayer->items[i]->isKey == false && mainLayer->items[i]->getRangeY() > heldItem->getRangeY()) {//item is on ground and is not a key, and has more vertical range than held item
			if (mainLayer->items[i]->currentFloor == currentFloor && mainLayer->items[i]->getPositionY() <= (getPositionY() + getSize().height)) {//item is on same floor as you, and not above you
				if (checkForPath(mainLayer, currentFloor, mainLayer->items[i]->currentRoom, currentRoom, this) == true) {//you can get to the item
					if (detectedPlayer->currentFloor == currentFloor && detectedPlayer->getPositionY() <= (getPositionY() + getSize().height)) {//player is on same floor as you, and not above you
						float displacement = detectedPlayer->getPositionX() - getPositionX();
						if (displacement < 0) {//player is to the left
							if (mainLayer->items[i]->getPositionX() < detectedPlayer->getPositionX()) {//item is left of the player
								continue;//move on to next item
							}
						}
						else if (displacement > 0) {//player is to the right
							if (mainLayer->items[i]->getPositionX() > detectedPlayer->getPositionX()) {//item is right of the player
								continue;//move on to next item
							}
						}
					}
					reachableItems.push_back(mainLayer->items[i]);
				}
			}
		}
	}

	if (reachableItems.size() > 0) {//at least one item was found
		foundItem = reachableItems[0];
		if (reachableItems.size() > 1) {//if more than one item was found
			float minDistance = reachableItems[0]->getPositionX() - getPositionX();//the distance between you and the first item found
			for (int i = 1; i < reachableItems.size(); i++) {//find item closest to you
				float distance = reachableItems[i]->getPositionX() - getPositionX();
				if (distance < minDistance) {
					minDistance = distance;
					foundItem = reachableItems[i];
				}
			}
		}
	}

	return foundItem;
}

void Enemy::visionRays(vector<Vec2> *points, Vec2* start, float time){
	detectedTag = -1;
	playerInVision = false;
	didRun = false;
	Vec2 offsetAdjust;

	if (visionEnabled == true) {
		PhysicsRayCastCallbackFunc func = [this, points, offsetAdjust, time](PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data)->bool
		{
			int visionContactTag = info.shape->getBody()->getNode()->getTag();
			string visionContactName = info.shape->getBody()->getNode()->getName();
			Node* visionContact = info.shape->getBody()->getNode();

			//enemy vision is blocked by walls, doors
			if (visionContactName == "wall" || visionContactName == "floor" || visionContactName == "door") {
				points->push_back(info.contact + offsetAdjust);
				didRun = true;
				return false;
			}
			else if (visionContactName == "door_radius") {
				if (static_cast<Door*>(visionContact->getParent())->checkOpen() == false) {
					if (flippedX == true) {
						points->push_back(info.contact + Vec2(-12, 0));
					}
					else if (flippedX == false) {
						points->push_back(info.contact + Vec2(12, 0));
					}
					didRun = true;
					return false;
				}
			}
			//vision blocked by other enemies
			else if ((visionContactTag != getTag()) && (visionContactName == "enemy" || visionContactName == "enemy_alert")) {
				if (static_cast<Enemy*>(visionContact)->knockedOut == true) {
					bodySeen = static_cast<Enemy*>(visionContact);//they have seen a knocked out enemy
				}
				if (visionContactName == "enemy_alert" && state->type != "alert") {
					if (suspicionLevel < maxSuspicion) {
						setSuspicion(maxSuspicion + 100);//enemies that see other alerted enemies will become alerted
					}
				}
				points->push_back(info.contact + offsetAdjust);
				didRun = true;
				return false;
			}
			//enemy sees the player
			else if (visionContactName == "player") {//not using tag anymore
				if (static_cast<Player*>(visionContact)->isHidden() == false) {//if the player is not hidden
					static_cast<Player*>(visionContact)->inVision = true;
					detectedTag = visionContactTag;
					playerInVision = true;
					points->push_back(info.contact + offsetAdjust);
					didRun = true;
					return false;
				}
			}
			//enemy sees an item
			else if (visionContactName == "item" && state->type != "alert") {//enemy won't pick up seen items when alert
				if (static_cast<Item*>(visionContact)->enemyCanUse == true) {
					//check if the item is above you
					if (static_cast<Item*>(visionContact)->getPositionY() <= (getPositionY() + getSize().height)) {
						itemToPickUp = static_cast<Item*>(visionContact);//enemy can pick item up
					}
					bool noticeItem = true;
					if (seenItems.size() > 0) {
						for (int i = 0; i < seenItems.size(); i++) {
							if (static_cast<Item*>(visionContact) == seenItems[i]) {//check if enemy has already seen this item before
								noticeItem = false;
								break;
							}
						}
					}
					if (noticeItem == true) {
						qMark->setVisible(true);
						changeSuspicion(maxSuspicion / 3);//seeing an item increases their suspicion by a third
						seenItems.push_back(static_cast<Item*>(visionContact));
						seenTimes.push_back(time);
					}
					points->push_back(info.contact + offsetAdjust);
					didRun = true;
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
			startPoint = getPosition() + Vec2(bodySize.width - 15, 87);
			*start = startPoint + Vec2(8, 0);//shift visuals forward a bit
			offsetAdjust = Vec2(-8, 0);//shift end points back by same amount so visual range is accurate
			direction = 1;
		}
		else {
			startPoint = getPosition() + Vec2(15, 87);
			*start = startPoint + Vec2(-8, 0);//shift visuals forward a bit
			offsetAdjust = Vec2(8, 0);//shift end points back by same amount so visual range is accurate
			direction = -1;
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
}

void Enemy::gotHit(Item* item, float time) {
	if (item->didHitWall == false && item->hp > 0) {
		stopAllActions();
		wasInHitStun = true;
		hitStunStart = time;
		hitStunTime = item->hitstun;
		if (item->getEffect() == Item::NONE) {
			if ((flippedX == true && item->knockback.x < 0) || (flippedX == false && item->knockback.x > 0)) {//if the enemy is hit from behind
				flipX();
				setSuspicion(maxSuspicion - maxSuspicion * 0.1);
			}
		}
		if (touchingWall == false) {
			stop();
			moveAbsoluteNoLimit(item->knockback);
		}
		item->used();
		hp -= item->dmg;//dealing damage to enemy
		if (item->getEffect() == Item::KILL) {
			if (state->type != "alert" && state->type != "attack") {//hitting unalerted enemies with killing weapons will always kill them instantly
				hp = 0;
			}
			else {
				changeSuspicion(maxSuspicion);
			}
		}
		else if(item->getEffect() == Item::KNOCKOUT) {
			if (knockedOut == false) {
				knockedOut = true;
				knockOutTime = baseKnockOutTime * static_cast<float>(item->dmg);//more powerful items knock out for longer
				knockOutTime = knockOutTime < minKnockOuttime ? minKnockOuttime : knockOutTime;//set minimum knockOutTime to 20 seconds
				if (state->type == "alert") {
					knockOutTime /= 2;//knockout time is halved for alerted enemies
				}
			}
			else {//if they're already knocked out
				knockOutTime += baseKnockOutTime * static_cast<float>(item->dmg);//make them stay down for longer by hitting them again
			}
		}
	}
}

void Enemy::update(GameLayer* mainLayer, float time) {
	updateFloor(mainLayer->floors);
	updateRoom(mainLayer->floors[currentFloor].rooms);
	//forgetting what items they have seen
	if (seenItems.size() > 0) {
		for (int i = 0; i < seenItems.size(); i++) {
			if (time - seenTimes[i] >= memoryTime) {
				seenTimes.erase(seenTimes.begin() + i);
				seenItems.erase(seenItems.begin() + i);
				i--;
			}
		}
	}
	//checking invincibility frames
	if (invincible == true && time - hitTime >= invicibilityTime) {
		invincible = false;
	}
	//checking if they've been hit
	if (itemHitBy != NULL) {
		if (invincible == false) {
			hitTime = time;
			invincible = true;
			gotHit(itemHitBy, time);
		}
		itemHitBy = NULL;
	}
	if (time - hitStunStart >= hitStunTime || hitStunStart == -1) {//only update if hitstun is over, of if histun never began
		hitStunStart = -1;
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
		wasInHitStun = false;
	}

	//resetting collision checks:
	didHitWall = false;
	touchingWall = false;
	//updating color of question mark, turns more red as suspicion increases
	float BluePercentage = suspicionLevel / maxSuspicion;
	float GreenPercentage = abs(BluePercentage - 1);//inverts the percentage
	qMark->setColor(ccc3(255, 255 * GreenPercentage, 255 * BluePercentage));
}

//Enemy States:
void Enemy::State::enter(Enemy* enemy, GameLayer* mainLayer, float time) {}
Enemy::State* Enemy::State::update(Enemy* enemy, GameLayer* mainLayer, float time) { return nullptr; }
void Enemy::State::exit(Enemy* enemy, GameLayer* mainLayer, float time) {}

//Default State:
void Enemy::DefaultState::enter(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->inAttackRange = false;
	enemy->exMark->setVisible(false);
	enemy->paused = false;
	enemy->moveSpeed = 1.0f;
	enemy->setSpeed(enemy->moveSpeed);
	enemy->setName("enemy");
	enemy->visionDegrees = enemy->defaultDegrees;
	enemy->visionRadius = enemy->defaultRadius;
}
Enemy::State* Enemy::DefaultState::update(Enemy* enemy, GameLayer* mainLayer, float time) {
	if (enemy->checkDead() == true) {
		return new DeathState;
	}
	//check if enemy has been knocked out
	if (enemy->knockedOut == true) {
		return new KnockOutState;
	}
	//check if enemy is walking into a door
	if (enemy->doorToUse != NULL) {
		return new UseDoorState;
	}
	//check if enemy has seen a dead body or knocked out ally
	if (enemy->bodySeen != NULL) {
		return new SeenBodyState;
	}
	//check if enemy has seen an item
	if (enemy->itemToPickUp != NULL) {
		if (time > 0.033) {//so they don't get suspoicious at the very beginning of the game
			return new GetItemState;
		}
		else {
			enemy->pickUpItem(mainLayer);
		}
	}
	//check if an enemy has become alerted
	if (enemy->suspicionLevel >= enemy->maxSuspicion) {
		if (enemy->detectedTag != -1) {
			enemy->detectedPlayer = static_cast<Player*>(mainLayer->getChildByTag(enemy->detectedTag));
		}
		return new AlertState;
	}
	else if (enemy->suspicionLevel >= enemy->maxSuspicion / 2) {
		return new SuspectState;
	}
	//checking if enemy spotted player
	if (enemy->seeingPlayer() == true) {
		enemy->changeSuspicion(enemy->maxSuspicion / (0.4f SECONDS));//amount if time it takes for max suspicion to be reached
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
		enemy->pathTo(mainLayer, enemy->detectedPlayer->getPositionX(), enemy->detectedPlayer->currentFloor, enemy->detectedPlayer->currentRoom, time, checkForPath);
		enemy->isTouched = false;
		enemy->detectedPlayer = NULL;
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
				if (enemy->pathTo(mainLayer, enemy->initialPos.x, enemy->startRoom.y, enemy->startRoom.x, time, checkForPath) == true) {
					enemy->returning = false;
				}
			}
		}
		//enemy does not move
		else if (enemy->pathTag == "STAND_LEFT" || enemy->pathTag == "STAND_RIGHT" || enemy->pathTag == "STAND_SWITCH") {
			if (enemy->returning == true) {
				if (enemy->pathTo(mainLayer, enemy->initialPos.x, enemy->startRoom.y, enemy->startRoom.x, time, checkForPath) == true) {
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
	enemy->inAttackRange = false;
	enemy->prevPauseTime = time;
	enemy->exMark->setVisible(false);
	enemy->qMark->setVisible(true);
	enemy->moveSpeed = 1.65f;
	enemy->setSpeed(enemy->moveSpeed);
	enemy->walkTime *= 0.65f;
	enemy->waitTime *= 0.65f;
	enemy->setName("enemy");
	enemy->turnTime *= 0.65f;
	//enemy->visionDegrees = enemy->defaultDegrees * 1.1;
	enemy->visionRadius = enemy->defaultRadius * 1.3;
}
Enemy::State* Enemy::SuspectState::update(Enemy* enemy, GameLayer* mainLayer, float time) {
	if (enemy->checkDead() == true) {
		return new DeathState;
	}
	//check if enemy has been knocked out
	if (enemy->knockedOut == true) {
		return new KnockOutState;
	}
	//check if enemy is walking into a door
	if (enemy->doorToUse != NULL) {
		return new UseDoorState;
	}
	//check if enemy has seen a dead body or knocked out ally
	if (enemy->bodySeen != NULL) {
		return new SeenBodyState;
	}
	//check if enemy has seen an item
	if (enemy->itemToPickUp != NULL) {
		return new GetItemState;
	}
	//check if enemy has heard a noise
	if (enemy->noiseLocation != NULL) {
		return new SearchState;
	}
	//check if an enemy has become alerted
	if (enemy->suspicionLevel >= enemy->maxSuspicion) {
		if (enemy->detectedTag != -1) {
			enemy->detectedPlayer = static_cast<Player*>(mainLayer->getChildByTag(enemy->detectedTag));
		}
		return new AlertState;
	}
	//check if an enemy has become unalerted
	else if (enemy->suspicionLevel <= 0) {
		return new DefaultState;
	}
	//checking if enemy spotted player
	if (enemy->seeingPlayer() == true) {
		enemy->changeSuspicion(enemy->maxSuspicion / (0.8f SECONDS));
	}
	else {
		enemy->changeSuspicion(-1 * enemy->maxSuspicion / (24 SECONDS));
	}
	//check if player bumped enemy
	if (enemy->isTouched == true) {
		enemy->timeToPauseFor = 5.0f;
		enemy->paused = true;
		enemy->wasFlipped = enemy->flippedX;
		enemy->pathTo(mainLayer, enemy->detectedPlayer->getPositionX(), enemy->detectedPlayer->currentFloor, enemy->detectedPlayer->currentRoom, time, checkForPath);
		enemy->changeSuspicion(enemy->maxSuspicion / (22 SECONDS));
		enemy->isTouched = false;
		enemy->detectedPlayer = NULL;
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
	enemy->goingToFirstItem = false;
	enemy->goingToBetterItem = false;
	enemy->goingToMoreRange = false;
	if (enemy->heldItem != NULL) {
		if (enemy->heldItem->isKey == true && enemy->inventory.size() > 1) {
			for (int i = 0; i < enemy->inventory.size(); i++) {
				if (enemy->inventory[i]->isKey == false) {
					enemy->removeChild(enemy->heldItem, true);
					enemy->heldItem = enemy->inventory[i];
					enemy->heldItem->initHeldItem();
					enemy->addChild(enemy->heldItem);
					break;
				}
			}
		}
	}
	if (enemy->detectedPlayer == NULL) {
		enemy->detectedPlayer = static_cast<Player*>(mainLayer->getChildByName("player"));
	}
	enemy->paused = false;
	enemy->startPauseTime = -1;
	enemy->qMark->setVisible(false);
	enemy->exMark->setVisible(true);
	enemy->setSpeed(2.09f);
	enemy->setName("enemy_alert");
	enemy->lostPlayer = false;
	enemy->reachedLastSeen = false;
	//enemy->visionDegrees = enemy->defaultDegrees * 1.1;
	enemy->visionRadius = enemy->defaultRadius * 1.7;
}
Enemy::State* Enemy::AlertState::update(Enemy* enemy, GameLayer* mainLayer, float time) {
	if (enemy->checkDead() == true) {
		return new DeathState;
	}
	//check if enemy has been knocked out
	if (enemy->knockedOut == true) {
		return new KnockOutState;
	}
	if (enemy->detectedPlayer == NULL) {
		return new DefaultState;
	}
	//check if enemy has no weapoon, and they have no item to pick up either
	if (enemy->itemToPickUp == NULL && (enemy->heldItem == NULL || enemy->heldItem->isKey == true)) {
		if (enemy->detectedPlayer->heldItem != NULL && enemy->detectedPlayer->heldItem->isKey == false) {//player has a held item and it is not a key
			enemy->itemToPickUp = (enemy->findClosestItem(mainLayer));//find a weapon that is closest to them
			if (enemy->itemToPickUp != NULL) {//if one was found
				enemy->goingToFirstItem = true;
				return new GetItemState;//go and get it
			}
		}
	}
	//check if there's a better weapon to pick up
	else if (enemy->heldItem != NULL && enemy->heldItem->isKey == false) {//enemy already has an item that isn't a key
		if (enemy->detectedPlayer->heldItem != NULL && enemy->detectedPlayer->heldItem->isKey == false) {//player has a weapon that isn't a key
			if (enemy->detectedPlayer->heldItem->powerLevel > enemy->heldItem->powerLevel) {//if player's weapon is better than theirs
				enemy->itemToPickUp = (enemy->findBetterItem(mainLayer));//find a weapon that is closest to them
				if (enemy->itemToPickUp != NULL) {//if one was found
					enemy->goingToBetterItem = true;
					return new GetItemState;//go and get it
				}
			}
		}
	}
	//check if enemy has run into an item
	if (enemy->itemToPickUp != NULL && (enemy->heldItem == NULL || enemy->heldItem->isKey == true)) {//enemy doesn't have a held item or it is a key
		enemy->pickUpItem(mainLayer);
	}
	//check if enemy is walking into a door
	if (enemy->doorToUse != NULL) {
		if (enemy->isTouched == false && enemy->itemHitBy == NULL && enemy->inAttackRange == false) {
			return new UseDoorState;
		}
	}

	//check if an enemy has become unalerted
	if (enemy->suspicionLevel < (enemy->maxSuspicion * 0.4)) {
		return new SuspectState;
	}
	//checking if enemy spotted player
	if (enemy->seeingPlayer() == true) {
		enemy->changeSuspicion(enemy->maxSuspicion / (1.0f SECONDS));
		enemy->lostPlayer = false;
		enemy->reachedLastSeen = false;
	}
	else {
		enemy->changeSuspicion(-enemy->maxSuspicion / (40 SECONDS));
	}
	//check if player bumped enemy
	if (enemy->isTouched == true) {
		enemy->changeSuspicion(enemy->maxSuspicion / (20 SECONDS));
		enemy->isTouched = false;
		enemy->lostPlayer = false;
		enemy->reachedLastSeen = false;
	}
	//enemy has somehow dropped straight to 0 supicion
	if (enemy->suspicionLevel <= 0) {
		return new DefaultState;
	}

	//check if the player is hidden
	if (enemy->detectedPlayer->isHidden() == true) {
		//if the player wasn't seen going into hiding
		if (enemy->detectedPlayer->wasSeen == false && enemy->lostPlayer == false) {
			enemy->lostPlayer = true;
			enemy->lastSeenLocation->setPositionX(enemy->detectedPlayer->getPositionX());
			enemy->lastSeenLocation->setPositionY(enemy->detectedPlayer->currentFloor);//use y position for floor player was on
		}
	}

	//enemy still knows where they player is
	if (enemy->lostPlayer == false) {
		enemy->pathTo(mainLayer, enemy->detectedPlayer->getPositionX(), enemy->detectedPlayer->currentFloor, enemy->detectedPlayer->currentRoom, time, checkForPath);
		//check if enemy has a held item or not
		if (enemy->heldItem == NULL) {
			enemy->heldItem = enemy->fist;//if not, give them a fist
			enemy->addChild(enemy->fist);
		}
		if (enemy->heldItem != NULL) {
			bool inVerticalRange = true;
			enemy->inAttackRange = false;
			//get distance from player
			if (enemy->flippedX == true) {
				enemy->distanceToPlayer = abs((enemy->detectedPlayer->getPositionX() + enemy->detectedPlayer->getSize().width) - enemy->getPositionX());
			}
			else if (enemy->flippedX == false) {
				enemy->distanceToPlayer = abs(enemy->detectedPlayer->getPositionX() - (enemy->getPositionX() + enemy->getSize().width));
			}
			//check if enemy is in range to attack player
			if (enemy->distanceToPlayer <= enemy->heldItem->getRange() && enemy->currentFloor == enemy->detectedPlayer->currentFloor) {//enemy is within horizontal range to attack player
				//for stabbing weapons
				if (enemy->heldItem->getAttackType() == Item::STAB) {
					if (enemy->detectedPlayer->getPositionY() <= ((enemy->getPositionY() + enemy->getSize().height / 2) + enemy->heldItem->getRangeY())) {//check vertical range
						enemy->inAttackRange = true;
					}
					else {
						inVerticalRange = false;
					}
				}
				//for swinging weapons
				else if (enemy->heldItem->getAttackType() == Item::SWING) {
					if (((enemy->detectedPlayer->getPosition() + Vec2(enemy->detectedPlayer->getSize().width / 2, 0)) - (enemy->getPosition() + enemy->getSize() / 2)).getLength() <= enemy->heldItem->getRangeRadius()) {//check for radial range
						enemy->inAttackRange = true;
					}
					else {
						inVerticalRange = false;
					}
				}
			}
			if (enemy->inAttackRange == true) {
				return new AttackState;
			}
			else {//in range == false
				if (enemy->heldItem == enemy->fist) {//if not in range, discard fist item
					enemy->heldItem = NULL;
					enemy->removeChild(enemy->fist, true);
				}
				if (enemy->heldItem != NULL) {//if they have an item
					if (inVerticalRange == false) {//you could get to them horizontally but not vertically
						//find an item with more vertical range
						enemy->itemToPickUp = (enemy->findMoreRange(mainLayer));//find a weapon that is closest to them
						if (enemy->itemToPickUp != NULL) {//if one was found
							enemy->goingToMoreRange = true;
							return new GetItemState;//go and get it
						}
					}
				}
			}
		}
	}
	//enemy didn't see player hide
	else {
		if (enemy->reachedLastSeen == false) {
			enemy->reachedLastSeen = enemy->pathTo(mainLayer, enemy->lastSeenLocation->getPositionX(), enemy->lastSeenLocation->getPositionY(), enemy->lastSeenLocation->getPositionX(), time, checkForPath);
		}
		else {//they have reached player's last seen location
			enemy->walk(time);
		}
	}
	return nullptr;
}
void Enemy::AlertState::exit(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->returning = true;//only used for enemies not following a path
	enemy->slowStop();
	enemy->previousTurnTime = -1;
	enemy->stopTime = -1;
	enemy->lostPlayer = false;
	enemy->reachedLastSeen = false;
}

//Attack State(using items):
void Enemy::AttackState::enter(Enemy* enemy, GameLayer* mainLayer, float time) {
	if (enemy->heldItem == NULL) {
		enemy->heldItem = enemy->fist;//if not, give them a fist
		enemy->addChild(enemy->fist);
	}
	enemy->stop();
	enemy->attackPrepareTime = time;
	enemy->beginUseItem(enemy->aimAngle);
}
Enemy::State* Enemy::AttackState::update(Enemy* enemy, GameLayer* mainLayer, float time) {
	if (enemy->checkDead() == true) {
		return new DeathState;
	}
	//check if enemy has been knocked out
	if (enemy->knockedOut == true) {
		return new KnockOutState;
	}
	if (enemy->heldItem == NULL) {//incase you knock them out in the middle of an attack and steal their weapon
		return new DefaultState;//shouldn't be possible now tha tthey can attack without an item
	}
	//if (enemy->heldItem->didHitWall == true) {
	//	enemy->move(Vec2(-1, 0));
	//}
	if (enemy->attackPrepareTime != -1.0f && time - enemy->attackPrepareTime >= enemy->heldItem->getStartTime()) {
		enemy->attackStartTime = time;
		enemy->useItem(enemy->aimAngle);
		enemy->attackPrepareTime = -1.0f;
	}
	if (enemy->attackStartTime != -1.0f && time - enemy->attackStartTime >= enemy->heldItem->getAttackTime()) {
		enemy->setSpriteFrame(enemy->stab.animation->getFrames().at(0)->getSpriteFrame());
		enemy->heldItem->getPhysicsBody()->setEnabled(false);
		enemy->attackEndTime = time;
		enemy->attackStartTime = -1.0f;
	}
	if (enemy->attackEndTime != -1.0f && time - enemy->attackEndTime >= enemy->heldItem->getLagTime()) {
		enemy->attackEndTime = -1.0f;
		return enemy->prevState;
	}
	return nullptr;
}
void Enemy::AttackState::exit(Enemy* enemy, GameLayer* mainLayer) {
	enemy->heldItem->didHitWall = false;
	//if (enemy->heldItem->hp <= 0) {enemy->breakItem(mainLayer);}//enemy items don't break
	enemy->heldItem->initHeldItem();
	if (enemy->heldItem == enemy->fist) {
		enemy->heldItem = NULL;
		enemy->removeChild(enemy->fist, true);
	}
}

//Use Door State:
void Enemy::UseDoorState::enter(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->inAttackRange = false;
	if (enemy->prevState->type != "attack") {
		enemy->paused = false;
		enemy->startPauseTime = -1;
		enemy->doorStartTime = time;
		enemy->startWaitTime = -1;
		enemy->openedDoor = false;
		if (enemy->doorToUse->checkOpen() == false) {
			enemy->openDoor();//only use the door if it's not open yet
		}
		enemy->moveTo(enemy->doorToUse->getPositionX());//this is to ensure they are facing towards the door
		if (enemy->flippedX == false) {
			enemy->doorUsePos = enemy->getPositionX() + enemy->bodySize.width;
		}
		else {
			enemy->doorUsePos = enemy->getPositionX() - enemy->bodySize.width;
		}
	}
}
Enemy::State* Enemy::UseDoorState::update(Enemy* enemy, GameLayer* mainLayer, float time) {
	if (enemy->checkDead() == true) {
		enemy->toEnter = new DeathState;
		return enemy->toEnter;
	}
	//check if enemy has been knocked out
	if (enemy->knockedOut == true) {
		enemy->toEnter = new KnockOutState;
		return enemy->toEnter;
	}
	if (enemy->doorToUse == NULL) {
		if (enemy->prevState->type != "attack") {
			enemy->toEnter = enemy->prevState;
			return enemy->toEnter;
		}
		else {
			if (enemy->getName() != "enemy_alert") {
				enemy->toEnter = new DefaultState;
				return enemy->toEnter;
			}
			else {
				enemy->toEnter = new AlertState;
				return enemy->toEnter;
			}
		}
	}
	if (enemy->doorToUse->checkOpen() == false) {//if the door get's closed on their face after they've started going through it
		enemy->openDoor();
	}
	if (enemy->prevState->type != "alert" && enemy->prevState->type != "attack") {//enemy is not coming from alert state
		if (enemy->doorToUse->checkLock() == true && enemy->openedDoor == false) {//they couldn't actually open the door, if they have a key they will use it automatically to unlock the door
			if (enemy->startWaitTime == -1 && enemy->moveToDoor(enemy->doorToUse) == true) {//enemy has walked up to door
				enemy->qMark->setVisible(true);
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
				enemy->startWaitTime = time;
			}
			if (enemy->startWaitTime != -1 && time - enemy->startWaitTime >= enemy->doorWaitTime) {
				enemy->flipX();
				enemy->startWaitTime = -1;
				enemy->toEnter = new DefaultState;
				return enemy->toEnter;
			}
		}
		else {
			if (enemy->paused == false) {
				if (abs(enemy->getPositionX() - enemy->doorUsePos) >= enemy->doorToUse->radius + 1) {//enemy has walked through door
					enemy->toEnter = enemy->prevState;
					return enemy->toEnter;
				}
				else {
					enemy->move(Vec2(4.5 * enemy->moveSpeed, 0));
				}
			}
			else {
				enemy->pause(time);
			}
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
			enemy->pathTo(mainLayer, enemy->detectedPlayer->getPositionX(), enemy->detectedPlayer->currentFloor, enemy->detectedPlayer->currentRoom, time, checkForPath);
			enemy->changeSuspicion(enemy->maxSuspicion / (22 SECONDS));
			enemy->isTouched = false;
			enemy->detectedPlayer = NULL;
		}
	}
	else if (enemy->prevState->type == "alert" || enemy->prevState->type == "attack"){//enemy was in alert state, just open door and run, or they were attacking
		if (enemy->doorToUse->checkLock() == true && enemy->openedDoor == false) {//they didn't actually open the door
			//break it down
			//check if enemy has a held item or not
			if (enemy->heldItem == NULL) {
				enemy->heldItem = enemy->fist;//if not, give them a fist
				enemy->addChild(enemy->fist);
			}
			if (enemy->heldItem != NULL) {
				//get distance from door
				if (enemy->flippedX == true) {
					enemy->distanceToDoor = abs((enemy->doorToUse->getPositionX() + enemy->doorToUse->getContentSize().width) - enemy->getPositionX());
				}
				else if (enemy->flippedX == false) {
					enemy->distanceToDoor = abs(enemy->doorToUse->getPositionX() - (enemy->getPositionX() + enemy->getSize().width));
				}
				//check if enemy is in range to attack door
				if (enemy->distanceToDoor <= enemy->heldItem->getRange()) {//enemy is within range to attack door
					enemy->toEnter = new AttackState;
					return enemy->toEnter;
				}
				else {
					enemy->moveToDoor(enemy->doorToUse);
					if (enemy->heldItem == enemy->fist) {//if not in range, discard fist item
						enemy->heldItem = NULL;
						enemy->removeChild(enemy->fist, true);
					}
				}
			}
		}
		else {//if door is unlocked
			if (abs(enemy->getPositionX() - enemy->doorUsePos) > enemy->doorToUse->radius + 1) {//enemy has walked through door
				if (enemy->prevState->type != "attack") {
					enemy->toEnter = enemy->prevState;
					return enemy->toEnter;
				}
				else {
					enemy->toEnter = new AlertState;
					return enemy->toEnter;
				}
			}
			else {
				enemy->move(Vec2(4.5 * enemy->moveSpeed, 0));
			}
		}
	}

	//check if an enemy has become alerted
	if (enemy->suspicionLevel >= enemy->maxSuspicion) {
		if (enemy->detectedTag != -1) {
			enemy->detectedPlayer = static_cast<Player*>(mainLayer->getChildByTag(enemy->detectedTag));
		}
		enemy->toEnter = new AlertState;
		return enemy->toEnter;
	}
	return nullptr;
}
void Enemy::UseDoorState::exit(Enemy* enemy, GameLayer* mainLayer, float time) {
	if (enemy->prevState->type != "alert" && enemy->prevState->type != "attack") {//alert enemies don't close doors behind them, even if they were locked before
		enemy->closeDoor();
	}
	if (enemy->toEnter->type != "attack") {
		enemy->doorToUse = NULL;
	}
	if (enemy->doorToUse != NULL) {
		if (enemy->doorToUse->checkBroken() == true) {
			enemy->doorToUse = NULL;
		}
	}
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
	enemy->inAttackRange = false;
	enemy->paused = false;
	//adding item pick up time to other time trackers
	enemy->pickupStartTime = time;
}
Enemy::State* Enemy::GetItemState::update(Enemy* enemy, GameLayer* mainLayer, float time) {
	if (enemy->checkDead() == true) {
		return new DeathState;
	}
	//check if enemy has been knocked out
	if (enemy->knockedOut == true) {
		return new KnockOutState;
	}
	//check if enemy is walking into a door
	if (enemy->doorToUse != NULL) {
		return new UseDoorState;
	}
	if (enemy->getName() == "enemy_alert") {//if enemy is an alert enemy
		if (enemy->goingToFirstItem == true) {
			enemy->itemToPickUp = enemy->findClosestItem(mainLayer);//update the closest item in case it changes/player has moved in front of it
		}
		else if(enemy->goingToBetterItem == true) {
			enemy->itemToPickUp = enemy->findBetterItem(mainLayer);//update the closest item in case it changes/player has moved in front of it
		}
		else if (enemy->goingToMoreRange == true) {
			//check if player is still not within range
			if (enemy->heldItem != NULL) {
				bool inVerticalRange = false;
				//for stabbing weapons
				if (enemy->heldItem->getAttackType() == Item::STAB) {
					if (enemy->detectedPlayer->getPositionY() <= ((enemy->getPositionY() + enemy->getSize().height / 2) + enemy->heldItem->getRangeY())) {//check vertical range
						inVerticalRange = true;
					}
				}
				//for swinging weapons
				else if (enemy->heldItem->getAttackType() == Item::SWING) {
					if (((enemy->detectedPlayer->getPosition() + Vec2(enemy->detectedPlayer->getSize().width / 2, 0)) - (enemy->getPosition() + enemy->getSize() / 2)).getLength() <= enemy->heldItem->getRangeRadius()) {//check for radial range
						inVerticalRange = true;
					}
				}
				//if they have become in range
				if (inVerticalRange == true) {//you could get to them horizontally but not vertically
					enemy->itemToPickUp = NULL;
				}
				else {
					enemy->itemToPickUp = enemy->findMoreRange(mainLayer);//continue going to weapon with more range
				}
			}
		}
	}
	if (enemy->paused == false) {
		if (enemy->itemToPickUp == NULL || enemy->itemToPickUp->getState() != Item::GROUND) {//the item they were going to pick up was picked up by someone else, or the item they were going to pick up broke somehow...
			if (enemy->prevState->type == "use_door") {//if they are coming from a use door state
				if (enemy->getName() != "enemy_alert") {
					return new DefaultState;//go to default state instead of previous state
				}
				else {
					return new AlertState;//go back to alert state if they are alerted and used a door while getting to an item
				}
			}
			return enemy->prevState;
		}
		if (enemy->moveToObject(enemy->itemToPickUp) == true) {
			enemy->pickUpItem(mainLayer);
			if (enemy->prevState->type == "use_door") {//if they are coming from a use door state
				if (enemy->getName() != "enemy_alert") {
					return new DefaultState;//go to default state instead of previous state
				}
				else {
					return new AlertState;//go back to alert state if they are alerted and used a door while getting to an item
				}
			}
			return enemy->prevState;
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
		if (enemy->getName() != "enemy_alert") {//only if they're not coming from an alert state
			enemy->changeSuspicion(-enemy->maxSuspicion / (60 SECONDS));//takes 30 seconds to drop from half to 0
		}
	}
	if (enemy->getName() != "enemy_alert") {//only if they're not coming from an alert state
	//check if player bumped enemy
		if (enemy->isTouched == true) {
			enemy->startPauseTime = -1;
			enemy->timeToPauseFor = 3.0f;
			enemy->paused = true;
			enemy->wasFlipped = enemy->flippedX;
			enemy->pathTo(mainLayer, enemy->detectedPlayer->getPositionX(), enemy->detectedPlayer->currentFloor, enemy->detectedPlayer->currentRoom, time, checkForPath);
			enemy->changeSuspicion(enemy->maxSuspicion / (22 SECONDS));
			enemy->isTouched = false;
			enemy->detectedPlayer = NULL;
		}
	}
	//check if an enemy has become alerted
	if (enemy->getName() != "enemy_alert") {//only if they're not coming from an alert state
		if (enemy->suspicionLevel >= enemy->maxSuspicion) {
			if (enemy->detectedTag != -1) {
				enemy->detectedPlayer = static_cast<Player*>(mainLayer->getChildByTag(enemy->detectedTag));
			}
			return new AlertState;
		}
	}
	return nullptr;
}
void Enemy::GetItemState::exit(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->returning = true;//only used for enemies not following a path
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

//Search State (for hearing noises):
void Enemy::SearchState::enter(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->inAttackRange = false;
	enemy->paused = false;
	enemy->startPauseTime = -1;
	enemy->qMark->setVisible(true);
}
Enemy::State* Enemy::SearchState::update(Enemy* enemy, GameLayer* mainLayer, float time) {
	if (enemy->checkDead() == true) {
		return new DeathState;
	}
	//check if enemy has been knocked out
	if (enemy->knockedOut == true) {
		return new KnockOutState;
	}
	//check if enemy has seen a dead body or knocked out ally
	if (enemy->bodySeen != NULL) {
		return new SeenBodyState;
	}
	if (enemy->paused == false) {
		if (enemy->reachedLocation == false) {
			if (enemy->pathTo(mainLayer, enemy->noiseLocation->getPositionX(), enemy->noiseLocation->startRoom.y, enemy->noiseLocation->startRoom.x, time, checkForPath) == true) {
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
		enemy->pathTo(mainLayer, enemy->detectedPlayer->getPositionX(), enemy->detectedPlayer->currentFloor, enemy->detectedPlayer->currentRoom, time, checkForPath);
		enemy->changeSuspicion(enemy->maxSuspicion / (22 SECONDS));
		enemy->isTouched = false;
		enemy->detectedPlayer = NULL;
	}
	//check if an enemy has become alerted
	if (enemy->suspicionLevel >= enemy->maxSuspicion) {
		if (enemy->detectedTag != -1) {
			enemy->detectedPlayer = static_cast<Player*>(mainLayer->getChildByTag(enemy->detectedTag));
		}
		return new AlertState;
	}
	//enemy has dropped to 0 supicion
	else if (enemy->suspicionLevel <= 0) {
		return new DefaultState;
	}
	return nullptr;
}
void Enemy::SearchState::exit(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->returning = true;//only used for enemies not following a path
	enemy->noiseLocation = NULL;
	enemy->returning = true;
}

//Seen Body State: (for seeing dead bodies and knocked out enemies)
void Enemy::SeenBodyState::enter(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->inAttackRange = false;
	enemy->paused = false;
	enemy->startPauseTime = -1;
	enemy->qMark->setVisible(true);
	enemy->changeSuspicion(enemy->maxSuspicion / 3);//seeing body increases suspicion by third instantly, more later
	enemy->turnTime = 2.5f;
}
Enemy::State* Enemy::SeenBodyState::update(Enemy* enemy, GameLayer* mainLayer, float time) {
	if (enemy->checkDead() == true) {
		return new DeathState;
	}
	//check if enemy has been knocked out
	if (enemy->knockedOut == true) {
		return new KnockOutState;
	}
	//check if enemy is walking into a door
	if (enemy->doorToUse != NULL) {
		return new UseDoorState;
	}
	if (enemy->paused == false) {
		if (enemy->reachedLocation == false) {
			if (enemy->moveToObject(enemy->bodySeen)== true) {
				enemy->reachedLocation = true;
			}
		}
		else {//they have reached location
			enemy->turnOnSpot(time);//stay where body is and look around
			enemy->changeSuspicion(enemy->maxSuspicion / (30 SECONDS));//suspicion will steadily increase until they become alerted
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
		enemy->pathTo(mainLayer, enemy->detectedPlayer->getPositionX(), enemy->detectedPlayer->currentFloor, enemy->detectedPlayer->currentRoom, time, checkForPath);
		enemy->setSuspicion(enemy->maxSuspicion);
		enemy->isTouched = false;
		enemy->detectedPlayer = NULL;
	}
	//check if an enemy has become alerted
	if (enemy->suspicionLevel >= enemy->maxSuspicion) {
		if (enemy->detectedTag != -1) {
			enemy->detectedPlayer = static_cast<Player*>(mainLayer->getChildByTag(enemy->detectedTag));
		}
		return new AlertState;
	}
	//enemy has dropped to 0 supicion somehow...
	else if (enemy->suspicionLevel <= 0) {
		return new DefaultState;
	}
	return nullptr;
}
void Enemy::SeenBodyState::exit(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->returning = true;
	enemy->bodySeen = NULL;
	enemy->turnTime = 6.0f;
	enemy->reachedLocation = false;
}

//Knock Out State:
void Enemy::KnockOutState::enter(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->inAttackRange = false;
	if (enemy->heldItem != NULL) {
		enemy->heldItem->initHeldItem();
	}
	enemy->dropInventory(mainLayer);
	enemy->setPhysicsBody(enemy->knockedOutBody);
	enemy->getPhysicsBody()->setPositionOffset(Vec2(0, -35));
	enemy->setSpriteFrame(enemy->knockout.animation->getFrames().at(0)->getSpriteFrame());//first frame of the knockout animation
	enemy->startKockOutTime = time;
	enemy->visionEnabled = false;
	enemy->paused = false;
	enemy->ZZZ->setVisible(true);
	enemy->ZZZ->runAction(enemy->ZZZAnimation.action);
	enemy->exMark->setVisible(false);
	enemy->qMark->setVisible(false);
	enemy->changeSuspicion(enemy->maxSuspicion * 0.75);//getting knocked out increases suspicion to 3/4
}
Enemy::State* Enemy::KnockOutState::update(Enemy* enemy, GameLayer* mainLayer, float time) {
	if (enemy->checkDead() == true) {
		return new DeathState;
	}
	if ((time - enemy->startKockOutTime) > (enemy->knockOutTime)) {
		return enemy->prevState;
	}
	else {
		float percentage = abs(((time - enemy->startKockOutTime) / enemy->knockOutTime) - 1);//inverse percentage of time left until enemy wakes up
		enemy->ZZZ->setScale((percentage * 1.2) + 0.25);
	}
	return nullptr;
}
void Enemy::KnockOutState::exit(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->setPhysicsBody(enemy->mainBody);
	enemy->getPhysicsBody()->setPositionOffset(Vec2(0, 35));
	enemy->setSpriteFrame(enemy->stand.animation->getFrames().at(0)->getSpriteFrame());//first frame of the standing animation
	enemy->knockedOut = false;
	enemy->visionEnabled = true;
	enemy->ZZZ->setVisible(false);
	enemy->ZZZ->stopAllActions();
}

//Death State:
void Enemy::DeathState::enter(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->dropInventory(mainLayer);
	enemy->isDead = true;
	enemy->setVisible(false);
}
Enemy::State* Enemy::DeathState::update(Enemy* enemy, GameLayer* mainLayer, float time) {
	return new DefaultState;//they'll never get to it...
	//return nullptr;
}
void Enemy::DeathState::exit(Enemy* enemy, GameLayer* mainLayer, float time) {
	//create dead body here
}
