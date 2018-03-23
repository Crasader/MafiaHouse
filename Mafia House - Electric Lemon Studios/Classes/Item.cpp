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
	range = 50;
}

void Item::initObject(Vec2 startPos)
{
	GameObject::initObject(startPos);
	retain();
	initRadius();
	createOutline(outlineName);
	initGroundItem();
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
	setPosition(Vec2(54, 33));
	setRotation(-45.0f);
	if (flippedX == true) {
		flipX();
	}
	setAnchorPoint(Vec2(0, 0));
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
	knockback = Vec2(abs(knockback.x), 0);//resetting knockback to positive
	enemyItem = false;
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
	hp--;
}

void Item::hitWall() {
	didHitWall = true;
	getPhysicsBody()->setEnabled(false);
}

void Item::playerInRange(Node* player) {
	if (player->getTag() >= 1 && player->getTag() <= 9) {
		if (playerRange == true) {
			outline->setColor(ccc3(100, 255, 100));//green
		}
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

void Item::prepareStab(float angle) {
	setPosition(Vec2(28, 46));
	setRotation(0 + angle);
	//auto prepare = MoveBy::create(5 FRAMES, Vec2(-12, 6));
	//runAction(prepare);
}

void Item::beginSwing(float angle) {
	setRotation(-135 + angle);
	setPosition(Vec2(40, 80));
	setAnchorPoint(Vec2(-0.9, 0.5));
	//auto prepare = MoveBy::create(10 FRAMES, Vec2(-16, 20));
	//auto rotate = RotateBy::create(10 FRAMES, -90);
	//runAction(Spawn::create(prepare,rotate));
}

void Item::stabSequence(Vec2 direction) {
	auto move = MoveBy::create(attackTime * 0.125, direction * 25);//stab forward
	auto hold = MoveBy::create(attackTime * 0.75, Vec2(0, 0));//wait
	auto moveback = MoveBy::create(attackTime * 0.125, -direction * 25);//pull back
	auto sequence = Sequence::create(move, hold, moveback, NULL);
	runAction(sequence);
}

void Item::swingSequence(Vec2 direction) {
	auto move = MoveBy::create(6 FRAMES, Vec2(10, -26));
	auto rotate = RotateBy::create(6 FRAMES, 120);

	auto hold = MoveBy::create(8 FRAMES, Vec2(0, 0));

	//auto moveback = MoveBy::create(6 FRAMES, Vec2(-10, 5));
	//auto rotateback = RotateBy::create(6 FRAMES, -135);
	//Spawn::create(moveback, rotateback)

	auto sequence = Sequence::create(Spawn::create(move, rotate), hold, NULL);
	runAction(sequence);
}

//Fist Class:
Fist::Fist(){
	Item::Item();
	priority = -1;
	hp = 1;
	dmg = 25;
	knockback = Vec2(20, 0);
	hitstun = 10 FRAMES;
	doorDmg = 7;
	canBreakDoor = true;
	effect = KILL;
	attackType = STAB;
	startTime = 5 FRAMES;
	attackTime = 12 FRAMES;
	lagTime = 14 FRAMES;
	range = 26;
	powerLevel = 0;
}
void Fist::initObject(Vec2 startPos){
	GameObject::initObject(startPos);
	retain();
	initHeldItem();
	setVisible(false);
}
void Fist::initHeldItem() {
	state = HELD;
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
	setAnchorPoint(Vec2(0, 0));
}

//Knife Class:
Knife::Knife(){
	outlineName = "items/knife_outline.png";
	Item::Item();
	priority = 1;
	hp = 2;
	dmg = 50;
	hitstun = 20 FRAMES;
	doorDmg = 7;
	//tag = 10100;//10100 - 10199 for knives
	effect = KILL;
	attackType = STAB;
	startTime = 10 FRAMES;
	attackTime = 8 FRAMES;
	lagTime = 10 FRAMES;
	range = 36;
	powerLevel = 5;
}

//Key Class:
Key::Key(){
	outlineName = "items/key_outline.png";
	Item::Item();
	isKey = true;
	priority = 0;
	hp = 40;
	dmg = 25;
	hitstun = 4 FRAMES;
	doorDmg = 6;
	effect = NONE;
	attackType = STAB;
	startTime = 3 FRAMES;
	attackTime = 6 FRAMES;
	lagTime = 4 FRAMES;
	range = 28;
	powerLevel = 0;
}

//Hammer Class:
Hammer::Hammer(){
	outlineName = "items/hammer_outline.png";
	Item::Item();
	priority = 3;
	hp = 3;
	dmg = 34;
	knockback = Vec2(80, 0);
	hitstun = 26 FRAMES;
	doorDmg = 34;
	canBreakDoor = true;
	effect = NONE;
	attackType = SWING;
	startTime = 16 FRAMES;
	attackTime = 20 FRAMES;
	lagTime = 18 FRAMES;
	range = 50;
	rangeY = 80;
	rangeRadius = 80;
	powerLevel = 10;
}
