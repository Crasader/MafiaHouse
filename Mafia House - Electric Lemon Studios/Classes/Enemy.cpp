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
	collision = 47;
	//for attacking without a weapon
	fist = Fist::createWithSpriteFrameName();
	fist->initObject();
	ZZZAnimation = GameAnimation(ZZZs, "icons/ZZZ/%03d.png", 4, 50 FRAMES, true);
}

Vec2 Enemy::getPosition() {
	return Node::getPosition() + Vec2(FRAME_OFFSET, 0);
}

void Enemy::flipX() {
	Character::flipX();
	if (flippedX == true) {
		qMark->setFlippedX(true);
		ZZZ->setFlippedX(true);
		ZZZ->setPositionNormalized(Vec2(0.3, 0.55));
	}
	else {
		qMark->setFlippedX(false);
		ZZZ->setFlippedX(false);
		ZZZ->setPositionNormalized(Vec2(0.55, 0.55));
	}
	fist->knockback *= -1;
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
	ZZZ = Sprite::createWithSpriteFrameName("icons/ZZZ/004.png");
	ZZZ->setAnchorPoint(Vec2(0.5, 0));
	ZZZ->setPositionNormalized(Vec2(0.65, 0.55));
	ZZZ->getTexture()->setTexParameters(texParams);
	ZZZ->setGlobalZOrder(9);
	ZZZ->setVisible(false);
	addChild(ZZZ);

	if (pathTag == "STAND_LEFT" || pathTag == "LEFT") {
		flipX();
	}
	lastSeenLocation = GameObject::create();
	addChild(lastSeenLocation);

	//initializing knocked out physics body
	knockedOutBody = PhysicsBody::createBox(Size(bodySize.width, bodySize.height / 2));
	knockedOutBody->setContactTestBitmask(0xFFFFFFFF);
	knockedOutBody->setTag(555);
	knockedOutBody->setName("enemy");
	knockedOutBody->setCategoryBitmask(2);
	knockedOutBody->setCollisionBitmask(10);
	knockedOutBody->setDynamic(true);
	knockedOutBody->setRotationEnable(false);
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
	if (itemToPickUp->getState() == Item::HELD) {
		itemToPickUp = NULL;
	}
	if (itemToPickUp != NULL && itemToPickUp->getState() == Item::GROUND && itemToPickUp->enemyItem != true) {
		if (itemToPickUp->isKey == false) {//item is not a key
			if (heldItem != NULL && (offhandItem == NULL || (offhandItem != NULL && offhandItem->isKey == false))) {//enemy has a held item already, and offhand item is not a key
				if (offhandItem != NULL) {
					removeChild(offhandItem, true);
				}
				heldItem->initOffhand();
				offhandItem = heldItem;
			}
			else {//held item is not replacing offhand item because it is a key
				if (heldItem != NULL) {
					removeChild(heldItem, true);
				}
			}

			itemToPickUp->removeFromParent();
			heldItem = itemToPickUp;

			addChild(heldItem);
			heldItem->initPickedUpItem();
			inventory.push_back(heldItem);

			itemToPickUp = NULL;

			if (flippedX == true) {
				heldItem->knockback *= -1;
			}
			heldItem->holderFlipped = flippedX;

			heldItem->enemyItem = true;
			if (heldItem->isKey == true) {
				hasKey = true;//if they pickup a key, they have a key to open locked doors
			}
		}
		else {//item is a key
			itemToPickUp->removeFromParent();
			if (offhandItem == NULL || offhandItem->isKey == false) {//enemy doesn't have an offhand item, or it is not a key
				if (offhandItem != NULL) {
					removeChild(offhandItem, true);
				}
				offhandItem = itemToPickUp;
				offhandItem->initOffhand();
				addChild(offhandItem);
			}
			else if (offhandItem->isKey == true && heldItem == NULL) {//they have an offhand item and it is a key already, and they don't have a held item yet
				heldItem = itemToPickUp;
				addChild(heldItem);
				heldItem->initPickedUpItem();
			}
			inventory.push_back(itemToPickUp);

			if (flippedX == true) {
				itemToPickUp->knockback *= -1;
			}
			itemToPickUp->holderFlipped = flippedX;

			itemToPickUp->enemyItem = true;

			hasKey = true;//if they pickup a key, they have a key to open locked doors

			itemToPickUp = NULL;
		}
	}
}

void Enemy::beginUseItem(float angle) {
	Character::beginUseItem(angle);
	if (heldItem != NULL) {
		if (heldItem->getAttackType() == Item::SHOOT) {
			heldItem->prepareShoot(0);
			setSpriteFrame(stab.animation->getFrames().at(1)->getSpriteFrame());//first frame of shooting animation
		}
	}
}
void Enemy::useItem(float angle) {
	Character::useItem(angle);
	if (heldItem != NULL) {
		if (heldItem->getAttackType() == Item::SHOOT) {//always shoot directly at the player
			heldItem->enemyShoot(targetLocation);
		}
	}
}

void Enemy::replaceThrownItem() {
	if (heldItem == NULL) {
		if (offhandItem != NULL) {//have a offhand item that isn't a key
			if (offhandItem->isKey == false) {
				offhandItem->initHeldItem();
				heldItem = offhandItem;//replace held item with offhand item
				offhandItem = NULL;
				if (inventory.size() > 1) {//they have another item in their inventory
					for (int i = 0; i < inventory.size(); i++) {
						if (inventory[i] != heldItem) {
							offhandItem = inventory[i];//set offhand item to new inventory item
							offhandItem->initOffhand();
							addChild(offhandItem);
							break;
						}
					}
				}
			}
			else {//they don't have an offhand item, or it's a key
				if (inventory.size() > 1) {//they have another item in their inventory
					for (int i = 0; i < inventory.size(); i++) {
						if (inventory[i] != offhandItem) {//the item is not their current offhand item
							heldItem = inventory[i];
							heldItem->initHeldItem();//replace held item with inventory item
							addChild(heldItem);
							break;
						}
					}
				}
			}
		}
		else {//they don't have an offhand item, or it's a key
			if (inventory.size() > 1) {//they have another item in their inventory
				for (int i = 0; i < inventory.size(); i++) {
					if (inventory[i] != offhandItem) {//the item is not their current offhand item
						heldItem = inventory[i];
						heldItem->initHeldItem();//replace held item with inventory item
						addChild(heldItem);
						break;
					}
				}
			}
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

void Enemy::Pause(float time) {
	if (startPauseTime == -1) {
		stopAnimation(WALK);
		startAnimation(STAND, stand);
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
	stopAnimation(WALK);
	startAnimation(STAND, stand);
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
	else if (stopTime != -1) {
		if (getPhysicsBody()->getVelocity().x < 15 && getPhysicsBody()->getVelocity().x > -15) {//enemy has stopped moving
			stopAnimation(WALK);
			startAnimation(STAND, stand);
		}
	}
	else if (stopTime == -1){
		stopAnimation(STAND);
		startAnimation(WALK, walking);
		move(Vec2(4.5f, 0) * moveSpeed);
	}
}

bool checkForPathWithLocks(GameLayer* mainlayer, int floorNum, int targetX, int searcherX, bool hasKey) {//always go Tip to Tail, put the thing you wanna get to first
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

bool checkForPath(GameLayer* mainlayer, int floorNum, int targetX, int searcherX, bool hasKey) {//always go Tip to Tail, put the thing you wanna get to first
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
						if (hasKey == false) {//and they don't have a key...
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
						if (hasKey == false) {//and they don't have a key...
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
Stair* Enemy::pathSearch(GameLayer* mainLayer, vector<Stair*> stairs, float xPos, bool(*checkPathFunc)(GameLayer* mainlayer, int floorNum, int targetX, int searcherX, bool hasKey)){
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
				if (checkPathFunc(mainLayer, queryStair->startRoom.y, queryStair->startRoom.x, partnerStair->startRoom.x, checkKey()) == true) {
					//get the distance between the query stair and the partner stair
					float distanceBetweenDoors = abs(partnerStair->getPositionX() - queryStair->getPositionX());

					if (mainLayer->getPartnerStair(queryStair)->startRoom.y == currentFloor) {//partner stair of this query stair is on same floor as you
						//check if there's a path from query's partner stair to you
						if (checkPathFunc(mainLayer, currentFloor, currentRoom, mainLayer->getPartnerStair(queryStair)->startRoom.x, checkKey()) == true) {
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

bool Enemy::pathTo(GameLayer* mainLayer, float positionX, int floorNum, int roomNum, float time, bool (*checkPathFunc)(GameLayer* mainlayer, int floorNum, int targetX, int searcherX, bool hasKey)) {
	//float displacement = positionX - getPositionX();
	Stair* takeStair = NULL;
	vector<Stair*> searchStairs;
	bool directPathToTarget = false;
	vector<Stair*>possibleStairsToTake;

	//target is on same floor as you:
	if (floorNum == currentFloor) {
		//check if enemy has a path to the player
		directPathToTarget = checkPathFunc(mainLayer, currentFloor, roomNum, currentRoom, checkKey());
	}
	//target is on different floor, or you can't get to them from the same floor as them:
	if (floorNum != currentFloor || directPathToTarget == false){
		if (floorNum != currentFloor) {
			for (int i = 0; i < mainLayer->stairs.size(); i++) {
				Stair* queryStair = mainLayer->stairs[i];

				if (queryStair->startRoom.y == floorNum) {//stair is on destination floor
					if (checkPathFunc(mainLayer, floorNum, roomNum, queryStair->startRoom.x, checkKey()) == true) {//check if you can get to the target from this stair
						Stair* partnerStair = mainLayer->getPartnerStair(queryStair);
						
						if (mainLayer->getPartnerStair(queryStair)->startRoom.y == currentFloor) {//partner stair is on your floor
							if (checkPathFunc(mainLayer, currentFloor, partnerStair->startRoom.x, currentRoom, checkKey()) == true) {//check if you can get to the partner stair
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
					if (checkPathFunc(mainLayer, floorNum, roomNum, queryStair->startRoom.x, checkKey()) == true) {//check if you can get to the target from this stair
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

void Enemy::moveFrom(float positionX) {
	float displacement = getPositionX() - positionX;
	Vec2 moveDirection;

	if (displacement < 0) {//you're to the left
		moveDirection = Vec2(-1, 0);
		if (flippedX == false) {
			flipX();
		}
	}
	else if (displacement >= 0) {//you're to the right
		moveDirection = Vec2(1, 0);
		if (flippedX == true) {
			flipX();
		}
	}
	stopAnimation(STAND);
	startAnimation(WALK, walking);
	moveAbsolute(moveDirection * 4.5 * moveSpeed);
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
	stopAnimation(STAND);
	startAnimation(WALK, walking);
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

Vec2 Enemy::furthestDirection() {
	furthestDistance = 0;
	PhysicsRayCastCallbackFunc func = [this](PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data)->bool
	{
		string contactName = info.shape->getBody()->getNode()->getName();
		Node* contact = info.shape->getBody()->getNode();

		if (contactName == "wall") {//raycast stopped by walls and locked doors only
			float distance = contact->getPositionX() - getPositionX();
			if (abs(distance) > abs(furthestDistance)) {
				furthestDistance = distance;
			}
			return false;
		}
		else if (contactName == "door") {
			if (static_cast<Door*>(contact)->checkLock() == true) {//door is locked
				if (hasKey == false) {//and they don't have a key
					float distance = contact->getPositionX() - getPositionX();
					if (abs(distance) > abs(furthestDistance)) {
						furthestDistance = distance;
					}
					return false;
				}
			}
		}
		return true;
	};
	Vec2 currentPosition = getPosition() + getSize() / 2;
	director->getRunningScene()->getPhysicsWorld()->rayCast(func, currentPosition, currentPosition + Vec2(5000, 0), nullptr);//ray cast straight right
	director->getRunningScene()->getPhysicsWorld()->rayCast(func, currentPosition, currentPosition + Vec2(-5000, 0), nullptr);//ray cast straight left

	if (furthestDistance > 0) {//distance was to right
		return Vec2(1, 0);//move right
	}
	else if (furthestDistance < 0) {//distance was to left
		return Vec2(-1, 0);//move left
	}
}

void Enemy::runaway(GameLayer* mainlayer, float time) {
	if (currentFloor == detectedPlayer->currentFloor) {//on same floor as player
		//prevUsedStair = NULL;
		furthestMoveDirection = Vec2(0, 0);
		moveFrom(detectedPlayer->getPositionX());//run from them
		if (stairToUse != NULL) {
			useStair(mainlayer);
			prevUsedStair = mainlayer->getPartnerStair(stairToUse);
			stairToUse = NULL;
		}
	}
	else if (currentFloor != detectedPlayer->currentFloor) {//on different floor from player
		stairToUse = NULL;
		vector<Stair*> possibleStairsToTake;
		for (int i = 0; i < mainlayer->stairs.size(); i++) {
			if (mainlayer->stairs[i]->startRoom.y == currentFloor && mainlayer->stairs[i] != prevUsedStair) {//stair is on same floor as you
				if (checkForPath(mainlayer, currentFloor, mainlayer->stairs[i]->startRoom.x, currentRoom, checkKey()) == true) {//you can get to it
					possibleStairsToTake.push_back(mainlayer->stairs[i]);//potential stair to take
				}
			}
		}
		if (possibleStairsToTake.size() > 0) {
			stairToTake = possibleStairsToTake[0];
			if (possibleStairsToTake.size() > 1) {
				float smallestDiff = abs(getPositionX() - possibleStairsToTake[0]->getPositionX());
				for (int i = 1; i < possibleStairsToTake.size(); i++) {
					float diff = abs(getPositionX() - possibleStairsToTake[i]->getPositionX());
					if (diff < smallestDiff) {
						smallestDiff = diff;
						stairToTake = possibleStairsToTake[i];
					}
				}
			}
			if (!(getPositionX() + bodySize.width >= stairToTake->getPositionX() && getPositionX() <= stairToTake->getPositionX() + stairToTake->getContentSize().width)) {
				moveTo(stairToTake->getPositionX());//move towards stairs
			}
			//if they are at location of stairs:
			else {
				stairToTake->use(this, mainlayer);//use the stairs
				prevUsedStair = mainlayer->getPartnerStair(stairToTake);
			}
		}
		else {//no stairs found
			vector<Enemy*> enemiesToRunTo;
			for (int i = 0; i < enemies.size(); i++) {
				if (enemies[i]->currentFloor == currentFloor && enemies[i]->hp > enemies[i]->maxHP / 2) {//enemy is on same floor as you, and is not running away
					if (checkForPath(mainlayer, currentFloor, enemies[i]->currentRoom, currentRoom, checkKey()) == true) {//you can get to them
						enemiesToRunTo.push_back(enemies[i]);//potential stair to take
					}
				}
			}
			if (enemiesToRunTo.size() > 0) {
				Enemy* enemyToRunTo = enemiesToRunTo[0];
				if (possibleStairsToTake.size() > 1) {
					float smallestDiff = abs(getPositionX() - enemiesToRunTo[0]->getPositionX());
					for (int i = 1; i < enemiesToRunTo.size(); i++) {
						float diff = abs(getPositionX() - enemiesToRunTo[i]->getPositionX());
						if (diff < smallestDiff) {
							smallestDiff = diff;
							enemyToRunTo = enemiesToRunTo[i];
						}
					}
				}
				if (!(getPositionX() + bodySize.width >= enemyToRunTo->getPositionX() && getPositionX() <= enemyToRunTo->getPositionX() + enemyToRunTo->getContentSize().width)) {
					moveTo(enemyToRunTo->getPositionX());//move towards stairs
				}
				//if they are at location of enemy:
				else {
					moveFrom(detectedPlayer->getPositionX());//run from them
				}
			}
			else {//no enemies found either
				if (furthestMoveDirection == Vec2(0, 0)) {
					furthestMoveDirection = furthestDirection();
					if (furthestMoveDirection.x > 0) {
						if (flippedX == true) { flipX(); }
					}
					else if (furthestMoveDirection.x < 0) {
						if (flippedX == false) { flipX(); }
					}
				}
				stopAnimation(STAND);
				startAnimation(WALK, walking);
				moveAbsolute(furthestMoveDirection * 4.5 * moveSpeed);//move in direction in which you can move the furthest
			}
		}
	}

	if (didHitWall == true) {
		canRunAway = false;
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
		if (mainLayer->items[i]->getState() == Item::GROUND && mainLayer->items[i]->isKey == false && mainLayer->items[i]->getRangeRadius() > heldItem->getRangeRadius()) {//item is on ground and is not a key, and has more vertical range than held item
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
					for (int j = 0; j < enemies.size(); j++) {
						if (enemies[j]->itemToPickUp != NULL){
							if (enemies[j]->itemToPickUp == mainLayer->items[j]) {//don't pick up items another enemy is already going to pick up
								continue;
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

void Enemy::noticeItem(Item* item, float time) {
	bool noticeItem = true;
	if (seenItems.size() > 0) {
		for (int i = 0; i < seenItems.size(); i++) {
			if (item == seenItems[i]) {//check if enemy has already seen this item before
				noticeItem = false;
				break;
			}
		}
	}
	if (noticeItem == true) {
		qMark->setVisible(true);
		changeSuspicion(maxSuspicion / 3);//seeing an item increases their suspicion by a third
		seenItems.push_back(item);
		seenTimes.push_back(time);
	}
}

void Enemy::noticeMissingItem(MissingItem* item, float time) {
	bool noticeItem = true;
	if (seenMissingItems.size() > 0) {
		for (int i = 0; i < seenMissingItems.size(); i++) {
			if (item == seenMissingItems[i]) {//check if enemy has already seen this item before
				noticeItem = false;
				break;
			}
		}
	}
	if (noticeItem == true) {
		qMark->setVisible(true);
		changeSuspicion(maxSuspicion / 5);//seeing an item increases their suspicion by a fifth
		seenMissingItems.push_back(item);
		seenMissingTimes.push_back(time);
	}
}

void Enemy::visionRays(vector<Vec2> *points, Vec2* start, float time){
	playerInVision = false;
	didRun = false;
	Vec2 offsetAdjust;

	if (visionEnabled == true) {
		PhysicsRayCastCallbackFunc func = [this, points, offsetAdjust, time](PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data)->bool
		{
			int visionContactTag = info.shape->getBody()->getNode()->getTag();
			string visionContactName = info.shape->getBody()->getNode()->getName();
			Node* visionContact = info.shape->getBody()->getNode();

			//enemy vision is blocked by walls, doors, physical objects
			if (visionContactName == "wall" || visionContactName == "floor" || visionContactName == "ceiling" || visionContactName == "door" || visionContactName == "phys_object" || visionContactName == "hide_radius" || visionContactName == "vent" || visionContactName == "exit_door") {
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
				if (visionContactName == "enemy_alert" && getName() != "enemy_alert") {
					if (time - alertEndTime >= alertCooldown || alertEndTime == -1) {//only become alert if they did not just come out of being alert
						if (suspicionLevel < maxSuspicion) {
							setSuspicion(maxSuspicion + 100);//enemies that see other alerted enemies will become alerted
						}
					}
				}
				points->push_back(info.contact + offsetAdjust);
				didRun = true;
				return false;
			}
			else if (visionContactName == "dead_body") {
				if (static_cast<DeadBody*>(visionContact)->isHidden == false) {
					bodySeen = static_cast<DeadBody*>(visionContact);//they have seen a knocked out enemy
					points->push_back(info.contact + offsetAdjust);
					didRun = true;
					return false;
				}
			}
			//enemy sees the player
			else if (visionContactName == "player") {//not using tag anymore
				if (visionContactTag < 10) {//if the player is not hidden
					static_cast<Player*>(visionContact)->inVision = true;
					static_cast<Player*>(visionContact)->seenTime = time;
					detectedPlayer = static_cast<Player*>(visionContact);
					playerInVision = true;
					points->push_back(info.contact + offsetAdjust);
					didRun = true;
					return false;
				}
			}
			//enemy sees an item
			else if (visionContactName == "item" && getName() != "enemy_alert") {//enemy won't pick up seen items when alert
				if (static_cast<Item*>(visionContact)->enemyCanUse == true) {
					//check if the item is above you
					if (static_cast<Item*>(visionContact)->getPositionY() <= (getPositionY() + getSize().height)) {
						itemToPickUp = static_cast<Item*>(visionContact);//enemy can pick item up
					}
					noticeItem(static_cast<Item*>(visionContact), time);
					points->push_back(info.contact + offsetAdjust);
					didRun = true;
					return false;
				}
				return true;//enemy cannot pick item up
			}
			else if (visionContactName == "missing_item" && getName() != "enemy_alert") {
				noticeMissingItem(static_cast<MissingItem*>(visionContact), time);
				return true;
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
			startPoint = getPosition() + Vec2(bodySize.width - 16, eyeHeight);
			*start = startPoint + Vec2(10, 0);//shift visuals forward a bit
			offsetAdjust = Vec2(-10, 0);//shift end points back by same amount so visual range is accurate
			direction = 1;
		}
		else {
			startPoint = getPosition() + Vec2(16, eyeHeight);
			*start = startPoint + Vec2(-10, 0);//shift visuals forward a bit
			offsetAdjust = Vec2(10, 0);//shift end points back by same amount so visual range is accurate
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

void Enemy::gotHit(Item* item, float time, GameLayer* mainLayer) {
	if (item->didHitWall == false) {
		stopAllActions();
		auto emitter = ParticleFireworks::create();
		emitter->setStartColor(Color4F(255, 0, 0, 1));
		emitter->setEndColor(Color4F(200, 0, 0, 1));//red
		emitter->setDuration(0.1f);
		emitter->setStartSize(4.0f);
		emitter->setStartSizeVar(1.0f);
		emitter->setEndSize(0.5f);
		emitter->setAngleVar(55);
		emitter->setGravity(Vec2(0, -300));
		emitter->setSpeed(75);
		emitter->setSpeedVar(200.0f);
		emitter->setLife(0.05f);
		emitter->setLifeVar(1.5f);
		emitter->setTextureWithRect(frameCache->getSpriteFrameByName("particles/pixel.png")->getTexture(), frameCache->getSpriteFrameByName("particles/pixel.png")->getRect());
		emitter->setGlobalZOrder(30);
		emitter->setPosition(getPosition() + getSize() / 2);
		director->getRunningScene()->addChild(emitter);
		if (item->getAttackType() != Item::SHOOT) {
			item->used();
		}
		if (item->getAttackType() == Item::SHOOT) {
			if (item->getEffect() == Item::KILL) {
				hp -= 100;
			}
			else {
				hp -= 34;
			}
		}
		else {
			hp -= item->dmg;//dealing damage to enemy
		}
		if (item->getEffect() == Item::NONE) {
			wasInHitStun = true;
			pauseSchedulerAndActions();
			hitStunStart = time;
			hitStunTime = item->hitstun;
			setSuspicion(maxSuspicion);
			if ((flippedX == true && item->holderFlipped == true) || (flippedX == false && item->holderFlipped == false)) {//if the enemy is hit from behind
				flipX();
			}
		}
		if (touchingWall == false) {
			stop();
			moveAbsoluteNoLimit(item->knockback);
		}
		if (item->getEffect() == Item::KILL) {
			if (getName() != "enemy_alert") {//hitting unalerted enemies with killing weapons will always kill them instantly
				hp = 0;
			}
			else {
				if (knockedOut == false) {
					createNoise(130, 1.5, time, getPosition() + Vec2(getSize().width / 2, getSize().height), Vec2(currentRoom, currentFloor), "enemy_wail", &mainLayer->noises);
					changeSuspicion(maxSuspicion);
					wasInHitStun = true;
					pauseSchedulerAndActions();
					hitStunStart = time;
					hitStunTime = item->hitstun;
				}
			}
		}
		else if(item->getEffect() == Item::KNOCKOUT) {
			if (knockedOut == false) {
				knockedOut = true;
				knockOutTime = baseKnockOutTime * static_cast<float>(item->dmg);//more powerful items knock out for longer
				knockOutTime = knockOutTime < minKnockOuttime ? minKnockOuttime : knockOutTime;//set minimum knockOutTime to 20 seconds
				if (getName() == "enemy_alert") {
					createNoise(130, 1.5, time, getPosition() + Vec2(getSize().width / 2, getSize().height), Vec2(currentRoom, currentFloor), "enemy_wail", &mainLayer->noises);
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
	if (offhandItem != NULL) {
		if (flippedX == true) {
			offhandItem->rotatePickUpRadius(-90);
		}
		else {
			offhandItem->rotatePickUpRadius(0);
		}
	}
	//checking if enemy still has a key, also removing stolen items
	if (inventory.size() > 0) {
		hasKey = false;
		for (int i = 0; i < inventory.size(); i++) {
			if (inventory[i]->enemyItem == false) {//item has been stolen
				inventory.erase(inventory.begin() + i);//remove it from their inventory
				i--;
				continue;
			}
			if (inventory[i]->isKey == true) {//if the item is a key
				hasKey = true;
			}
		}
	}
	//forgetting what missing items they have seen
	if (seenMissingItems.size() > 0) {
		for (int i = 0; i < seenMissingItems.size(); i++) {
			if (time - seenMissingTimes[i] >= memoryTime) {
				seenMissingTimes.erase(seenMissingTimes.begin() + i);
				seenMissingItems.erase(seenMissingItems.begin() + i);
				i--;
			}
		}
	}
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
	//checking if an item has fallen on them
	if (fallenItem != NULL && getName() != "enemy_alert") {
		noticeItem(fallenItem, time);
	}
	//checking invincibility frames
	if (invincible == true && time - hitTime >= invicibilityTime) {
		invincible = false;
	}
	//checking if they've been hit
	if (itemHitBy != NULL ) {
		if (itemHitBy->isUnderObject == false) {
			if (invincible == false) {
				if (itemHitBy->getState() == Item::THROWN || itemHitBy->getState() == Item::FALLING) {
					if (itemHitBy == thrownItem && (time - itemHitBy->thrownTime >= thrownItemDelay)) {
						gotHit(itemHitBy, time, mainLayer);
						hitTime = time;
						invincible = true;
						thrownItem = NULL;
					}
					else if (itemHitBy != thrownItem) {
						gotHit(itemHitBy, time, mainLayer);
						hitTime = time;
						invincible = true;
					}
				}
				else {
					gotHit(itemHitBy, time, mainLayer);
					hitTime = time;
					invincible = true;
				}
			}
		}
		itemHitBy = NULL;
	}
	if (time - hitStunStart >= hitStunTime || hitStunStart == -1) {//only update if hitstun is over, of if histun never began
		resumeSchedulerAndActions();
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
	enemy->canRunAway = true;
	enemy->inAttackRange = false;
	enemy->exMark->setVisible(false);
	enemy->paused = false;
	enemy->moveSpeed = 1.0f;
	enemy->setSpeed(enemy->moveSpeed);
	enemy->turnTime = enemy->defaultTurnTime;
	enemy->waitTime = enemy->defaultWaitTime;
	enemy->walkTime = enemy->defaultWalkTime;
	enemy->setName("enemy");
	enemy->getPhysicsBody()->setCollisionBitmask(13);
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
	//check if enemy has been hit by a slow thrown item
	if (enemy->itemBumpedBy != NULL) {
		if (enemy->directionHitFrom.x < 0) {//hit from the right
			if (enemy->flippedX == true) {
				enemy->flipX();
			}
		}
		else if (enemy->directionHitFrom.x > 0) {//hit from the left
			if (enemy->flippedX == false) {
				enemy->flipX();
			}
		}
		enemy->timeToPauseFor = 7.0f;
		enemy->paused = true;
		enemy->wasFlipped = enemy->flippedX;
		enemy->changeSuspicion(enemy->maxSuspicion / 2);
		enemy->itemBumpedBy = NULL;
	}
	if (enemy->noiseLocation != Vec2(0, 0)) {
		return new SearchState;
	}
	//check if an enemy has become alerted
	if (enemy->suspicionLevel >= enemy->maxSuspicion) {
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
		enemy->Pause(time);
	}
	return nullptr;
}

//Suspicious State:
void Enemy::SuspectState::enter(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->canRunAway = true;
	enemy->inAttackRange = false;
	enemy->prevPauseTime = time;
	enemy->exMark->setVisible(false);
	enemy->qMark->setVisible(true);
	enemy->moveSpeed = 1.65f;
	enemy->setSpeed(enemy->moveSpeed);
	enemy->walkTime = enemy->defaultWalkTime * 0.65f;
	enemy->waitTime = enemy->defaultWaitTime * 0.65f;
	enemy->setName("enemy");
	enemy->getPhysicsBody()->setCollisionBitmask(13);
	enemy->turnTime = enemy->defaultTurnTime * 0.65f;
	//enemy->visionDegrees = enemy->defaultDegrees * 1.1;
	enemy->visionRadius = enemy->defaultRadius * 1.3;
	if (enemy->prevState->type == "alert") {
		enemy->alertEndTime = time;
	}
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
	//check if enemy has been hit by a slow thrown item
	if (enemy->itemBumpedBy != NULL) {
		if (enemy->directionHitFrom.x < 0) {//hit from the right
			if (enemy->flippedX == true) {
				enemy->flipX();
			}
		}
		else if (enemy->directionHitFrom.x > 0) {//hit from the left
			if (enemy->flippedX == false) {
				enemy->flipX();
			}
		}
		enemy->timeToPauseFor = 7.0f;
		enemy->paused = true;
		enemy->wasFlipped = enemy->flippedX;
		enemy->changeSuspicion(enemy->maxSuspicion / 2);
		enemy->itemBumpedBy = NULL;
	}
	//check if enemy has heard a noise
	if (enemy->noiseLocation != Vec2(0, 0)) {
		return new SearchState;
	}
	//check if an enemy has become alerted
	if (enemy->suspicionLevel >= enemy->maxSuspicion) {
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
		enemy->changeSuspicion(-1 * enemy->maxSuspicion / (15 SECONDS));
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
		if (randNum(1, 100) % 30 == 0) {//3% chance to stop, 3/100 nums are divisible by 30
			enemy->wasFlipped = enemy->flippedX;
			enemy->timeToPauseFor = static_cast<float>(randNum(1, 6));//will Pause for between 1 and 6 seconds
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
		enemy->Pause(time);
	}

	return nullptr;
}
void Enemy::SuspectState::exit(Enemy* enemy, GameLayer* mainLayer, float time) {
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
	enemy->getPhysicsBody()->setCollisionBitmask(29);
	enemy->lostPlayer = false;
	enemy->reachedLastSeen = false;
	//enemy->visionDegrees = enemy->defaultDegrees * 1.1;
	enemy->visionRadius = enemy->defaultRadius * 1.7;
	if (enemy->prevState->type != "use_door" && enemy->prevState->type != "attack" && enemy->prevState->type != "get_item") {
		mainLayer->numTimesDetected++;
		enemy->createNoise(180, 2, time, enemy->getPosition() + Vec2(enemy->getSize().width / 2, enemy->getSize().height), Vec2(enemy->currentRoom, enemy->currentFloor), "enemy_shout", &mainLayer->noises);
	}
}
Enemy::State* Enemy::AlertState::update(Enemy* enemy, GameLayer* mainLayer, float time) {
	if (enemy->hp <= enemy->maxHP / 2) {
		enemy->runningAway = true;
	}
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
	if (enemy->runningAway == false) {//if enemy has more than half hp
		//check if enemy has no weapon
		if (enemy->itemToPickUp == NULL && (enemy->heldItem == NULL || enemy->heldItem->isKey == true)) {
			if (enemy->detectedPlayer->heldItem != NULL && enemy->detectedPlayer->heldItem->isKey == false) {//player has a held item and it is not a key
				if (enemy->detectedPlayer->currentFloor == enemy->currentFloor && checkForPath(mainLayer, enemy->currentFloor, enemy->detectedPlayer->currentRoom, enemy->currentRoom, enemy->checkKey()) == true) {
					enemy->itemToPickUp = (enemy->findClosestItem(mainLayer));//find a weapon that is closest to them
				}
				if (enemy->itemToPickUp != NULL) {//if one was found
					enemy->goingToFirstItem = true;
					return new GetItemState;//go and get it
				}
			}
		}
		//check if there's a better weapon to pick up
		else if (enemy->itemToPickUp == NULL && enemy->heldItem != NULL && enemy->heldItem->isKey == false) {//enemy already has an item that isn't a key
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
	}
	//check if enemy has run into an item
	if (enemy->itemToPickUp != NULL && (enemy->heldItem == NULL || enemy->heldItem->isKey == true)) {//enemy doesn't have a held item or it is a key
		enemy->itemToPickUp = NULL;
		//enemy->pickUpItem(mainLayer);//removing for now, could be annoying
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
		enemy->changeSuspicion(-enemy->maxSuspicion / (35 SECONDS));
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
			enemy->lastSeenLocation->currentFloor = enemy->detectedPlayer->currentFloor;
			enemy->lastSeenLocation->currentRoom = enemy->detectedPlayer->currentRoom;
			enemy->lastSeenLocation->setPosition(enemy->detectedPlayer->getPosition());
		}
	}

	if (enemy->runningAway == false || enemy->canRunAway == false) {//enemy has more than half hp still, or they cannot run away anymore
	//enemy still knows where they player is
		if (enemy->lostPlayer == false) {
			if (enemy->canRunAway == false) {
				enemy->turnTime = 1.0f;
				enemy->turnOnSpot(time);
			}
			else {
				enemy->pathTo(mainLayer, enemy->detectedPlayer->getPositionX(), enemy->detectedPlayer->currentFloor, enemy->detectedPlayer->currentRoom, time, checkForPath);
			}
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
					if (checkForPath(mainLayer, enemy->currentFloor, enemy->detectedPlayer->currentRoom, enemy->currentRoom, false) == true) {//enemy is in same room as player
						Vec2 displacement = (enemy->detectedPlayer->getPosition() + enemy->detectedPlayer->getSize() / 2) - (enemy->getPosition() + enemy->getSize() / 2);
						bool doorInbetween = false;
						for (int i = 0; i < mainLayer->doors.size(); i++) {//checking if door is inbetween enemy and player
							if (mainLayer->doors[i]->getName() == "door" && mainLayer->doors[i]->checkOpen() == false && mainLayer->doors[i]->startRoom.y == enemy->currentFloor) {
								if (displacement.x < 0) {//player is to the left
									if (mainLayer->doors[i]->getPositionX() < enemy->getPositionX() && mainLayer->doors[i]->getPositionX() > enemy->detectedPlayer->getPositionX()) {//door is between enemy and player
										doorInbetween = true;
										break;
									}
								}
								else {//player is to the right
									if (mainLayer->doors[i]->getPositionX() > enemy->getPositionX() && mainLayer->doors[i]->getPositionX() < enemy->detectedPlayer->getPositionX()) {//door is between enemy and player
										doorInbetween = true;
										break;
									}
								}
							}
						}
						if (doorInbetween == false) {
							//check vertical range
							if (displacement.getLength() <= enemy->heldItem->getRangeRadius()) {//check for radial range
								enemy->inAttackRange = true;
								float angle = displacement.getAngle() * 180 / M_PI;
								if ((angle > -22.5 && angle <= 22.5) || (angle > 157.5 || angle <= -157.5)) {
									enemy->aimAngle = 0;
								}
								else if ((angle > -67.5 && angle <= -22.5) || (angle > -157.5 && angle <= -112.5)) {
									enemy->aimAngle = 45;
								}
								else if ((angle > 22.5 && angle <= 67.5) || (angle > 112.5 && angle <= 157.5)) {
									enemy->aimAngle = 315;
								}
								else if ((angle > 67.5 && angle <= 112.5)) {
									enemy->aimAngle = 270;
								}
								else if ((angle > -112.5 && angle <= -67.5)) {
									enemy->aimAngle = 90;
								}
							}
							else {
								inVerticalRange = false;
							}
						}
					}
				}
				if (enemy->inAttackRange == true) {
					if (enemy->heldItem->getAttackType() == Item::SHOOT) {
						enemy->targetLocation = enemy->detectedPlayer->getPosition() + (enemy->detectedPlayer->getSize() / 2);
					}
					return new AttackState;
				}
				else {//in range == false
					if (enemy->runningAway == false) {
						if (inVerticalRange == false) {//you could get to them horizontally but not vertically
							//find an item with more vertical range
							enemy->itemToPickUp = (enemy->findMoreRange(mainLayer));//find a weapon that is closest to them
							if (enemy->itemToPickUp != NULL) {//if one was found
								enemy->goingToMoreRange = true;
								if (enemy->heldItem == enemy->fist) {//if not in range, discard fist item
									enemy->heldItem = NULL;
									enemy->removeChild(enemy->fist, true);
								}
								return new GetItemState;//go and get it
							}
						}
					}
					if (enemy->heldItem == enemy->fist) {//if not in range, discard fist item
						enemy->heldItem = NULL;
						enemy->removeChild(enemy->fist, true);
					}
				}
			}
		}
		//enemy didn't see player hide
		else {
			if (enemy->reachedLastSeen == false) {
				enemy->reachedLastSeen = enemy->pathTo(mainLayer, enemy->lastSeenLocation->getPositionX(), enemy->lastSeenLocation->currentFloor, enemy->lastSeenLocation->currentRoom, time, checkForPath);
			}
			else {//they have reached player's last seen location
				enemy->walk(time);
			}
		}
	}
	else {//enemy has less than half hp, and can run away
		enemy->inAttackRange = false;
		if (enemy->heldItem != NULL) {
			if (enemy->heldItem->isKey == false) {
				Vec2 displacement = (enemy->detectedPlayer->getPosition() + enemy->detectedPlayer->getSize() / 2) - (enemy->getPosition() + enemy->getSize() / 2);//displacement between enemy and player
				if ((displacement.getLength() <= 200 && displacement.getLength() > 150) && enemy->currentFloor == enemy->detectedPlayer->currentFloor) {//check if player is in throw range, and they're on the same floor as you
					float angle = displacement.getAngle() * 180 / M_PI;
					if ((angle > -22.5 && angle <= 22.5) || (angle > 157.5 || angle <= -157.5)) {
						enemy->aimAngle = 0;
					}
					else if ((angle > -67.5 && angle <= -22.5) || (angle > -157.5 && angle <= -112.5)) {
						enemy->aimAngle = 45;
					}
					else if ((angle > 22.5 && angle <= 67.5) || (angle > 112.5 && angle <= 157.5)) {
						enemy->aimAngle = 315;
					}
					else if ((angle > 67.5 && angle <= 112.5)) {
						enemy->aimAngle = 270;
					}
					else if ((angle > -112.5 && angle <= -67.5)) {
						enemy->aimAngle = 90;
					}
					if (displacement.x < 0) {//player it to the left
						if (enemy->flippedX == false) {
							enemy->flipX();
						}
					}
					else if (displacement.x > 0) {//player is to the right
						if (enemy->flippedX == true) {
							enemy->flipX();
						}
					}
					if (enemy->heldItem->getAttackType() != Item::SHOOT) {
						return new ThrowState;
					}
					else {//enemy has a gun
						enemy->targetLocation = enemy->detectedPlayer->getPosition() + (enemy->detectedPlayer->getSize() / 2);
						return new AttackState;
					}
				}
			}
		}

		enemy->runaway(mainLayer, time);
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
	enemy->bodySeen = NULL;
	if (enemy->heldItem == enemy->fist) {//if not in range, discard fist item
		enemy->heldItem = NULL;
		enemy->removeChild(enemy->fist, true);
	}
}

//Attack State(using items):
void Enemy::AttackState::enter(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->stopAllActions();
	if (enemy->heldItem == NULL) {
		enemy->heldItem = enemy->fist;//if not, give them a fist
		enemy->addChild(enemy->fist);
	}
	enemy->stop();
	enemy->attackPrepareTime = time;
	enemy->beginUseItem(enemy->aimAngle);;
}
Enemy::State* Enemy::AttackState::update(Enemy* enemy, GameLayer* mainLayer, float time) {
	if (enemy->wasInHitStun == true) {
		return enemy->prevState;
	}
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
		enemy->finishUseItem();
		enemy->attackEndTime = time;
		enemy->attackStartTime = -1.0f;
	}
	if (enemy->attackEndTime != -1.0f && time - enemy->attackEndTime >= enemy->heldItem->getLagTime()) {
		enemy->attackEndTime = -1.0f;
		return enemy->prevState;
	}
	return nullptr;
}
void Enemy::AttackState::exit(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->attackPrepareTime = -1.0f;
	enemy->attackStartTime = -1.0f;
	enemy->attackEndTime = -1.0f;
	enemy->heldItem->didHitWall = false;
	//if (enemy->heldItem->hp <= 0) {enemy->breakItem(mainLayer);}//enemy items don't break
	enemy->heldItem->initHeldItem();
	if (enemy->heldItem == enemy->fist) {
		enemy->heldItem = NULL;
		enemy->removeChild(enemy->fist, true);
	}
}

//Throw State:
void Enemy::ThrowState::enter(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->stopAllActions();
	enemy->stop();
	enemy->attackStartTime = -1;
	enemy->attackEndTime = -1;
	enemy->attackPrepareTime = time;
	enemy->beginThrowItem();
}
Enemy::State* Enemy::ThrowState::update(Enemy* enemy, GameLayer* mainLayer, float time) {
	if (enemy->checkDead() == true) {
		return new DeathState;
	}
	//check if enemy has been knocked out
	if (enemy->knockedOut == true) {
		return new KnockOutState;
	}

	if (enemy->attackPrepareTime != -1.0f && time - enemy->attackPrepareTime >= enemy->heldItem->getStartTime()) {
		enemy->attackStartTime = time;
		enemy->throwItem(mainLayer, time);
		enemy->attackPrepareTime = -1.0f;
	}
	if (enemy->attackStartTime != -1.0f && time - enemy->attackStartTime >= enemy->thrownItem->getAttackTime()) {
		enemy->attackEndTime = time;
		enemy->attackStartTime = -1.0f;
	}
	if (enemy->attackEndTime != -1.0f && time - enemy->attackEndTime >= enemy->thrownItem->getLagTime()) {
		enemy->attackEndTime = -1.0f;
		return enemy->prevState;
	}

	return nullptr;
}
void Enemy::ThrowState::exit(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->aimAngle = 0;
	enemy->replaceThrownItem();
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
				enemy->returning = true;
				enemy->noiseLocation = Vec2(0, 0);
				enemy->itemToPickUp = NULL;
				enemy->bodySeen = NULL;
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
				enemy->Pause(time);
			}
		}

		//checking if enemy spotted player
		if (enemy->seeingPlayer() == true) {
			enemy->changeSuspicion(enemy->maxSuspicion / (0.6f SECONDS));//increases 1/45th of max every frame, takes 45 frames to alert guard
		}
		else {
			if (enemy->runningAway == false || enemy->canRunAway == false) {
				enemy->changeSuspicion(-enemy->maxSuspicion / (60 SECONDS));//takes 30 seconds to drop from half to 0
			}
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
			if (enemy->hp >= enemy->maxHP / 2 || enemy->canRunAway == false) {//they have more than half hp
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
						enemy->targetLocation = enemy->doorToUse->getPosition() + enemy->doorToUse->getContentSize() / 2;
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
			else if (enemy->currentFloor == enemy->detectedPlayer->currentFloor){//they have less than half hp, and player is on same floor as enemy
				if (checkForPath(mainLayer, enemy->currentFloor, enemy->currentRoom, enemy->detectedPlayer->currentRoom, false) == true) {
					enemy->canRunAway = false;
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
					if (((enemy->detectedPlayer->getPosition() + Vec2(enemy->detectedPlayer->getSize().width / 2, 0)) - (enemy->getPosition() + enemy->getSize() / 2)).getLength() <= enemy->heldItem->getRangeRadius()) {//check for radial range
						inVerticalRange = true;
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
		enemy->Pause(time);
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
	if (enemy->prevState->type != "use_door") {
		enemy->changeSuspicion(enemy->maxSuspicion / 4);//hearing a noise increases suspicion by quarter instantly, more later
	}
	enemy->moveSpeed = 1.65f;
	enemy->setSpeed(enemy->moveSpeed);
	enemy->walkTime = enemy->defaultWalkTime * 0.65f;
	enemy->waitTime = enemy->defaultWaitTime * 0.65f;
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
		enemy->noiseLocation = Vec2(0, 0);
		enemy->noiseRoom = Vec2(0, 0);
		return new SeenBodyState;
	}
	if (enemy->paused == false) {
		if (enemy->reachedLocation == false) {
			if (enemy->pathTo(mainLayer, enemy->noiseLocation.x, enemy->noiseRoom.y, enemy->noiseRoom.x, time, checkForPath) == true) {
				enemy->reachedLocation = true;
				enemy->noiseLocation = Vec2(0, 0);
				enemy->noiseRoom = Vec2(0, 0);
			}
		}
		else {//they have reached location
			enemy->walk(time);//start patrolling the area
			enemy->changeSuspicion(-1 * enemy->maxSuspicion / (40 SECONDS));
		}
		//check if enemy is walking into a door
		if (enemy->doorToUse != NULL) {
			return new UseDoorState;
		}
	}
	else {
		enemy->Pause(time);
	}

	//checking if enemy spotted player
	if (enemy->seeingPlayer() == true) {
		enemy->changeSuspicion(enemy->maxSuspicion / (0.5f SECONDS));//takes half a secobnd to reach max suspicion from 0
	}
	//check if player bumped enemy
	if (enemy->isTouched == true) {
		enemy->startPauseTime = -1;
		enemy->timeToPauseFor = 3.0f;
		enemy->paused = true;
		enemy->wasFlipped = enemy->flippedX;
		enemy->pathTo(mainLayer, enemy->detectedPlayer->getPositionX(), enemy->detectedPlayer->currentFloor, enemy->detectedPlayer->currentRoom, time, checkForPath);
		enemy->changeSuspicion(enemy->maxSuspicion / 5);
		enemy->isTouched = false;
		enemy->detectedPlayer = NULL;
	}
	//check if an enemy has become alerted
	if (enemy->suspicionLevel >= enemy->maxSuspicion) {
		enemy->noiseLocation = Vec2(0, 0);
		enemy->noiseRoom = Vec2(0, 0);
		return new AlertState;
	}
	//enemy has dropped to 0 supicion
	else if (enemy->suspicionLevel <= 0) {
		enemy->noiseLocation = Vec2(0, 0);
		enemy->noiseRoom = Vec2(0, 0);
		return new DefaultState;
	}
	return nullptr;
}
void Enemy::SearchState::exit(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->returning = true;//only used for enemies not following a path
}

//Seen Body State: (for seeing dead bodies and knocked out enemies)
void Enemy::SeenBodyState::enter(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->inAttackRange = false;
	enemy->paused = false;
	enemy->startPauseTime = -1;
	enemy->qMark->setVisible(true);
	enemy->moveSpeed = 1.65f;
	enemy->setSpeed(enemy->moveSpeed);
	enemy->turnTime = 2.2f;
	if (enemy->prevState->type != "use_door") {
		enemy->changeSuspicion(enemy->maxSuspicion / 3);//seeing body increases suspicion by third instantly, more later
		enemy->createNoise(180, 2, time, enemy->getPosition() + Vec2(enemy->getSize().width / 2, enemy->getSize().height), Vec2(enemy->currentRoom, enemy->currentFloor), "enemy_shout", &mainLayer->noises);
	}
}
Enemy::State* Enemy::SeenBodyState::update(Enemy* enemy, GameLayer* mainLayer, float time) {
	if (enemy->checkDead() == true) {
		return new DeathState;
	}
	//check if enemy has been knocked out
	if (enemy->knockedOut == true) {
		enemy->bodySeen = NULL;
		return new KnockOutState;
	}
	//check if enemy is walking into a door
	if (enemy->doorToUse != NULL) {
		return new UseDoorState;
	}
	if (enemy->bodySeen == NULL) {
		return new DefaultState;
	}
	if (enemy->paused == false) {
		if (enemy->reachedLocation == false) {
			if (enemy->moveToObject(enemy->bodySeen)== true) {
				enemy->reachedLocation = true;
			}
		}
		else {//they have reached location
			enemy->turnOnSpot(time);//stay where body is and look around
			enemy->changeSuspicion(enemy->maxSuspicion / (25 SECONDS));//suspicion will steadily increase until they become alerted
		}
	}
	else {
		enemy->Pause(time);
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
		enemy->bodySeen = NULL;
		return new AlertState;
	}
	//enemy has dropped to 0 supicion somehow...
	else if (enemy->suspicionLevel <= 0) {
		enemy->bodySeen = NULL;
		return new DefaultState;
	}
	return nullptr;
}
void Enemy::SeenBodyState::exit(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->returning = true;
	enemy->reachedLocation = false;
}

//Knock Out State:
void Enemy::KnockOutState::enter(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->stopAllActions();
	enemy->inAttackRange = false;
	if (enemy->heldItem != NULL) {
		enemy->heldItem->initHeldItem();
	}
	enemy->dropInventory(mainLayer);
	enemy->knockedOutBody->setVelocity(enemy->getPhysicsBody()->getVelocity());
	enemy->stop();
	enemy->setPhysicsBody(enemy->knockedOutBody);
	enemy->getPhysicsBody()->setPositionOffset(Vec2(0, -25));
	enemy->startAnimation(KNOCKOUT, enemy->knockout);//running knockout animation
	enemy->startKockOutTime = time;
	enemy->visionEnabled = false;
	enemy->paused = false;
	enemy->exMark->setVisible(false);
	enemy->qMark->setVisible(false);
	enemy->changeSuspicion(enemy->maxSuspicion * 0.25);//getting knocked out increases suspicion by 1/4
}
Enemy::State* Enemy::KnockOutState::update(Enemy* enemy, GameLayer* mainLayer, float time) {
	if (enemy->checkDead() == true) {
		enemy->toEnter = new DeathState;
		return enemy->toEnter;
	}
	if (enemy->getActionByTag(KNOCKOUT) == NULL) {
		if (enemy->ZZZ->getActionByTag(ZZZs) == NULL) {
			enemy->ZZZ->runAction(enemy->ZZZAnimation.action);
			enemy->ZZZ->setVisible(true);
		}
	}
	if ((time - enemy->startKockOutTime) > (enemy->knockOutTime)) {
		if (enemy->prevState->type == "attack" || enemy->prevState->type == "knockout") {
			enemy->toEnter = new DefaultState;
			return enemy->toEnter;
		}
		enemy->toEnter = enemy->prevState;
		return enemy->toEnter;
	}
	else {
		float percentage = abs(((time - enemy->startKockOutTime) / enemy->knockOutTime) - 1);//inverse percentage of time left until enemy wakes up
		enemy->ZZZ->setScale((percentage * 1.2) + 0.25);
	}
	return nullptr;
}
void Enemy::KnockOutState::exit(Enemy* enemy, GameLayer* mainLayer, float time) {
	if (enemy->toEnter->type != "death") {
		enemy->mainBody->setVelocity(enemy->getPhysicsBody()->getVelocity());
		enemy->stop();
		enemy->setPhysicsBody(enemy->mainBody);
		enemy->getPhysicsBody()->setPositionOffset(Vec2(0, 0));
		enemy->setSpriteFrame(enemy->stand.animation->getFrames().at(0)->getSpriteFrame());//first frame of the standing animation
		enemy->visionEnabled = true;
	}
	enemy->knockedOut = false;
	enemy->ZZZ->setVisible(false);
	enemy->ZZZ->stopAllActions();
}

//Death State:
void Enemy::DeathState::enter(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->stopAllActions();
	enemy->visionEnabled = false;
	enemy->dropInventory(mainLayer);
	if (enemy->prevState->type == "knockout") {
		enemy->startAnimation(DEATH, enemy->knockoutDeath);
	}
	else {
		if (enemy->suspicionLevel >= enemy->maxSuspicion / 2) {
			enemy->createNoise(250, 3, time, enemy->getPosition() + Vec2(enemy->getSize().width / 2, enemy->getSize().height), Vec2(enemy->currentRoom, enemy->currentFloor), "enemy_scream", &mainLayer->noises);
		}
		enemy->startAnimation(DEATH, enemy->dying);
	}
	enemy->getPhysicsBody()->setDynamic(false);
	enemy->stop();
}
Enemy::State* Enemy::DeathState::update(Enemy* enemy, GameLayer* mainLayer, float time) {
	if (enemy->getActionByTag(DEATH) == NULL) {//dying animation has finished
		return new DefaultState;//they'll never get to it...
	}
	return nullptr;
}
void Enemy::DeathState::exit(Enemy* enemy, GameLayer* mainLayer, float time) {
	enemy->isDead = true;
	//create dead body here
	DeadBody* newBody = DeadBody::createWithSpriteFrameName(enemy->deadBodyName);
	newBody->initObject(enemy->getPosition(), enemy->deadBodyOutlineName);
	mainLayer->addChild(newBody);
	mainLayer->bodies.push_back(newBody);
}

Thug::Thug() {
	//proeprties
	eyeHeight = 88;
	defaultDegrees = 60;
	visionDegrees = defaultDegrees;//width of angle of vision
	defaultRadius = 180;
	visionRadius = defaultRadius;//how far vision reaches
	baseSpeed = 50;
	maxSpeed = baseSpeed;
	deadBodyName = "enemy/thug/dead.png";
	deadBodyOutlineName = "enemy/thug/dead_outline.png";
	//initializing animations:
	stand = GameAnimation(STAND, "enemy/thug/stand/%03d.png", 15, 10 FRAMES, true);
	walking = GameAnimation(WALK, "enemy/thug/walk/%03d.png", 7, 10 FRAMES, true);
	knockout = GameAnimation(KNOCKOUT, "enemy/thug/knockdown/%03d.png", 4, 20 FRAMES, false);
	knockoutDeath = GameAnimation(DEATH, "enemy/thug/knockdown_die/%03d.png", 2, 20 FRAMES, false);
	dying = GameAnimation(DEATH, "enemy/thug/die/%03d.png", 4, 20 FRAMES, false);
	stab = GameAnimation(STAB, "enemy/thug/stab/%03d.png", 2, 10 FRAMES, false);
	swing = GameAnimation(SWING, "enemy/thug/swing/%03d.png", 2, 8 FRAMES, false);
	throwing = GameAnimation(THROW, "enemy/thug/throw/%03d.png", 2, 6 FRAMES, false);
}

Guard::Guard() {
	//proeprties
	isGuard = true;
	eyeHeight = 84;
	defaultDegrees = 60;
	visionDegrees = defaultDegrees;//width of angle of vision
	defaultRadius = 190;
	visionRadius = defaultRadius;//how far vision reaches
	baseSpeed = 55;
	maxSpeed = baseSpeed;
	defaultTurnTime = 3.5f;
	defaultWalkTime = 4.5f;
	baseKnockOutTime = 0.8f;
	minKnockOuttime = 6.0f;
	deadBodyName = "enemy/guard/dead.png";
	deadBodyOutlineName = "enemy/guard/dead_outline.png";
	//initializing animations:
	stand = GameAnimation(STAND, "enemy/guard/stand/%03d.png", 11, 10 FRAMES, true);
	walking = GameAnimation(WALK, "enemy/guard/walk/%03d.png", 8, 8 FRAMES, true);
	knockout = GameAnimation(KNOCKOUT, "enemy/guard/knockdown/%03d.png", 5, 15 FRAMES, false);
	knockoutDeath = GameAnimation(DEATH, "enemy/guard/knockdown_die/%03d.png", 3, 15 FRAMES, false);
	dying = GameAnimation(DEATH, "enemy/guard/die/%03d.png", 5, 15 FRAMES, false);
	stab = GameAnimation(STAB, "enemy/guard/stab/%03d.png", 2, 10 FRAMES, false);
	swing = GameAnimation(SWING, "enemy/guard/swing/%03d.png", 2, 8 FRAMES, false);
	throwing = GameAnimation(THROW, "enemy/guard/throw/%03d.png", 2, 6 FRAMES, false);
}

Boss::Boss() {
	//proeprties
	isBoss = true;
	runningAway = true;//the boss will always run away from you
	eyeHeight = 75;
	defaultDegrees = 65;
	visionDegrees = defaultDegrees;//width of angle of vision
	defaultRadius = 180;
	visionRadius = defaultRadius;//how far vision reaches
	baseSpeed = 40;
	maxSpeed = baseSpeed;
	deadBodyName = "enemy/boss/dead.png";
	deadBodyOutlineName = "enemy/boss/dead_outline.png";
	//initializing animations:
	stand = GameAnimation(STAND, "enemy/boss/stand/%03d.png", 19, 10 FRAMES, true);
	walking = GameAnimation(WALK, "enemy/boss/walk/%03d.png", 12, 8 FRAMES, true);
	knockout = GameAnimation(KNOCKOUT, "enemy/boss/knockdown/%03d.png", 4, 20 FRAMES, false);
	knockoutDeath = GameAnimation(DEATH, "enemy/boss/knockdown_die/%03d.png", 1, 20 FRAMES, false);
	dying = GameAnimation(DEATH, "enemy/boss/die/%03d.png", 4, 20 FRAMES, false);
	stab = GameAnimation(STAB, "enemy/boss/stab/%03d.png", 2, 10 FRAMES, false);
	swing = GameAnimation(SWING, "enemy/boss/swing/%03d.png", 2, 8 FRAMES, false);
	throwing = GameAnimation(THROW, "enemy/boss/throw/%03d.png", 2, 6 FRAMES, false);
}
