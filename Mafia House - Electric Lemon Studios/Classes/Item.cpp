#include "Item.h"

Item::Item()
{
	name = "item";
	//sprite properties
	zOrder = 6;
	//physics body properties
	category = 32;
	collision = 8;
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
	createOutline(outlineName);
}
//initializing pickup radius:
void Item::initRadius() {
	Size pickUpBox = getContentSize() * 1.7;
	pickUpRadius = Node::create();
	pickUpRadius->setPositionNormalized(Vec2(0.5, 0.5));
	pickUpRadius->setName("item_radius");

	auto pickUpRadiusBody = PhysicsBody::createBox(pickUpBox);
	pickUpRadiusBody->setDynamic(false);
	pickUpRadiusBody->setCategoryBitmask(4);
	pickUpRadiusBody->setCollisionBitmask(3);
	pickUpRadiusBody->setContactTestBitmask(0xFFFFFFFF);
	pickUpRadiusBody->setTag(10000);
	pickUpRadiusBody->setName("item_radius");
	pickUpRadius->setPhysicsBody(pickUpRadiusBody);

	addChild(pickUpRadius);
}

//used when player picks up item
void Item::initHeldItem() {
	state = HELD;
	outline->setVisible(false);
	pickUpRadius->getPhysicsBody()->setEnabled(false);
	getPhysicsBody()->setEnabled(false);
	getPhysicsBody()->setDynamic(true);
	getPhysicsBody()->setGravityEnable(false);
	getPhysicsBody()->setCategoryBitmask(8);
	getPhysicsBody()->setCollisionBitmask(42);
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
	initGroundItem();
	setPosition(pos);
	//setRotation(-20);
	if (flip == true) {
		flipX();
		setAnchorPoint(Vec2(0, 0));
		setRotation(-getRotation());
	}
}
//used when thrown item becomes ground item
void Item::initGroundItem() {
	state = GROUND;
	outline->setVisible(true);
	getPhysicsBody()->setCategoryBitmask(32);
	getPhysicsBody()->setCollisionBitmask(8);
	getPhysicsBody()->setEnabled(true);
	getPhysicsBody()->setGravityEnable(true);
	getPhysicsBody()->setDynamic(true);
	setName("item");
	getPhysicsBody()->setName("item");
	pickUpRadius->getPhysicsBody()->setEnabled(true);
}

void Item::breakItem() {
	release();
	getParent()->removeChild(this, true);
}

void Item::used() {
	hp = 0;
}

void Item::hitWall() {
	didHitWall = true;
	getPhysicsBody()->setEnabled(false);
}

void Item::playerInRange() {
	if (playerRange == true){
		outline->setColor(ccc3(100, 255, 100));//green
	}
	playerRange = false;
}

void Item::hasMoved() {
	if (abs((getPosition() - initialPos).getLengthSq()) > 50 * 50) {//if item is not within a 50 px radius of it's starting position
		enemyCanUse = true;
		outline->setColor(ccc3(255,100,100));//red
	}
	else {
		enemyCanUse = false;
		outline->setColor(ccc3(100,100,255));//blue
	}
}

void Item::checkSpeed() {
	float speedSq = getPhysicsBody()->getVelocity().getLengthSq();//squared speed
		if (speedSq < 50 * 50) {//check if speed has gone below threshold
			initGroundItem();
	}
}

void Item::beginStab() {
	setPosition(Vec2(25, 45));
	setRotation(0);
	//auto prepare = MoveBy::create(5 FRAMES, Vec2(-12, 6));
	//runAction(prepare);
}

void Item::beginSwing() {
	setRotation(-135);
	setPosition(Vec2(35, 85));
	//auto prepare = MoveBy::create(10 FRAMES, Vec2(-16, 20));
	//auto rotate = RotateBy::create(10 FRAMES, -90);
	//runAction(Spawn::create(prepare,rotate));
}

void Item::stabSequence() {
	auto move = MoveBy::create(attackTime * 0.125, Vec2(25, 6));
	auto hold = MoveBy::create(attackTime * 0.75, Vec2(0, 0));
	auto moveback = MoveBy::create(attackTime * 0.125, Vec2(-25, -6));
	auto sequence = Sequence::create(move, hold, moveback, NULL);
	runAction(sequence);
}

void Item::swingSequence() {
	auto move = MoveBy::create(7 FRAMES, Vec2(20, -35));
	auto rotate = RotateBy::create(7 FRAMES, 135);

	auto hold = MoveBy::create(6 FRAMES, Vec2(0, 0));

	//auto moveback = MoveBy::create(6 FRAMES, Vec2(-10, 5));
	//auto rotateback = RotateBy::create(6 FRAMES, -135);
	//Spawn::create(moveback, rotateback)

	auto sequence = Sequence::create(Spawn::create(move, rotate), hold, NULL);
	runAction(sequence);
}

//Knife Class:
Knife::Knife()
{
	outlineName = "items/knife_outline.png";
	Item::Item();
	hp = 2;
	dmg = 5;
	tag = 10100;//10100 - 10199 for knives
	effect = KILL;
	attackType = STAB;
	startTime = 6 FRAMES;
	attackTime = 8 FRAMES;
	lagTime = 10 FRAMES;
}
Knife::~Knife(){
}

//Key Class:
Key::Key()
{
	outlineName = "items/key_outline.png";
	Item::Item();
	isKey = true;
	hp = 1;
	dmg = 0;
	tag = 10200;//10200 - 10299 for keys
	effect = NONE;
	attackType = STAB;
	startTime = 4 FRAMES;
	attackTime = 6 FRAMES;
	lagTime = 6 FRAMES;
}
Key::~Key() {
}

//Hammer Class:
Hammer::Hammer()
{
	outlineName = "items/hammer_outline.png";
	Item::Item();
	hp = 2;
	dmg = 5;
	canBreakDoor = true;
	tag = 10100;//10100 - 10199 for knives
	effect = KILL;
	attackType = SWING;
	startTime = 14 FRAMES;
	attackTime = 20 FRAMES;
	lagTime = 18 FRAMES;
}
Hammer::~Hammer() {
}
