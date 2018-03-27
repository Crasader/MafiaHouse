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
}

void DeadBody::initObject(Vec2 startPos)
{
	GameObject::initObject(startPos);
	getPhysicsBody()->setMass(1000000);
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
	if (player->getTag() >= 1 && player->getTag() <= 9) {
		if (playerRange == true) {
			outline->setColor(ccc3(100, 255, 115));//green
		}
		else {
			outline->setColor(ccc3(255, 255, 255));//white
		}
	}
	playerRange = false;
}

void DeadBody::initPickedUpBody() {
	//setAnchorPoint(Vec2(0.5, 0.5));
	setPosition(Vec2(5, 38));
	outline->setVisible(false);
	pickUpRadius->getPhysicsBody()->setEnabled(false);
	//getPhysicsBody()->setGravityEnable(false);
	getPhysicsBody()->setDynamic(false);
	getPhysicsBody()->setCollisionBitmask(1);
	if (flippedX == true) {
		flipX();
	}
}

void  DeadBody::initDroppedBody(Vec2 pos, bool flip) {
	setPosition(pos);
	if (flip == true) {
		flipX();
	}
	outline->setVisible(true);
	getPhysicsBody()->setCollisionBitmask(42);
	//getPhysicsBody()->setGravityEnable(true);
	getPhysicsBody()->setDynamic(true);
	pickUpRadius->getPhysicsBody()->setEnabled(true);
}