#include "Stair.h"

//Stair Class
Stair::Stair() {
	name = "stair";
	tag = 40000;
	//sprite properties
	zOrder = 2;
	scale = 1.0f;
	//physics body properties
	dynamic = false;
	category = 16;
	collision = 3;

	outlineName = "objects/stair_outline.png";
}
Stair::~Stair() {
}

void Stair::initObject() {
	setContentSize(stairSize);//won't be needed once we have a proper sprite

	GameObject::initObject();
	//use type to set tag of the stairway
	if (type == 2) {
		setTag(tag + pairNum + 1000);//will have tag of it's partner stairway plus 1000
	}
	else if (type == 1){
		setTag(tag + pairNum);
	}

	createOutline(outlineName);
	outline->setColor(ccc3(255, 235, 50));//yellow
	outline->setVisible(false);
}

void Stair::playerInRange() {
	if (playerRange == true) {
		outline->setVisible(true);
		//outline->setColor(ccc3(255, 235, 50));//yellow
	}
	else {
		outline->setVisible(false);
		//outline->setColor(ccc3(155, 155, 155));//grey
	}
	playerRange = false;
}

void Stair::use(GameObject* user, Node* mainLayer) {
	user->stop();
	if (type == 1) {
		user->setPosition(mainLayer->getChildByTag(getTag() + 1000)->getPosition() + Vec2(getContentSize().width / 2, 0) - Vec2(user->getContentSize().width / 2, 0));
		user->currentFloor = static_cast<Stair*>(mainLayer->getChildByTag(getTag() + 1000))->startRoom.y;//setting user's current floor to the floor the stair is on
	}
	else if (type == 2) {
		user->setPosition(mainLayer->getChildByTag(getTag() - 1000)->getPosition() + Vec2(getContentSize().width / 2, 0) - Vec2(user->getContentSize().width / 2, 0));
		user->currentFloor = static_cast<Stair*>(mainLayer->getChildByTag(getTag() - 1000))->startRoom.y;//setting user's current floor to the floor the stair is on
	}
}

//Door Class
Door::Door() {
	name = "door";
	tag = 50000;
	//sprite properties
	zOrder = 6;
	scale = 1.0f;
	//physics body properties
	dynamic = false;
	category = 0xFFFFFFFF;
	collision = 0xFFFFFFFF;

	outlineName = "objects/door/outline.png";
}
Door::~Door() {
}

void Door::initObject(Vec2 startPos) {
	//setColor(ccc3(255, 155, 0));//orange
	setContentSize(size);//set the size of the wall
	GameObject::initObject(startPos);

	auto useRadius = Node::create();
	useRadius->setPositionNormalized(Vec2(0.5, 0.5));
	useRadius->setName("door_radius");

	auto radiusBody = PhysicsBody::createBox(useBox);
	radiusBody->setDynamic(false);
	radiusBody->setCategoryBitmask(4);
	radiusBody->setCollisionBitmask(3);
	radiusBody->setContactTestBitmask(0xFFFFFFFF);
	radiusBody->setTag(10000);
	radiusBody->setName("door_radius");
	useRadius->setPhysicsBody(radiusBody);
	addChild(useRadius);

	createOutline(outlineName);
}

void Door::initObject(int orient, Vec2 startPos) {
	if (orient == 2) {//horizontal
		size = Size(110, 20);
		useBox = Size(110, radius);
	}
	else if (orient == 1) {//vertical
		size = Size(20, 110);
		useBox = Size(radius, 110);
	}
	Door::initObject(startPos);
}

void Door::playerInRange() {
	if (playerRange == true) {
		outline->setColor(ccc3(255, 235, 50));//yellow
	}
	else {
		outline->setColor(ccc3(155, 155, 155));//grey
	}
	playerRange = false;
}

void Door::updateColour() {
	if (broken == false) {
		float percentage = hp / maxHP;
		float inversePercentage = abs(percentage - 1);//inverts the percentage

		if (locked == false) {
			setColor(ccc3(255 * percentage, 175 * percentage, 255 * inversePercentage));
		}
		else {
			setColor(ccc3(255 * percentage, 255 * inversePercentage, 255 * inversePercentage));
		}
	}
}

void Door::itemHit(Item* item) {
	if (broken == false) {
		if (item->isKey == true) {//item is a key
			if (locked == true) {
				unlock();
			}
			else {
				lock();//keys can lock doors
			}
			item->hp -= 2;//keys can only be used twice
			item->didHitWall = false;
		}
		else if (item->canBreakDoor == true || item->enemyItem == true) {//all enemy items will break down doors
			noiseLevel = item->doorDmg * 5;
			roomHitFrom = Vec2(item->currentRoom, item->currentFloor);
			hp -= item->doorDmg;//item deals dmg to the door
			item->hp--;
		}
		if (hp <= 0) {
			breakDoor();
		}
	}
}

void Door::updateBroken() {
	if (hp <= 0) {
		breakDoor();
	}
}

void Door::breakDoor() {
	if (broken == false) {
		//will probably change sprite
		unlock();
		broken = true;
		setColor(ccc3(155, 0, 255));//purple
	}
}

bool Door::use() {
	if (locked == false) {
		if (isOpen == false) {
			isOpen = true;
			getPhysicsBody()->setEnabled(false);
			setGlobalZOrder(2);
			setOpacity(100);
			outline->setGlobalZOrder(2);
			outline->setOpacity(100);
			return true;
		}
		else {
			isOpen = false;
			getPhysicsBody()->setEnabled(true);
			setGlobalZOrder(5);
			setOpacity(255);
			outline->setGlobalZOrder(5);
			outline->setOpacity(255);
			return true;
		}
	}
	return false;
}

void Door::unlock() {
	if (broken == false) {//can't unlock if it's broken
		if (locked == true) {
			locked = false;
			if (rightRoom != NULL) {
				rightRoom->leftLocked = false;
			}
			if (leftRoom != NULL) {
				leftRoom->rightLocked = false;
			}
			//setColor(ccc3(255, 155, 0));//orange
		}
	}
}

void Door::lock() {
	if (broken == false) {//can't lock if it's broken
		if (locked == false) {
			locked = true;
			if (rightRoom != NULL) {
				rightRoom->leftLocked = true;
			}
			if (leftRoom != NULL) {
				leftRoom->rightLocked = true;
			}
			//setColor(ccc3(255, 0, 0));//red
		}
	}
}

//Vent Class
Vent::Vent() {
	radius = 46.0f;
	name = "vent";
	tag = 60000;
	//sprite properties
	zOrder = 6;
	scale = 1.0f;
	//physics body properties
	dynamic = false;
	category = 0xFFFFFFFF;
	collision = 0xFFFFFFFF;
}
Vent::~Vent() {
}

void Vent::initObject(int orient, Vec2 startPos) {
	if (orient == 2) {//horizontal
		size = Size(50, 20);
		outlineName = "objects/vent/outline_h.png";
		useBox = Size(55 + radius / 2, radius + 10);
	}
	else if (orient == 1) {//vertical
		size = Size(20, 50);
		outlineName = "objects/vent/outline_v.png";
		useBox = Size(radius, 55 + radius / 2);
	}
	setContentSize(size);//set the size of the wall
	GameObject::initObject(startPos);

	auto useRadius = Node::create();
	useRadius->setPositionNormalized(Vec2(0.5, 0.5));
	useRadius->setName("vent_radius");

	auto radiusBody = PhysicsBody::createBox(useBox);
	radiusBody->setDynamic(false);
	radiusBody->setCategoryBitmask(4);
	radiusBody->setCollisionBitmask(3);
	radiusBody->setContactTestBitmask(0xFFFFFFFF);
	radiusBody->setTag(10000);
	radiusBody->setName("vent_radius");
	useRadius->setPhysicsBody(radiusBody);
	addChild(useRadius);

	createOutline(outlineName);
	//initializing physics body for enemies to walk on
	auto body = PhysicsBody::createBox(size);//player is half height when crouching
	body->setContactTestBitmask(0xFFFFFFFF);
	body->setCategoryBitmask(1);
	body->setCollisionBitmask(2);//only collide with enemies
	body->setDynamic(false);
	enemyWalkBody = Node::create();
	enemyWalkBody->setPositionNormalized(Vec2(0.5, 0.5));
	enemyWalkBody->setPhysicsBody(body);
	addChild(enemyWalkBody);
}

void Vent::itemHit(Item* item) {
	if (broken == false) {
		if (item->isKey == true) {//item is a key
			if (locked == true) {
				unlock();
				item->hp -= 2;//keys can only be used twice
			}//you can't lock a vent again, there's no point
			item->didHitWall = false;
		}
		else if (item->canBreakDoor == true || item->enemyItem == true) {//all enemy items will break down doors
			noiseLevel = item->doorDmg * 3;
			hp -= item->doorDmg;//item deals dmg to the door
			item->hp--;
		}
		if (hp <= 0) {
			breakDoor();
		}
	}
}

//Exit Class
Exit::Exit() {
	isExit = true;
	radius = 40.0f;
	name = "exit_door";
	tag = 70000;
	//sprite properties
	zOrder = 7;
	scale = 1.0f;
	//physics body properties
	dynamic = false;
	category = 0xFFFFFFFF;
	collision = 0xFFFFFFFF;
	outlineName = "objects/door/outline.png";
}
Exit::~Exit() {
}

void Exit::initObject(int orient, Vec2 startPos) {
	setContentSize(size);//set the size of the wall
	GameObject::initObject(startPos);

	auto useRadius = Node::create();
	useRadius->setPositionNormalized(Vec2(0.5, 0.5));
	useRadius->setName("exit_radius");

	auto radiusBody = PhysicsBody::createBox(useBox);
	radiusBody->setDynamic(false);
	radiusBody->setCategoryBitmask(4);
	radiusBody->setCollisionBitmask(1);
	radiusBody->setContactTestBitmask(0xFFFFFFFF);
	radiusBody->setName("exit_radius");
	useRadius->setPhysicsBody(radiusBody);
	addChild(useRadius);

	createOutline(outlineName);
}

void Exit::initExit() {
	auto body = PhysicsBody::createBox(Size(110, 40));
	body->setEnabled(false);
	body->setDynamic(false);
	body->setCategoryBitmask(4);
	body->setCollisionBitmask(1);
	body->setContactTestBitmask(0xFFFFFFFF);
	exitBox = Node::create();
	exitBox->setPhysicsBody(body);
	exitBox->setName("level_exit");
	exitBox->setAnchorPoint(Vec2(0, 0));
	addChild(exitBox);
	if (side == 1) {//right side
		exitBox->setPosition(Vec2(20, 0));
	}
	else {//left side
		setPosition(getPosition() - Vec2(10, 0));
		exitBox->setPosition(Vec2(-20, 0));
	}
}

void Exit::updateColour() {
	if (canOpen == true) {//you can exit the level
		setColor(ccc3(55, 255, 55));//green
	}
	else {//you can't exit the level yet
		setColor(ccc3(55, 55, 255));//blue
	}
}

bool Exit::use() {
	if (canOpen == true) {
		exitBox->getPhysicsBody()->setEnabled(true);
		isOpen = true;
		getPhysicsBody()->setEnabled(false);
		setGlobalZOrder(2);
		setOpacity(100);
		outline->setGlobalZOrder(2);
		outline->setOpacity(100);
		return true;
	}
	return false;
}