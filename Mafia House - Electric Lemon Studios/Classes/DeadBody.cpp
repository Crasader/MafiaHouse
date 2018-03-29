#include "DeadBody.h"

DeadBody::DeadBody()
{
	name = "dead_body";
	//sprite properties
	zOrder = 6;
	//physics body properties
	category = 32;
	collision = 42;
	tag = 70000;//each item type will be identified by the second and third digit: 10100 - 10199 for knives
	dynamic = true;
	rotate = false;
	outlineName = "enemy/thug/dead_outline.png";
	startTime = 0.4f;
	effect = NONE;
	dmg = 0;
	knockback = Vec2(200, 0);
	state = GROUND;
}

void DeadBody::initObject(Vec2 startPos)
{
	GameObject::initObject(startPos);
	getPhysicsBody()->setLinearDamping(1.0f);
	getPhysicsBody()->setRotationEnable(false);
	retain();
	initRadius();
	createOutline(outlineName);
}

//initializing pickup radius:
void DeadBody::initRadius() {
	Size pickUpBox = Size(getContentSize().width * 1.2, getContentSize().height / 2);
	pickUpRadius = Node::create();
	pickUpRadius->setPositionNormalized(Vec2(0.5, 0.5));
	pickUpRadius->setName("body_radius");
	auto pickUpRadiusBody = PhysicsBody::createBox(pickUpBox);
	pickUpRadiusBody->setDynamic(false);
	pickUpRadiusBody->setCategoryBitmask(4);
	pickUpRadiusBody->setCollisionBitmask(64);
	pickUpRadiusBody->setContactTestBitmask(0xFFFFFFFF);
	pickUpRadiusBody->setTag(10000);
	pickUpRadiusBody->setName("body_radius");
	pickUpRadius->setPhysicsBody(pickUpRadiusBody);
	addChild(pickUpRadius);
}

void DeadBody::playerInRange(Node* player) {
	if (isHidden == false && (player->getTag() >= 1 && player->getTag() <= 9)) {//body is not hidden and player is not hidden
		if (playerRange == true) {
			outline->setColor(ccc3(100, 255, 115));//green
		}
		else {
			outline->setColor(ccc3(255, 100, 100));//red
		}
	}
	else if (isHidden == true && player->getTag() >= 10){
		if (playerRange == true) {
			outline->setColor(ccc3(100, 255, 115));//green
		}
		else {
			outline->setColor(ccc3(100, 100, 100));//grey
		}
	}
	else {
		outline->setColor(ccc3(100, 100, 100));//grey
	}
	playerRange = false;
}

void DeadBody::initPickedUpBody() {
	state = HELD;
	setGlobalZOrder(5);
	outline->setGlobalZOrder(5);
	setPosition(Vec2(5, 38));
	setRotation(0);
	outline->setVisible(false);
	pickUpRadius->getPhysicsBody()->setEnabled(false);
	getPhysicsBody()->setDynamic(false);
	getPhysicsBody()->setCollisionBitmask(1);
	if (flippedX == true) {
		flipX();
	}
	knockback = Vec2(abs(knockback.x), 0);
}

void DeadBody::initHeldBody() {
	setRotation(0);
	setPosition(Vec2(5, 38));
}

void DeadBody::initCrouchPickedUpBody() {
	state = HELD;
	setGlobalZOrder(5);
	outline->setGlobalZOrder(5);
	setPosition(Vec2(5, 8));
	setRotation(0);
	outline->setVisible(false);
	pickUpRadius->getPhysicsBody()->setEnabled(false);
	getPhysicsBody()->setDynamic(false);
	getPhysicsBody()->setCollisionBitmask(1);
	if (flippedX == true) {
		flipX();
	}
	knockback = Vec2(abs(knockback.x), 0);
}

void DeadBody::initCrouchHeldBody() {
	setRotation(0);
	setPosition(Vec2(5, 8));
}

void  DeadBody::initDroppedBody(Vec2 pos, bool flip) {
	getPhysicsBody()->setDynamic(true);
	initGroundItem();
	if (isHidden == false) {
		setGlobalZOrder(6);
		outline->setGlobalZOrder(6);
	}
	setPosition(pos);
	if (flip == true) {
		flipX();
		setAnchorPoint(Vec2(0, 0));
		setRotation(-getRotation());
	}
}

void DeadBody::prepareThrow() {
	setRotation(0);
	setPosition(Vec2(24, 70));
}

void DeadBody::prepareCrouchThrow() {
	setRotation(0);
	setPosition(Vec2(24, 40));
}

void DeadBody::throwItem(float angle, Vec2 pos, bool flip) {
	initThrownItem();
	setGlobalZOrder(6);
	outline->setGlobalZOrder(6);
	//getPhysicsBody()->setLinearDamping(0.2f);
	Vec2 direction = angleToDirection(angle);
	setPosition(pos);
	setRotation(0);
	if (flip == true) {
		flipX();
		setAnchorPoint(Vec2(0, 0));
		setRotation(-getRotation());
	}
	moveNoLimit(direction * 160);
}

void DeadBody::initThrownItem() {
	state = THROWN;
	outline->setVisible(true);
	outline->setColor(ccc3(210, 0, 255));//purple
	pickUpRadius->getPhysicsBody()->setEnabled(false);
	getPhysicsBody()->setCategoryBitmask(8);
	getPhysicsBody()->setCollisionBitmask(42);
	setName("held_item");
	getPhysicsBody()->setName("held_item");
	getPhysicsBody()->setDynamic(true);
	getPhysicsBody()->setRotationEnable(false);
}

void DeadBody::initGroundItem() {
	state = GROUND;
	//knockback = Vec2(abs(knockback.x), 0);//resetting knockback to positive
	outline->setVisible(true);
	getPhysicsBody()->setCategoryBitmask(32);
	getPhysicsBody()->setCollisionBitmask(42);
	//getPhysicsBody()->setLinearDamping(0.0f);
	pickUpRadius->getPhysicsBody()->setEnabled(true);
	setName("dead_body");
	getPhysicsBody()->setName("dead_body");
}

void DeadBody::checkThrownSpeed() {
	float speed = getPhysicsBody()->getVelocity().getLength();
	if (speed <= 10){
		initGroundItem();
	}
}
void DeadBody::checkGroundSpeed() {
	float speed = getPhysicsBody()->getVelocity().getLength();
	if (speed >= 100) {
		initThrownItem();
	}
}