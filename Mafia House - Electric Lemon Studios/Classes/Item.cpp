#include "Item.h"
#include "Player.h"
#include "Enemy.h"

Item::Item()
{
	name = "item";
	//sprite properties
	zOrder = 6;
	//physics body properties
	category = 32;
	collision = 40;
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
	pickUpRadiusBody->setCollisionBitmask(66);
	pickUpRadiusBody->setContactTestBitmask(0xFFFFFFFF);
	pickUpRadiusBody->setTag(10000);
	pickUpRadiusBody->setName("item_radius");
	pickUpRadius->setPhysicsBody(pickUpRadiusBody);

	addChild(pickUpRadius);
}

void Item::initMissingItem() {
	missingItem = MissingItem::createWithSpriteFrameName(itemFile);
	auto body = PhysicsBody::createBox(missingItem->getContentSize());
	body->setDynamic(false);
	body->setCollisionBitmask(0);
	body->setContactTestBitmask(0xFFFFFFFF);
	body->setEnabled(false);
	missingItem->setPhysicsBody(body);
	missingItem->setName("missing_item");
	missingItem->setOpacity(155);
	missingItem->setVisible(false);
	missingItem->setPosition(getPosition());
	missingItem->owner = this;
}

//used when player picks up item
void Item::initPickedUpItem() {
	prevState = state;
	state = HELD;
	initHeldItem();
	outline->setVisible(false);
	pickUpRadius->getPhysicsBody()->setEnabled(false);
	pickUpRadius->getPhysicsBody()->setRotationOffset(0);
	getPhysicsBody()->setDynamic(false);
	getPhysicsBody()->setGravityEnable(false);
	getPhysicsBody()->setCategoryBitmask(8);
	getPhysicsBody()->setCollisionBitmask(46);
	setName("held_item");
	getPhysicsBody()->setName("held_item");
	if (flippedX == true) {
		flipX();
	}
	knockback = Vec2(abs(knockback.x), 0);//resetting knockback to positive
}

void Item::initHeldItem() {
	didHitWall = false;
	setAnchorPoint(Vec2(0, 0));
	setPosition(Vec2(54, 33));
	setRotation(-45.0f);
	getPhysicsBody()->setRotationOffset(0);
	getPhysicsBody()->setEnabled(false);
	outline->setVisible(false);
}

void Item::initCrouchHeldItem() {
	didHitWall = false;
	setAnchorPoint(Vec2(0, 0));
	setPosition(Vec2(45, 16));
	setRotation(20.0f);
	getPhysicsBody()->setRotationOffset(0);
	getPhysicsBody()->setEnabled(false);
}

void Item::initOffhand() {
	didHitWall = false;
	setAnchorPoint(Vec2(0, 0));
	setPosition(Vec2(27, 50));
	setRotation(135.0f);
	getPhysicsBody()->setRotationOffset(0);
	getPhysicsBody()->setEnabled(false);
	outline->setColor(ccc3(255, 100, 100));//red
	outline->setVisible(true);
	pickUpRadius->getPhysicsBody()->setEnabled(true);
	pickUpRadius->setPositionNormalized(Vec2(0.25, 0.75));
}

void Item::rotatePickUpRadius(float degrees) {
	pickUpRadius->getPhysicsBody()->setRotationOffset(degrees);
}

//used when player drops item
void Item::initDroppedItem(Vec2 pos, bool flip) {
	initGroundItem();
	setPosition(pos + Vec2(0,10));
	//setRotation(-20);
	if (flip == true) {
		flipX();
		setAnchorPoint(Vec2(0, 0));
		setRotation(-getRotation());
	}
}

Vec2 Item::angleToDirection(float angle) {
	Vec2 direction;
	if (angle == 270) { direction = Vec2(0, 1); }
	else if (angle == 315) { direction = Vec2(1, 1); }
	else if (angle == 0) { direction = Vec2(1, 0); }
	else if (angle == 45) { direction = Vec2(1, -1); }
	else if (angle == 90) { direction = Vec2(0, -1); }
	return direction;
}

void Item::prepareThrow(float angle) {
	setAnchorPoint(Vec2(0, 0.5));
	setPosition(Vec2(24, 70));
	setRotation(angle);
}

void Item::prepareCrouchThrow(float angle) {
	setAnchorPoint(Vec2(0, 0.5));
	setPosition(Vec2(26, 40));
	setRotation(angle);
}

void Item::spin() {
	if (flippedX == false) {
		setRotation(getRotation() + 25);
	}
	else {
		setRotation(getRotation() - 25);
	}
}

void Item::throwItem(float angle, Vec2 pos, bool flip) {
	initThrownItem();
	getPhysicsBody()->setLinearDamping(1.0f);
	setAnchorPoint(Vec2(0, 0));
	Vec2 direction = angleToDirection(angle);
	setPosition(pos);
	setRotation(angle);
	if (flip == true) {
		flipX();
		setRotation(-getRotation());
	}
	if (flip == true) {
		if (direction == Vec2(1, -1)) {
			moveNoLimit(Vec2(0, -700));
		}
		else if (direction == Vec2(1, 1)) {
			moveNoLimit(Vec2(0, 700));
		}
		else if (direction == Vec2(0, 1)) {
			moveNoLimit(Vec2(-700, 0));
		}
		else if (direction == Vec2(0, -1)) {
			moveNoLimit(Vec2(-700, 0));
		}
		else {
			moveNoLimit(Vec2(700, 0));
		}
	}
	else {
		moveNoLimit(Vec2(700, 0));
	}
}

void Item::initThrownItem() {
	prevState = state;
	state = THROWN;
	didHitWall = false;
	enemyItem = false;
	outline->setVisible(true);
	outline->setColor(ccc3(210, 0, 255));//purple
	pickUpRadius->getPhysicsBody()->setEnabled(false);
	getPhysicsBody()->setCategoryBitmask(8);
	getPhysicsBody()->setCollisionBitmask(46);
	setName("held_item");
	getPhysicsBody()->setName("held_item");
	getPhysicsBody()->setEnabled(true);
	getPhysicsBody()->setDynamic(true);
	getPhysicsBody()->setGravityEnable(false);
}

void Item::initFallItem() {
	prevState = state;
	state = FALLING;
	getPhysicsBody()->setEnabled(true);
	getPhysicsBody()->setGravityEnable(true);
	getPhysicsBody()->setDynamic(true);
	getPhysicsBody()->setLinearDamping(0.5f);
	pickUpRadius->getPhysicsBody()->setEnabled(true);
	outline->setVisible(false);
}

void Item::initGroundItem() {
	prevState = state;
	state = GROUND;
	didHitWall = false;
	enemyItem = false;
	outline->setVisible(true);
	getPhysicsBody()->setCategoryBitmask(32);
	getPhysicsBody()->setCollisionBitmask(40);
	getPhysicsBody()->setEnabled(true);
	getPhysicsBody()->setGravityEnable(true);
	getPhysicsBody()->setDynamic(true);
	getPhysicsBody()->setLinearDamping(1.0f);
	pickUpRadius->getPhysicsBody()->setEnabled(true);
	setName("item");
	getPhysicsBody()->setName("item");
	pickUpRadius->getPhysicsBody()->setRotationOffset(0);
}

bool Item::checkBroken() {
	if (hp <= 0) {
		breakItem();
		return true;
	}
	return false;
}

void Item::breakItem() {
	auto emitter = ParticleExplosion::create();
	emitter->setStartColor(Color4F(50, 50, 255, 1));
	emitter->setEndColor(Color4F(0, 0, 255, 1));//blue
	emitter->setStartSize(3.0f);
	emitter->setStartSizeVar(1.0f);
	emitter->setEndSize(0.5f);
	emitter->setEndSizeVar(0.5f);
	emitter->setSpeed(1600.0f);
	emitter->setSpeedVar(0.0f);
	emitter->setTotalParticles(150);
	emitter->setEmissionRate(1000000.0f);
	emitter->setEmitterMode(ParticleSystem::Mode::RADIUS);
	emitter->setStartRadius(0.0f);
	emitter->setEndRadius(50.0f);
	emitter->setEndRadiusVar(30.0f);
	emitter->setLife(0.1f);
	emitter->setLifeVar(1.5f);
	emitter->setTextureWithRect(frameCache->getSpriteFrameByName("particles/pixel.png")->getTexture(), frameCache->getSpriteFrameByName("particles/pixel.png")->getRect());
	emitter->setGlobalZOrder(30);
	if (state == HELD) {
		emitter->setPosition(convertToWorldSpace(getPosition() - getContentSize() / 2));
	}
	else {
		emitter->setPosition(getPosition());
	}
	director->getRunningScene()->addChild(emitter);
	if (missingItem != NULL) {
		missingItem->owner = NULL;
	}
	release();
	//autorelease();
	removeFromParent();
}

void Item::used() {
	hp--;
}

void Item::hitWall() {
	didHitWall = true;
	//getPhysicsBody()->setEnabled(false);
}

void Item::stealRange(Node* player) {
	if (player->getTag() >= 1 && player->getTag() <= 9) {
		if (playerRange == true) {
			outline->setColor(ccc3(100, 255, 100));//green
		}
		else {
			outline->setColor(ccc3(255, 100, 100));//red
		}
	}
	playerRange = false;
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
		if (missingItem != NULL) {
			missingItem->setVisible(true);
			missingItem->getPhysicsBody()->setEnabled(true);
		}
	}
	else {
		enemyCanUse = false;
		outline->setColor(ccc3(100,100,255));//blue
		if (missingItem != NULL) {
			missingItem->setVisible(false);
			missingItem->getPhysicsBody()->setEnabled(false);
		}
	}
}

void Item::checkThrownSpeed() {
	float speed = getPhysicsBody()->getVelocity().getLength();
	if (prevState == HELD) {
		if (attackType == STAB) {
			if (speed <= 500) {//speed is less than 450
				initFallItem();
			}
		}
		else if (attackType == SWING || attackType == SHOOT) {
			if (speed <= 492) {//speed is less than 450
				initFallItem();
			}
		}
	}
	else if (prevState == FALLING) {
		if (speed < 100) {//speed is less than 450
			initFallItem();
		}
	}
}

void Item::checkFallingSpeed() {
	float speed = getPhysicsBody()->getVelocity().getLength();
	if (speed <= 50) {//speed is less than 50
		initGroundItem();
	}
	if (prevState == GROUND) {
		if (speed >= 150) {//speed is greater than 150
			initThrownItem();
		}
	}
}

void Item::checkGroundSpeed() {
	float speed = getPhysicsBody()->getVelocity().getLength();
	 if (speed > 50) {//speed becomes greater than 50
		initFallItem();
	}
}

void Item::prepareStab(float angle) {
	setPosition(Vec2(30, 56));
	setRotation(0 + angle);
	setAnchorPoint(Vec2(0, 0.5));
	//auto prepare = MoveBy::create(5 FRAMES, Vec2(-12, 6));
	//runAction(prepare);
}

void Item::prepareSwing(float angle) {
	setRotation(-135 + angle);
	setPosition(Vec2(35, 70));
	setAnchorPoint(Vec2(-0.95, 0.5));
	//auto prepare = MoveBy::create(10 FRAMES, Vec2(-16, 20));
	//auto rotate = RotateBy::create(10 FRAMES, -90);
	//runAction(Spawn::create(prepare,rotate));
}

void Item::prepareCrouchStab(float angle) {
	setPosition(Vec2(30, 26));
	setRotation(0 + angle);
	setAnchorPoint(Vec2(0, 0.5));
}

void Item::prepareCrouchSwing(float angle) {
	setRotation(-135 + angle);
	setPosition(Vec2(35, 40));
	setAnchorPoint(Vec2(-0.95, 0.5));
}

void Item::stabSequence(float angle, bool flip) {
	Vec2 direction = angleToDirection(angle);
	if (direction == Vec2(1, -1) || direction == Vec2(1, 1)) {
		if (flip == true) { getPhysicsBody()->setRotationOffset(90); }
	}

	auto move = MoveBy::create(attackTime * 0.125, direction * 25);//stab forward
	auto hold = MoveBy::create(attackTime * 0.75, Vec2(0, 0));//wait
	auto moveback = MoveBy::create(attackTime * 0.125, -direction * 25);//pull back
	auto sequence = Sequence::create(move, hold, moveback, NULL);
	runAction(sequence);
}

void Item::swingSequence(float angle, bool flip) {
	Vec2 direction = angleToDirection(angle);
	if (direction == Vec2(1, -1) || direction == Vec2(1, 1)) {
		if (flip == true) { getPhysicsBody()->setRotationOffset(90); }
	}

	Vec2 movement = Vec2(6, -26);
	movement = movement.rotate(direction);

	auto move = MoveBy::create(attackTime * 0.3, movement);
	auto rotate = RotateBy::create(attackTime * 0.3, 120);

	auto hold = MoveBy::create(attackTime * 0.4, Vec2(0, 0));

	//auto moveback = MoveBy::create(6 FRAMES, Vec2(-10, 5));
	//auto rotateback = RotateBy::create(6 FRAMES, -135);
	//Spawn::create(moveback, rotateback)

	auto sequence = Sequence::create(Spawn::create(move, rotate), hold, NULL);
	runAction(sequence);
}

void Item::fallAttack() {
	getPhysicsBody()->setEnabled(true);
	if (attackType == STAB) {
		setPosition(Vec2(52, 35));
		setRotation(90);
		setAnchorPoint(Vec2(0, 0.5));
	}
	else if (attackType == SWING || attackType == SHOOT) {
		setPosition(Vec2(60, 35));
		setRotation(20);
		setAnchorPoint(Vec2(0, 0.5));
	}
}

//Gun Class:
Gun::Gun() {
	itemFile = "items/gun.png";
	outlineName = "items/gun_outline.png";
	Item::Item();
	priority = 20;
	maxHP = 3;
	hp = maxHP;
	dmg = 50;
	hitstun = 10 FRAMES;
	doorDmg = 100;
	canBreakDoor = true;
	//tag = 10100;//10100 - 10199 for knives
	effect = KNOCKOUT;
	attackType = SHOOT;
	startTime = 30 FRAMES;
	attackTime = 30 FRAMES;
	lagTime = 30 FRAMES;
	range = 300;
	rangeRadius = 300;
	powerLevel = 20;
	noiseLevel = 0.5f;
}

void Gun::initHeldItem() {
	Item::initHeldItem();
	effect = KNOCKOUT;
}

void Item::prepareShoot(float angle) {
	setRotation(angle);
	setPosition(Vec2(35, 70));
	setAnchorPoint(Vec2(-0.95, 0.5));
}

void Item::prepareCrouchShoot(float angle) {
	setRotation(angle);
	setPosition(Vec2(35, 35));
	setAnchorPoint(Vec2(-0.95, 0.5));
}

void Item::enemyShoot(Vec2 target) {
	effect = KILL;
	wasShot = true;
	endpoint = Vec2(0, 0);
	hitTarget = false;
	PhysicsRayCastCallbackFunc func = [this](PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data)->bool
	{
		string contactName = info.shape->getBody()->getNode()->getName();
		Node* contact = info.shape->getBody()->getNode();

		if (contactName == "wall" || contactName == "floor" || contactName == "ceiling" || contactName == "phys_object" || contactName == "exit_door") {//raycast stopped by physical objects
			endpoint = info.contact;
			return false;
		}
		else if (contactName == "door") {
			hitTarget = true;
			static_cast<Door*>(contact)->itemHit(this);
			endpoint = info.contact;
			return false;
		}
		else if (contactName == "door_radius") {//collides with a door radius
			if (static_cast<Door*>(contact->getParent())->checkOpen() == false) {//the doors is closed
				hitTarget = true;
				static_cast<Door*>(contact->getParent())->itemHit(this);
				if (holderFlipped == true) {
					endpoint = info.contact - Vec2(12, 0);
				}
				else {
					endpoint = info.contact + Vec2(12, 0);
				}
				return false;
			}
		}
		else if (contactName == "player") {//hit the player
			hitTarget = true;
			static_cast<Player*>(contact)->itemHitBy = this;//set item they were hit by to this gun
			endpoint = info.contact;
			return false;
		}
		return true;
	};
	if (holderFlipped == false) {
		startpoint = getParent()->convertToWorldSpace(getPosition()) + Vec2(60, 0);
	}
	else {
		startpoint = getParent()->convertToWorldSpace(getPosition()) - Vec2(60, 0);
	}
	director->getRunningScene()->getPhysicsWorld()->rayCast(func, startpoint, target, nullptr);//ray cast directly towards target
	if (hitTarget == false) {
		director->getRunningScene()->getPhysicsWorld()->rayCast(func, startpoint, target, nullptr);//ray cast again
	}
	if (hitTarget == false) {
		director->getRunningScene()->getPhysicsWorld()->rayCast(func, startpoint, target, nullptr);//triple ray cast
	}
	if (endpoint == Vec2(0, 0)) {//nothing was hit
		endpoint = target * 2;//for drawing bullet line
	}
}

void Item::playerShoot(float angle) {
	effect = KILL;
	used();
	wasShot = true;
	hitTarget = false;
	Vec2 direction = angleToDirection(angle);
	if (holderFlipped == true) {
		direction.x *= -1;
	}
	endpoint = Vec2(0, 0);
	PhysicsRayCastCallbackFunc func = [this](PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data)->bool
	{
		string contactName = info.shape->getBody()->getNode()->getName();
		Node* contact = info.shape->getBody()->getNode();

		if (contactName == "wall" || contactName == "floor" || contactName == "ceiling" || contactName == "phys_object" || contactName == "exit_door") {//raycast stopped by physical objects
 			endpoint = info.contact;
			return false;
		}
		else if (contactName == "door") {
			hitTarget = true;
			static_cast<Door*>(contact)->itemHit(this);
			endpoint = info.contact;
			return false;
		}
		else if (contactName == "door_radius") {//collides with a door radius
			if (static_cast<Door*>(contact->getParent())->checkOpen() == false) {//the doors is closed
				hitTarget = true;
				static_cast<Door*>(contact->getParent())->itemHit(this);
				if (holderFlipped == true) {
					endpoint = info.contact - Vec2(12, 0);
				}
				else {
					endpoint = info.contact + Vec2(12, 0);
				}
				return false;
			}
		}
		if (contactName == "enemy" || contactName == "enemy_alert") {//hit an enemy
			hitTarget = true;
			static_cast<Enemy*>(contact)->itemHitBy = this;//set item they were hit by to this gun
			endpoint = info.contact;
			return false;
		}
		return true;
	};
	startpoint = getParent()->convertToWorldSpace(getPosition()) + direction * 60;
	director->getRunningScene()->getPhysicsWorld()->rayCast(func, startpoint, startpoint + direction * 1000, nullptr);//ray cast in direction of aim
	if (hitTarget == false){
		director->getRunningScene()->getPhysicsWorld()->rayCast(func, startpoint, startpoint + direction * 1000, nullptr);//ray cast again
	}
	if (hitTarget == false) {
		director->getRunningScene()->getPhysicsWorld()->rayCast(func, startpoint, startpoint + direction * 1000, nullptr);//triple cast just to be sure
	}
	if (endpoint == Vec2(0, 0)) {//nothing was hit
		endpoint = startpoint + direction * 1000;//for drawing bullet line
	}
}

//Fist Class:
Fist::Fist(){
	Item::Item();
	priority = -1;
	hp = 1;
	dmg = 25;
	knockback = Vec2(20, 0);
	hitstun = 8 FRAMES;
	doorDmg = 7;
	canBreakDoor = true;
	effect = NONE;
	attackType = STAB;
	startTime = 6 FRAMES;
	attackTime = 12 FRAMES;
	lagTime = 14 FRAMES;
	range = 26;
	rangeRadius = 90;
	powerLevel = 0;
}
void Fist::initObject(Vec2 startPos){
	GameObject::initObject(startPos);
	retain();
	initHeldItem();
	setVisible(false);
}
void Fist::initHeldItem() {
	prevState = state;
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
	itemFile = "items/knife.png";
	outlineName = "items/knife_outline.png";
	Item::Item();
	priority = 1;
	maxHP = 2;
	hp = maxHP;
	dmg = 50;
	hitstun = 10 FRAMES;
	doorDmg = 7;
	//tag = 10100;//10100 - 10199 for knives
	effect = KILL;
	attackType = STAB;
	startTime = 11 FRAMES;
	attackTime = 8 FRAMES;
	lagTime = 13 FRAMES;
	range = 34;
	rangeRadius = 100;
	powerLevel = 5;
	noiseLevel = 0.45f;
}

//Key Class:
Key::Key(){
	itemFile = "items/key.png";
	outlineName = "items/key_outline.png";
	Item::Item();
	isKey = true;
	priority = 0;
	maxHP = 4;
	hp = maxHP;
	dmg = 25;
	hitstun = 4 FRAMES;
	doorDmg = 6;
	effect = NONE;
	attackType = STAB;
	startTime = 6 FRAMES;
	attackTime = 6 FRAMES;
	lagTime = 7 FRAMES;
	range = 26;
	rangeRadius = 90;
	powerLevel = 0;
	noiseLevel = 0.25f;
}

//Hammer Class:
Hammer::Hammer(){
	itemFile = "items/hammer.png";
	outlineName = "items/hammer_outline.png";
	Item::Item();
	priority = 3;
	maxHP = 3;
	hp = maxHP;
	dmg = 34;
	knockback = Vec2(80, 0);
	hitstun = 24 FRAMES;
	doorDmg = 34;
	canBreakDoor = true;
	effect = KNOCKOUT;
	attackType = SWING;
	startTime = 16 FRAMES;
	attackTime = 20 FRAMES;
	lagTime = 20 FRAMES;
	range = 45;
	rangeRadius = 130;
	powerLevel = 10;
	noiseLevel = 0.65f;
}
