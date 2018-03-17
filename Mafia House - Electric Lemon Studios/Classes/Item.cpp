#include "Item.h"

Item::Item()
{
	name = "item";
	//sprite properties
	zOrder = 6;
	//physics body properties
	category = 16;
	collision = 3;
	tag = 10000;//eac_h item type will be identified by the second and third digit: 10100 - 10199 for knives
	dynamic = true;
	rotate = true;
	attackType = STAB;
	startTime = 10 FRAMES;
	attackTime = 10 FRAMES;
	lagTime = 20 FRAMES;
}
Item::~Item(){
}

void Item::initObject(Vec2 startPos)
{
	GameObject::initObject(startPos);
	retain();
	initRadius();
}
//initializing pickup radius:
void Item::initRadius() {
	Size pickUpBox = getContentSize() * 1.7;
	auto pickUpRadius = Node::create();
	pickUpRadius->setPositionNormalized(Vec2(0.5, 0.5));
	pickUpRadius->setName("item_radius");

	auto pickUpRadiusBody = PhysicsBody::createBox(pickUpBox);
	pickUpRadiusBody->setDynamic(false);
	pickUpRadiusBody->setCategoryBitmask(4);
	pickUpRadiusBody->setCollisionBitmask(1);
	pickUpRadiusBody->setContactTestBitmask(0xFFFFFFFF);
	pickUpRadiusBody->setTag(10000);
	pickUpRadiusBody->setName("item_radius");
	pickUpRadius->setPhysicsBody(pickUpRadiusBody);

	addChild(pickUpRadius);
}

//used when player picks up item
void Item::initHeldItem() {
	removeChildByName("item_radius", true);
	getPhysicsBody()->setEnabled(false);
	getPhysicsBody()->setDynamic(false);
	getPhysicsBody()->setCategoryBitmask(8);
	getPhysicsBody()->setCollisionBitmask(7);
	setName("held_item");
	getPhysicsBody()->setName("held_item");
	setPosition(Vec2(50, 32));
	setRotation(-45.0f);
	if (flippedX == true) {
		flipX();
	}
}
//used when player drops item
void Item::initDroppedItem(Vec2 pos, bool flip) {
	getPhysicsBody()->setCategoryBitmask(16);
	getPhysicsBody()->setCollisionBitmask(3);
	getPhysicsBody()->setEnabled(true);
	getPhysicsBody()->setDynamic(true);
	setName("item");
	getPhysicsBody()->setName("item");
	if (flip == true) {
		flipX();
		setAnchorPoint(Vec2(0, 0));
	}
	setPosition(pos);
	setRotation(0);
	initRadius();
}

void Item::breakItem() {
	release();
	getParent()->removeChild(this, true);
}

void Item::used() {
	isUsed = true;
}

void Item::beginStab() {
	setPosition(Vec2(25, 45));
	setRotation(0);
	//auto prepare = MoveBy::create(5 FRAMES, Vec2(-12, 6));
	//runAction(prepare);
}

void Item::beginSwing() {
	setRotation(-135);
	setPosition(Vec2(10, 45));
	//auto prepare = MoveBy::create(10 FRAMES, Vec2(-16, 20));
	//auto rotate = RotateBy::create(10 FRAMES, -90);
	//runAction(Spawn::create(prepare,rotate));
}

void Item::stabSequence() {
	auto move = MoveBy::create(1 FRAMES, Vec2(25, 6));
	auto hold = MoveBy::create(6 FRAMES, Vec2(0, 0));
	auto moveback = MoveBy::create(1 FRAMES, Vec2(-25, -6));
	auto sequence = Sequence::create(move, hold, moveback, NULL);
	runAction(sequence);
}

void Item::swingSequence() {
	auto move = MoveBy::create(4 FRAMES, Vec2(20, -25));
	auto rotate = RotateBy::create(4 FRAMES, 90);
	auto hold = MoveBy::create(6 FRAMES, Vec2(0, 0));
	auto moveback = MoveBy::create(6 FRAMES, Vec2(-10, 5));
	auto rotateback = RotateBy::create(6 FRAMES, -90);
	auto sequence = Sequence::create(Spawn::create(move, rotate), hold, Spawn::create(moveback, rotateback), NULL);
	runAction(sequence);
}

//Knife Class:
Knife::Knife()
{
	Item::Item();
	tag = 10100;//10100 - 10199 for knives
	attackType = STAB;
	startTime = 6 FRAMES;
	attackTime = 8 FRAMES;
	lagTime = 10 FRAMES;
}
Knife::~Knife(){
}
