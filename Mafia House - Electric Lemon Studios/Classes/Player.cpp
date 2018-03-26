#include "Player.h"
#include "Wall.h"

Player::Player()
{
	FRAME_OFFSET = 22;
	bodySize = Size(26, 90);
	standSize = bodySize;
	crouchSize = Size(standSize.width, 35);
	//sprite properties
	zOrder = 5;
	scale = 1.0f;
	name = "player";
	//physics body properties
	tag = 1;
	dynamic = true;
	category = 1;
	collision = 30;
	//max movement speed
	baseSpeed = 70;
	//initializing animations
	stand = GameAnimation(STAND, "player/stand/%03d.png", 11, 10 FRAMES, true);
	moonwalk = GameAnimation(MOONWALK, "player/walk_moon/%03d.png", 7, 8 FRAMES, true);
	walking = GameAnimation(WALK, "player/walk/%03d.png", 6, 10 FRAMES, true);
	stab = GameAnimation(STAB, "player/stab/%03d.png", 2, 10 FRAMES, false);
	swing = GameAnimation(SWING, "player/swing/%03d.png", 2, 10 FRAMES, false);
	crouchstab = GameAnimation(CROUCHSTAB, "player/crouch_stab/%03d.png", 2, 10 FRAMES, false);
	crouchswing = GameAnimation(CROUCHSWING, "player/crouch_swing/%03d.png", 2, 10 FRAMES, false);
	crouch = GameAnimation(CROUCH, "player/crouch/%03d.png", 5, 10 FRAMES, false);
	standup = GameAnimation(STANDUP, "player/stand_up/%03d.png", 5, 10 FRAMES, false);
	crouchwalk = GameAnimation(CROUCHWALK, "player/crouch_walk/%03d.png", 5, 10 FRAMES, true);
	climbing = GameAnimation(CLIMB, "player/climb/%03d.png", 5, 10 FRAMES, false);
	jumping = GameAnimation(JUMP, "player/jump/%03d.png", 2, 10 FRAMES, false);
	falling = GameAnimation(FALL, "player/fall/%03d.png", 1, 10 FRAMES, true);
	rolling = GameAnimation(FALL, "player/roll/%03d.png", 6, 6 FRAMES, false);
}
Player::~Player(){}

void Player::initObject(Vec2 startPos) {
	Character::initObject(startPos);
	//initializing crouching physics body
	crouchBody = PhysicsBody::createBox(crouchSize);//player is half height when crouching
	crouchBody->setContactTestBitmask(0xFFFFFFFF);
	crouchBody->setTag(1);
	crouchBody->setName("player");
	crouchBody->setCategoryBitmask(1);
	crouchBody->setCollisionBitmask(30);
	crouchBody->setDynamic(true);
	crouchBody->setRotationEnable(false);
	crouchBody->retain();
}

//functions for player actions:
void Player::resetCollisionChecks() {
	isHidingUnder = false;
	touchingFloor = false;
	touchingWall = false;
	objectToClimb = NULL;
	isHit = false;
	doorToUse = NULL;
	stairToUse = NULL;
	objectToHideBehind = NULL;
	itemToPickUp = NULL;
	inVision = false;
	//bodyToPickUp = NULL;
}

void Player::wasHit(Item* item, float time) {
	if (item->didHitWall == false) {
		stopAllActions();
		wasInHitStun = true;
		hitStunStart = time;
		hitStunTime = item->hitstun;
		//item->used();//enemy items don't break
		isHit = true;
		hp -= item->dmg;//taking damage from attack
		if (touchingWall == false) {
			stop();
			moveAbsoluteNoLimit(item->knockback);
		}
	}
}

void Player::walkPrepareAttack(Input input, float time) {
	if (input == MOVE_LEFT) {
		if (moveDirection == 0) {
			walkingSound = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/walk.wav", true);
			moveDirection = 1;
			if (isCrouched == false) {
				startAnimation(WALK, walking);
			}
			else {
				startAnimation(CROUCHWALK, crouchwalk);
			}
			setSpeed(moveSpeed);
		}
		walking.action->setSpeed(moveSpeed);
		moveAbsolute(Vec2(-7.0f * moveSpeed, 0));
	}
	if (input == MOVE_RIGHT) {
		if (moveDirection == 0) {
			walkingSound = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/walk.wav", true);
			moveDirection = 2;
			if (isCrouched == false) {
				startAnimation(WALK, walking);
			}
			else {
				startAnimation(CROUCHWALK, crouchwalk);
			}
			setSpeed(moveSpeed);
		}
		walking.action->setSpeed(moveSpeed);
		moveAbsolute(Vec2(7.0f * moveSpeed, 0));
	}
	if (input == STOP) {
		stopX();
		CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(walkingSound);
		moveDirection = 0;
		stopAnimation(WALK);
		stopAnimation(CROUCHWALK);
		if (isCrouched == false) {
			setSpriteFrame(stand.animation->getFrames().at(0)->getSpriteFrame());//run standing animation here
		}
		else {
			setSpriteFrame(crouchwalk.animation->getFrames().at(0)->getSpriteFrame());//run standing animation here
		}
	}
}

void Player::walk(Input input, float time) {
	if (input == MOVE_LEFT) {
		if (moveDirection == 0){
			walkingSound = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/walk.wav", true);
			moveDirection = 1;
			if (turned == false) {
				turned = true;
				flipX();
			}
			stopAnimation(MOONWALK);
			startAnimation(WALK, walking);
			setSpeed(moveSpeed);
		}
		if (moveDirection == 1) {
			if (getActionByTag(MOONWALK) == NULL) {
				startAnimation(WALK, walking);
			}
			walking.action->setSpeed(moveSpeed);
			moveAbsolute(Vec2(-9.0f * moveSpeed, 0));
			//run walking animation
		}
		else if (moveDirection == 2) {
			stopAnimation(WALK);
			startAnimation(MOONWALK, moonwalk);
			setSpeed(moveSpeed * 1.4f);
			moonwalk.action->setSpeed(moveSpeed);
			if (turned == false) {
				turned = true;
				flipX();
				moonwalking = true;
			}
		}
	}
	if (input == MOVE_RIGHT) {
		if (moveDirection == 0) {
			walkingSound = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/walk.wav", true);
			moveDirection = 2;
			if (turned == true) {
				turned = false;
				flipX();
			}
			stopAnimation(MOONWALK);
			startAnimation(WALK, walking);
			setSpeed(moveSpeed);
		}
		if (moveDirection == 2) {
			if (getActionByTag(MOONWALK) == NULL) {
				startAnimation(WALK, walking);
			}
			walking.action->setSpeed(moveSpeed);
			moveAbsolute(Vec2(9.0f * moveSpeed, 0));
			//run walking animation
		}
		else if (moveDirection == 1) {
			stopAnimation(WALK);
			startAnimation(MOONWALK, moonwalk);
			setSpeed(moveSpeed * 1.4f);
			moonwalk.action->setSpeed(moveSpeed);
			if (turned == true) {
				turned = false;
				flipX();
				moonwalking = true;
			}
		}
	}
	if (input == STOP) {
		if (time - prevStopTime >= stopDelay || prevStopTime == -1) {
			prevStopTime = time;
			if (moonwalking == false) {
				stopX();
			}
			moonwalking = false;
		}
		CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(walkingSound);
		moveDirection = 0;
		stopAnimation(WALK);
		stopAnimation(MOONWALK);
		setSpriteFrame(stand.animation->getFrames().at(0)->getSpriteFrame());//run standing animation here
	}
}

void Player::crouchWalk(Input input, float time) {
	if (input == MOVE_LEFT) {
		if (moveDirection == 0) {
			moveDirection = 1;
			if (turned == false) {
				turned = true;
				flipX();
			}
			startAnimation(CROUCHWALK, crouchwalk);
			setSpeed(moveSpeed);
		}
		if (moveDirection == 1) {
			startAnimation(CROUCHWALK, crouchwalk);
			walking.action->setSpeed(moveSpeed);
			moveAbsolute(Vec2(-9.0f * moveSpeed, 0));
		}
		else if (moveDirection == 2) {
			setSpeed(moveSpeed * 1.4f);
			if (turned == false) {
				turned = true;
				flipX();
				moonwalking = true;
			}
		}
	}
	if (input == MOVE_RIGHT) {
		if (moveDirection == 0) {
			moveDirection = 2;
			if (turned == true) {
				turned = false;
				flipX();
			}
			startAnimation(CROUCHWALK, crouchwalk);
			setSpeed(moveSpeed);
		}
		if (moveDirection == 2) {
			startAnimation(CROUCHWALK, crouchwalk);
			walking.action->setSpeed(moveSpeed);
			moveAbsolute(Vec2(9.0f * moveSpeed, 0));
			//run walking animation
		}
		else if (moveDirection == 1) {
			setSpeed(moveSpeed * 1.4f);
			moonwalk.action->setSpeed(moveSpeed);
			if (turned == true) {
				turned = false;
				flipX();
				moonwalking = true;
			}
		}
	}
	if (input == STOP) {
		if (time - prevStopTime >= stopDelay || prevStopTime == -1) {
			prevStopTime = time;
			if (moonwalking == false) {
				stopX();
			}
			moonwalking = false;
		}
		moveDirection = 0;
		stopAnimation(CROUCHWALK);
		setSpriteFrame(crouchwalk.animation->getFrames().at(0)->getSpriteFrame());//run standing animation here
	}
}

void Player::jump() {
	if (touchingFloor == true) {
		move(Vec2(0, 100));//apply force straight up
	}
}

void Player::pickUpItem(GameLayer* mainLayer) {
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/equip.wav");
	Character::pickUpItem(mainLayer);
	if (isCrouched == true) {
		if (heldItem != NULL) {
			heldItem->initCrouchHeldItem();
		}
	}
}

void Player::dropItem(GameLayer* mainLayer) {
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/unequip.wav");
	Character::dropItem(mainLayer);
}

void Player::beginUseItem(float angle) {
	if (heldItem != NULL) {
		if (isCrouched == false) {
			if (heldItem->getAttackType() == Item::STAB) {
				heldItem->prepareStab(angle);
				setSpriteFrame(stab.animation->getFrames().at(0)->getSpriteFrame());//setting player sprite to first frame of stab animation
			}
			else if (heldItem->getAttackType() == Item::SWING) {
				heldItem->prepareSwing(angle);
				setSpriteFrame(swing.animation->getFrames().at(0)->getSpriteFrame());//first frame of the swing animation
			}
		}
		else {
			if (heldItem->getAttackType() == Item::STAB) {
				heldItem->prepareCrouchStab(angle);
				setSpriteFrame(crouchstab.animation->getFrames().at(0)->getSpriteFrame());//setting player sprite to first frame of stab animation
			}
			else if (heldItem->getAttackType() == Item::SWING) {
				heldItem->prepareCrouchSwing(angle);
				setSpriteFrame(crouchswing.animation->getFrames().at(0)->getSpriteFrame());//first frame of the swing animation
			}
		}
	}
}

void Player::useItem(float angle) {
	if (heldItem != NULL) {
		if (isCrouched == false) {
			heldItem->getPhysicsBody()->setEnabled(true);
			if (heldItem->getAttackType() == Item::STAB) {
				heldItem->stabSequence(angle, flippedX);
				setSpriteFrame(stab.animation->getFrames().at(1)->getSpriteFrame());
			}
			else if (heldItem->getAttackType() == Item::SWING) {
				heldItem->swingSequence(angle, flippedX);
				setSpriteFrame(swing.animation->getFrames().at(1)->getSpriteFrame());//run animation here rather than setting frame if there's more than 2 frames for swinging
			}
		}
		else {
			heldItem->getPhysicsBody()->setEnabled(true);
			if (heldItem->getAttackType() == Item::STAB) {
				heldItem->crouchStabSequence(angle, flippedX);
				setSpriteFrame(crouchstab.animation->getFrames().at(1)->getSpriteFrame());
			}
			else if (heldItem->getAttackType() == Item::SWING) {
				heldItem->crouchSwingSequence(angle, flippedX);
				setSpriteFrame(crouchswing.animation->getFrames().at(1)->getSpriteFrame());//run animation here rather than setting frame if there's more than 2 frames for swinging
			}
		}
	}
}

void Player::finishUseItem() {
	if (isCrouched == false) {
		if (heldItem->getAttackType() == Item::STAB) {
			setSpriteFrame(stab.animation->getFrames().at(0)->getSpriteFrame());
		}
		else if (heldItem->getAttackType() == Item::SWING) {
			setSpriteFrame(swing.animation->getFrames().at(0)->getSpriteFrame());//first frame of the swing animation
		}
	}
	else {
		if (heldItem->getAttackType() == Item::STAB) {
			setSpriteFrame(crouchstab.animation->getFrames().at(0)->getSpriteFrame());//setting player sprite to first frame of stab animation
		}
		else if (heldItem->getAttackType() == Item::SWING) {
			setSpriteFrame(crouchswing.animation->getFrames().at(0)->getSpriteFrame());//first frame of the swing animation
		}
	}
	heldItem->getPhysicsBody()->setEnabled(false);
}



void Player::useDoor() {
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/openDoor.wav");
	Character::useDoor();
}

void Player::useStair(GameLayer* mainLayer) {
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/stairs.wav");
	Character::useStair(mainLayer);
}

void Player::hide() {
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/hide.wav");
	if (hidden == false) {
		wasSeen = false;
		if (inVision == true) {//if player was in enemy vision upon entering hiding
			wasSeen = true;
		}
		hidden = true;
		setTag(getTag() + 10);//for enemy vision rays
		setGlobalZOrder(getGlobalZOrder() - 3);
		if (heldItem != NULL) {
			heldItem->setGlobalZOrder(heldItem->getGlobalZOrder() - 3);
		}
		hideObject->hide();
	}
	else {
		hidden = false;
		wasSeen = false;
		setTag(getTag() - 10);//for enemy vision rays
		setGlobalZOrder(getGlobalZOrder() + 3);
		if (heldItem != NULL) {
			heldItem->setGlobalZOrder(heldItem->getGlobalZOrder() + 3);
		}
		hideObject->unhide();
	}
}

void Player::stayWithin(HideObject* object) {
	Vec2 range = Vec2(object->getContentSize().width / 2, object->getContentSize().height / 2);
	Vec2 displacement = (getPosition() + Vec2(getSize().width / 2, getSize().height / 2)) - (object->getPosition() + Vec2(object->getContentSize().width / 2, object->getContentSize().height / 2));
	if (displacement.x > range.x - getSize().width / 2) {
		if (hittingRight == true) {
			setSpeed(0.3);
			moveAbsolute(Vec2(-3, 0));
		}
		else {
			hittingRight = true;
			stop();
			moveAbsolute(Vec2(-3, 0));
		}
	}
	else {
		if (hittingLeft == false) {
			setSpeed(1.0);
		}
		hittingRight = false;
	}
	if (displacement.x < -(range.x - getSize().width / 2)) {
		if (hittingLeft == true) {
			setSpeed(0.3);
			moveAbsolute(Vec2(3, 0));
		}
		else {
			hittingLeft = true;
			stop();
			moveAbsolute(Vec2(3, 0));
		}
	}
	else {
		if (hittingRight == false) {
			setSpeed(1.0);
		}
		hittingLeft = false;
	}
}
//have player stay behind object they are hiding behind
void Player::hiding() {
	stayWithin(hideObject);
}

void Player::climb()
{

	/*auto moveAction = MoveBy::create(0.5, Vec2(0, climbObject->getPositionY() + climbObject->getContentSize().height - getPositionY() + 4));
	moveAction->setTag(222);
	/*auto callback = CallFunc::create([this]() {
		climbComplete = true;
	});
	auto sequence = Sequence::create(moveAction, callback, nullptr);
	runAction(moveAction);*/
	startAnimation(CLIMB,climbing);
}

//Update Checking:
void Player::update(GameLayer* mainLayer, float time) {
	updateFloor(mainLayer->floors);//checking if floor has changed
	updateRoom(mainLayer->floors[currentFloor].rooms);//checking if room has changed
	if (itemHitBy != NULL) {
		wasHit(itemHitBy, time);
		itemHitBy = NULL;
	}
	if (isHidingUnder == true) {
		if (inVision == true) {//if player was in enemy vision upon entering hiding
			wasSeen = true;
		}
		setOpacity(200);
		hidden = true;
	}
	else{//if they are no longer under object
		setOpacity(255);
		if (state->type != "hide") {//and not in a hide state
			hidden = false;
			wasSeen = false;
		}
	}
	newState = state->update(this, mainLayer, time);
	if (newState != NULL)
	{
		state->exit(this, mainLayer);

		if (prevState != NULL && newState != prevState) { delete prevState; }
		prevState = state;
		state = newState;
		newState = NULL;

		state->enter(this, mainLayer, time);
	}
}

//Input Handling:
void Player::handleInput(GameLayer* mainLayer, float time, Input input) {
	if (time - hitStunStart >= hitStunTime || hitStunStart == -1) {//only allow player input if hitstun is over, of if histun never began
		hitStunStart = -1;
		newState = state->handleInput(this, mainLayer, time, input);
		if (newState != NULL)
		{
			state->exit(this, mainLayer);

			if (prevState != NULL && newState != prevState) { delete prevState; }
			prevState = state;
			state = newState;
			newState = NULL;

			state->enter(this, mainLayer, time);
		}
		wasInHitStun = false;
	}
}

//Player States:
void Player::State::enter(Player* player, GameLayer* mainLayer, float time) {}
Player::State* Player::State::update(Player* player, GameLayer* mainLayer, float time) {return nullptr;}
Player::State* Player::State::handleInput(Player* player, GameLayer* mainLayer, float time, Input input) {return nullptr;}
void Player::State::exit(Player* player, GameLayer* mainLayer) {}

//Neutral State:
void Player::NeutralState::enter(Player* player, GameLayer* mainLayer, float time) {
	player->isCrouched = false;
	player->stop();
	player->stopAllActions();
	if (player->prevState->type == "crouch") {
		player->setPhysicsBody(player->mainBody);
		player->bodySize = player->standSize;
		player->getPhysicsBody()->setPositionOffset(Vec2(0, 17));
		//player->startAnimation(STANDUP, player->standup);
		if (player->heldItem != NULL) {
			player->heldItem->initHeldItem();
		}
	}
	player->setSpriteFrame(player->stand.animation->getFrames().at(0)->getSpriteFrame());
	player->moveSpeed = 1.0f;
	player->setSpeed(player->moveSpeed);
}
Player::State* Player::NeutralState::update(Player* player, GameLayer* mainLayer, float time) {
	//if (player->checkDead() == true) { return new DeathState; }
	if (player->touchingFloor == false && player->getPhysicsBody()->getVelocity().y < -50) {
		return new FallState;
	}
	return nullptr;
}
Player::State* Player::NeutralState::handleInput(Player* player, GameLayer* mainLayer, float time, Input input) {
	if (input == USE_DOOR) {
		player->useDoor();
	}
	if (input == USE_STAIR) {
		player->useStair(mainLayer);
	}
	if (input == DROP) {
		player->dropItem(mainLayer);
	}
	if (input == USE_ITEM) {
		return new AttackState;
	}
	if (input == PICKUP) {
		player->pickUpItem(mainLayer);
	}
	if (input == HIDE) {
		return new HideState;
	}
	if (input == MOVE_LEFT || input == MOVE_RIGHT || input == STOP) {
		player->walk(input, time);
	}
	if (input == MOVE_UP) {
		if (player->objectToClimb == NULL) {//only jump if not colliding with a physical object
			return new JumpState;
		}
		else {//player has an object to climb
			player->climbObject = player->objectToClimb;
			return new ClimbState;
		}
	}
	if (input == MOVE_DOWN) {
		if (player->touchingFloor == true) {
			return new CrouchState;
		}
	}
	if (input == ROLL) {
		return new RollState;
	}
	if (input == NO_CLIP) {
		return new NoClipState;
	}
	return nullptr;
}

//Crouching State:
void Player::CrouchState::enter(Player* player, GameLayer* mainLayer, float time) {
	player->isCrouched = true;
	player->stop();
	player->stopAllActions();
	if (player->prevState->type == "neutral") {
		player->setPhysicsBody(player->crouchBody);
		player->bodySize = player->crouchSize;
		player->getPhysicsBody()->setPositionOffset(Vec2(0, -28));
		//player->startAnimation(CROUCH, player->crouch);
		if (player->heldItem != NULL) {
			player->heldItem->initCrouchHeldItem();
		}
	}
	player->setSpriteFrame(player->crouchwalk.animation->getFrames().at(0)->getSpriteFrame());
	player->moveSpeed = 0.5f;
	player->setSpeed(player->moveSpeed);
}
Player::State* Player::CrouchState::update(Player* player, GameLayer* mainLayer, float time) {
	//if (player->checkDead() == true) { return new DeathState; }
	if (player->touchingFloor == false && player->getPhysicsBody()->getVelocity().y < -50) {
		return new FallState;
	}
	return nullptr;
}
Player::State* Player::CrouchState::handleInput(Player* player, GameLayer* mainLayer, float time, Input input) {
	if (input == USE_DOOR) {
		player->useDoor();
	}
	if (input == USE_STAIR) {
		player->useStair(mainLayer);
	}
	if (input == DROP) {
		player->dropItem(mainLayer);
	}
	if (input == USE_ITEM) {
		return new AttackState;
	}
	if (input == PICKUP) {
		player->pickUpItem(mainLayer);
	}
	if (input == HIDE) {
		return new HideState;
	}
	if (input == MOVE_LEFT || input == MOVE_RIGHT || input == STOP) {
		player->crouchWalk(input, time);
	}
	if (input == MOVE_UP) {
		return new NeutralState;
	}
	if (input == ROLL) {
		return new RollState;
	}
	return nullptr;
}

//Jump State:
void Player::JumpState::enter(Player* player, GameLayer* mainLayer, float time) {
	player->stopAllActions();
	player->jump();
	player->startAnimation(JUMP, player->jumping);
}
Player::State* Player::JumpState::update(Player* player, GameLayer* mainLayer, float time) {
	if (player->getPhysicsBody()->getVelocity().y <= 0) {//when player's vertical speed has stopped
		return new FallState;
	}
	return nullptr;
}
Player::State* Player::JumpState::handleInput(Player* player, GameLayer* mainLayer, float time, Input input) {
	if (input == DROP) {
		player->dropItem(mainLayer);
	}
	if (input == PICKUP) {
		player->pickUpItem(mainLayer);
	}
	if (input == MOVE_UP) {
		if (player->objectToClimb != NULL) {//player has an object to climb
			player->climbObject = player->objectToClimb;
			return new ClimbState;
		}
	}
	return nullptr;
}
void Player::JumpState::exit(Player* player, GameLayer* mainLayer) {
}

//Fall State:
void Player::FallState::enter(Player* player, GameLayer* mainLayer, float time) {
	player->stopAllActions();
	player->startAnimation(FALL, player->falling);
}
Player::State* Player::FallState::update(Player* player, GameLayer* mainLayer, float time) {
	if (player->touchingFloor == true){//if player has hit the ground
		//player->startAnimation(LAND, landing);
		player->setPhysicsBody(player->crouchBody);
		player->bodySize = player->crouchSize;
		player->getPhysicsBody()->setPositionOffset(Vec2(0, -28));
		return new CrouchState;
	}
	//if (player->touchingFloor == true && player->getActionByTag(LAND) == NULL) {//player's landing animation has finished
	//	return new CrouchState;
	//}
	return nullptr;
}
Player::State* Player::FallState::handleInput(Player* player, GameLayer* mainLayer, float time, Input input) {
	if (input == DROP) {
		player->dropItem(mainLayer);
	}
	if (input == USE_ITEM) {
		return new AttackState;
	}
	if (input == PICKUP) {
		player->pickUpItem(mainLayer);
	}
	if (input == MOVE_UP) {
		if (player->objectToClimb != NULL) {//player has an object to climb
			player->climbObject = player->objectToClimb;
			return new ClimbState;
		}
	}
	return nullptr;
}
void Player::FallState::exit(Player* player, GameLayer* mainLayer) {
}

//Climb State:
void Player::ClimbState::enter(Player* player, GameLayer* mainLayer, float time) {
	player->maxSpeedY = 60;
	//player->stop();
	player->stopAllActions();
	//setting physics body to crouching body
	player->setPhysicsBody(player->crouchBody);
	player->bodySize = player->crouchSize;
	player->getPhysicsBody()->setPositionOffset(Vec2(0, -28));

	player->startAnimation(CLIMB, player->climbing);
}
Player::State* Player::ClimbState::update(Player* player, GameLayer* mainLayer, float time) {
	if (player->getPositionY() < player->climbObject->getPositionY() + player->climbObject->getContentSize().height + 4) {
		player->move(Vec2(0, 30));
	}
	else {
		return new CrouchState;
	}

	return nullptr;
}
void Player::ClimbState::exit(Player* player, GameLayer* mainLayer) {
	player->objectToClimb = NULL;
}

//Attack State(using items):
void Player::AttackState::enter(Player* player, GameLayer* mainLayer, float time) {
	player->stopAllActions();
	if (player->isCrouched == true) {
		player->moveSpeed = (0.2f);
		player->setSpeed(player->moveSpeed);
	}
	else {
		player->moveSpeed = (0.4f);
		player->setSpeed(player->moveSpeed);
	}
	player->beginUseItem(0);
	player->attackPrepareTime = time;
}
Player::State* Player::AttackState::update(Player* player, GameLayer* mainLayer, float time) {
	//if (player->checkDead() == true) { return new DeathState; }
	if (player->heldItem->didHitWall == true) {
		player->move(Vec2(-50, 0));
		//player->moveAbsolute(-player->heldItem->knockback / 5);
	}
	if (player->attackRelease == false) {
		player->beginUseItem(player->aimAngle);
	}
	if (player->attackRelease == true && player->attackPrepareTime != -1.0f && time - player->attackPrepareTime >= player->heldItem->getStartTime()) {
		player->attackStartTime = time;

		player->useItem(player->aimAngle);
		player->attackPrepareTime = -1.0f;
	}
	if (player->attackStartTime != -1.0f && time - player->attackStartTime >= player->heldItem->getAttackTime()) {
		player->finishUseItem();
		player->attackEndTime = time;
		player->attackStartTime = -1.0f;
	}
	if (player->attackEndTime != -1.0f && time - player->attackEndTime >= player->heldItem->getLagTime()) {
		player->attackEndTime = -1.0f;
		player->attackRelease = false;
		return player->prevState;
	}
	return nullptr;
}
Player::State* Player::AttackState::handleInput(Player* player, GameLayer* mainLayer, float time, Input input) {
	if (player->wasInHitStun == true) {//player is in hitstun
		player->attackRelease = true;//forced to release attack
		player->walk(STOP, time);
	}
	//only register inputs if player is still preparing attack
	if (player->attackRelease == false) {
		//input for aiming
		if (input == AIM_UP) {
			player->aimAngle = 270;
		}
		if (input == AIM_DOWN) {
			player->aimAngle = 90;
		}
		if (player->flippedX == true) {//only register left inputs for aiming while facing left
			if (input == AIM_UP_LEFT) {
				player->aimAngle = 315;
			}
			if (input == AIM_LEFT) {
				player->aimAngle = 0;
			}
			if (input == AIM_DOWN_LEFT) {
				player->aimAngle = 45;
			}
		}
		if (player->flippedX == false) {//only register right inputs for aiming while facing right
			if (input == AIM_UP_RIGHT) {
				player->aimAngle = 315;
			}
			if (input == AIM_RIGHT) {
				player->aimAngle = 0;
			}
			if (input == AIM_DOWN_RIGHT) {
				player->aimAngle = 45;
			}
		}
		//input for moving
		if ((input == MOVE_LEFT || input == MOVE_RIGHT || input == STOP)) {
			player->walkPrepareAttack(input, time);
		}
		//input for releasing attack
		if (input == USE_RELEASE) {
			player->attackRelease = true;
			player->walkPrepareAttack(STOP, time);
		}
	}
	return nullptr;
}
void Player::AttackState::exit(Player* player, GameLayer* mainLayer) {
	player->aimAngle = 0;
	player->heldItem->didHitWall = false;
	player->moveSpeed = (1.0f);
	player->setSpeed(player->moveSpeed);
	if (player->heldItem->hp <= 0) {//if item is broken, no hp left
		player->breakItem(mainLayer);
	}
	else{
		if (player->isCrouched == false) {
			player->heldItem->initHeldItem();
		}
		else {
			player->heldItem->initCrouchHeldItem();
		}
	}
}

//Rolling State:
void Player::RollState::enter(Player* player, GameLayer* mainLayer, float time) {
	if (player->prevState->type == "neutral") {
		player->setPhysicsBody(player->crouchBody);
		player->bodySize = player->crouchSize;
		player->getPhysicsBody()->setPositionOffset(Vec2(0, -28));
		if (player->heldItem != NULL) {
			player->heldItem->initCrouchHeldItem();
		}
	}
	player->heldItem->setVisible(false);
	player->stopAllActions();
	player->stop();
	player->startAnimation(ROLLING, player->rolling);
	player->moveNoLimit(Vec2(400, 0));//applying force for the roll
	player->getPhysicsBody()->setLinearDamping(1.0f);
}
Player::State* Player::RollState::update(Player* player, GameLayer* mainLayer, float time) {
	if (player->getPhysicsBody()->getVelocity().x == 0) {//when player's horizontal speed has stopped
		return new CrouchState;
	}
	return nullptr;
}
Player::State* Player::RollState::handleInput(Player* player, GameLayer* mainLayer, float time, Input input) {
	if (input == DROP) {
		player->dropItem(mainLayer);
	}
	if (input == PICKUP) {
		player->pickUpItem(mainLayer);
	}
	return nullptr;
}
void Player::RollState::exit(Player* player, GameLayer* mainLayer) {
	player->heldItem->setVisible(true);
	player->getPhysicsBody()->setLinearDamping(0.0f);
}


//Hide State:
void Player::HideState::enter(Player* player, GameLayer* mainLayer, float time) {
	player->hideObject = player->objectToHideBehind;
	player->hide();
}
Player::State* Player::HideState::update(Player* player, GameLayer* mainLayer, float time) {
	//if (player->checkDead() == true) { return new DeathState; }
	if (player->isHit == true) {//force exit hiding if hit by enemy
		if (player->isCrouched == true) {
			return new CrouchState;
		}
		else {
			return new NeutralState;
		}
	}
	player->hiding();
	return nullptr;
}
Player::State* Player::HideState::handleInput(Player* player, GameLayer* mainLayer, float time, Input input) {
	if (input == HIDE) {
		if (player->isCrouched == true) {
			return new CrouchState;
		}
		else {
			return new NeutralState;
		}
	}
	if (input == MOVE_LEFT) {
		if (player->hittingLeft == false) {
			if (player->isCrouched == false) {
				player->walk(input, time);
			}
			else {
				player->crouchWalk(input, time);
			}
		}
	}
	else if (input == MOVE_RIGHT) {
		if (player->hittingRight == false) {
			if (player->isCrouched == false) {
				player->walk(input, time);
			}
			else {
				player->crouchWalk(input, time);
			}
		}
	}
	else if (input == STOP) {
		if (player->isCrouched == false) {
			player->walk(input, time);
		}
		else {
			player->crouchWalk(input, time);
		}
	}
	if (player->hittingLeft == false && player->hittingRight == false) {
		if (input == MOVE_UP) {
			if (player->isCrouched == true) {
				player->stopAllActions();
				player->isCrouched = false;
				player->setPhysicsBody(player->mainBody);
				player->bodySize = player->standSize;
				player->getPhysicsBody()->setPositionOffset(Vec2(0, 17));
				//player->startAnimation(STANDUP, player->standup);
				player->setSpriteFrame(player->stand.animation->getFrames().at(0)->getSpriteFrame());
				player->moveSpeed = 1.0f;
				player->setSpeed(player->moveSpeed);
			}
		}
		else if (input == MOVE_DOWN) {
			if (player->isCrouched == false) {
				player->stopAllActions();
				player->isCrouched = true;
				player->setPhysicsBody(player->crouchBody);
				player->bodySize = player->crouchSize;
				player->getPhysicsBody()->setPositionOffset(Vec2(0, -25));
				//player->startAnimation(CROUCH, player->crouch);
				player->setSpriteFrame(player->crouchwalk.animation->getFrames().at(0)->getSpriteFrame());
				player->moveSpeed = 0.5f;
				player->setSpeed(player->moveSpeed);
			}
		}
	}
	return nullptr;
}
void Player::HideState::exit(Player* player, GameLayer* mainLayer) {
	player->moveSpeed = 1.0f;
	player->setSpeed(player->moveSpeed);
	player->hide();
}

//Death State:
void Player::DeathState::enter(Player* player, GameLayer* mainLayer, float time) {
}
Player::State* Player::DeathState::update(Player* player, GameLayer* mainLayer, float time) {
	return nullptr;
}
void Player::DeathState::exit(Player* player, GameLayer* mainLayer, float time) {
}


//No Clip state:
void Player::NoClipState::enter(Player* player, GameLayer* mainLayer, float time) {
	player->noclip = true;
	player->getPhysicsBody()->setVelocityLimit(250);
	player->getPhysicsBody()->setGravityEnable(false);
	player->getPhysicsBody()->setCollisionBitmask(0);
	player->moveSpeed = 4;
	player->setSpeed(player->moveSpeed);
	//player->hidden = true;
}
Player::State* Player::NoClipState::handleInput(Player* player, GameLayer* mainLayer, float time, Input input) {
	if (input == USE_DOOR) {
		player->useDoor();
	}
	if (input == USE_STAIR) {
		player->useStair(mainLayer);
	}
	if (input == USE_ITEM) {
		return new AttackState;
	}
	if (input == HIDE) {
		return new HideState;
	}
	if (input == MOVE_LEFT || input == MOVE_RIGHT || input == STOP) {
		player->walk(input, time);
	}
	if (input == MOVE_UP) {
		player->move(Vec2(0, 30));
	}
	else if (input == MOVE_DOWN) {
		player->move(Vec2(0, -30));
	}
	if (input == NO_CLIP) {
		return new NeutralState;
	}
	return nullptr;
}
void Player::NoClipState::exit(Player* player, GameLayer* mainLayer) {
	player->noclip = false;
	player->getPhysicsBody()->setVelocityLimit(10000);
	player->getPhysicsBody()->setGravityEnable(true);
	player->getPhysicsBody()->setCollisionBitmask(30);
	player->moveSpeed = 1;
	player->setSpeed(player->moveSpeed);
	//player->hidden = false;
}
