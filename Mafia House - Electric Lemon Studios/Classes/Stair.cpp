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
	collision = 1;
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
}
Door::~Door() {
}

void Door::initObject(Vec2 startPos) {
	setColor(ccc3(255, 155, 0));//orange
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

void Door::itemHit(Item* item) {
	if (broken == false) {
		if (item->isKey == true) {//item is a key
			unlock();
			item->hp--;
		}
		else if (item->canBreakDoor == true) {
			hp -= item->dmg;//item deals dmg to the door
			item->hp--;
		}
		if (hp <= 0) {
			breakDoor();
		}
	}
}

void Door::breakDoor() {
	//will probably change sprite
	unlock();
	broken = true;
	setColor(ccc3(155, 0, 255));//purple
}

void Door::use() {
	if (locked == false) {
		if (isOpen == false) {
			isOpen = true;
			getPhysicsBody()->setEnabled(false);
			setGlobalZOrder(2);
			setOpacity(100);
		}
		else {
			isOpen = false;
			getPhysicsBody()->setEnabled(true);
			setGlobalZOrder(5);
			setOpacity(255);
		}
	}
}

void Door::unlock() {
	if (broken == false) {//can't unlock if it's broken
		if (locked == true) {
			locked = false;
			setColor(ccc3(255, 155, 0));//orange
		}
	}
}

void Door::lock() {
	if (broken == false) {//can't lock if it's broken
		if (locked == false) {
			locked = true;
			setColor(ccc3(255, 0, 0));//red
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
		useBox = Size(55 + radius / 2, radius);
	}
	else if (orient == 1) {//vertical
		size = Size(20, 50);
		useBox = Size(radius, 55 + radius / 2);
	}
	Door::initObject(startPos);
}
