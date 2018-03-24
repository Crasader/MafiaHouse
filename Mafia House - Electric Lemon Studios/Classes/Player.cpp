#include "Player.h"
#include "Wall.h"

Player::Player()
{
	FRAME_OFFSET = 22;
	bodySize = Size(26, 90);
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
	stand = GameAnimation(STAND, "player/stand/%03d.png", 11, 10 FRAMES);
	moonwalk = GameAnimation(MOONWALK, "player/walk_moon/%03d.png", 7, 8 FRAMES);
	walking = GameAnimation(WALK, "player/walk/%03d.png", 6, 10 FRAMES);
	stab = GameAnimation(STAB, "player/stab/%03d.png", 2, 10 FRAMES);
	swing = GameAnimation(SWING, "player/swing/%03d.png", 2, 10 FRAMES);
}
Player::~Player(){
}

//functions for player actions:
void Player::resetCollisionChecks() {
	touchingWall = false;
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
			startAnimation(WALK, walking);
			setSpeed(moveSpeed);
		}
		walking.action->setSpeed(moveSpeed);
		moveAbsolute(Vec2(-7.0f * moveSpeed, 0));
	}
	if (input == MOVE_RIGHT) {
		if (moveDirection == 0) {
			walkingSound = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/walk.wav", true);
			moveDirection = 2;
			startAnimation(WALK, walking);
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
		setSpriteFrame(stand.animation->getFrames().at(0)->getSpriteFrame());//run standing animation here
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
			walking.action->setSpeed(moveSpeed);
			moveAbsolute(Vec2(-9.0f * moveSpeed, 0));
			//run walking animation
		}
		else if (moveDirection == 2) {
			setSpeed(moveSpeed * 1.4f);
			moonwalk.action->setSpeed(moveSpeed);
			if (turned == false) {
				turned = true;
				flipX();
				stopAnimation(WALK);
				startAnimation(MOONWALK, moonwalk);
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
				stopAnimation(WALK);
				startAnimation(MOONWALK, moonwalk);
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

void Player::pickUpItem(GameLayer* mainLayer) {
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/equip.wav");
	Character::pickUpItem(mainLayer);
}

void Player::dropItem(GameLayer* mainLayer) {
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/unequip.wav");
	Character::dropItem(mainLayer);
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
		if (inVision == true) {//if player was in enemy vision upon entering hiding
			wasSeen = true;
		}
		hidden = true;
		setTag(getTag() + 10);
		getPhysicsBody()->setTag(2);//for enemy vision rays
		setGlobalZOrder(getGlobalZOrder() - 3);
		if (heldItem != NULL) {
			heldItem->setGlobalZOrder(heldItem->getGlobalZOrder() - 3);
		}
		hideObject->hide();
	}
	else {
		hidden = false;
		wasSeen = false;
		setTag(getTag() - 10);
		getPhysicsBody()->setTag(1);//for enemy vision rays
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

//Update Checking:
void Player::update(GameLayer* mainLayer, float time) {
	updateFloor(mainLayer->floors);//checking if floor has changed
	updateRoom(mainLayer->floors[currentFloor].rooms);//checking if room has changed
	if (itemHitBy != NULL) {
		wasHit(itemHitBy, time);
		itemHitBy = NULL;
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
void Player::State::enter(Player* player, GameLayer* mainLayer, float time) {
}
Player::State* Player::State::update(Player* player, GameLayer* mainLayer, float time) {
	return nullptr;
}
Player::State* Player::State::handleInput(Player* player, GameLayer* mainLayer, float time, Input input) {
	return nullptr;
}
void Player::State::exit(Player* player, GameLayer* mainLayer) {
}

//Neutral State:
void Player::NeutralState::enter(Player* player, GameLayer* mainLayer, float time) {
	player->moveSpeed = 1.0f;
	player->setSpeed(player->moveSpeed);
	player->setSpriteFrame(player->stand.animation->getFrames().at(0)->getSpriteFrame());
}
Player::State* Player::NeutralState::update(Player* player, GameLayer* mainLayer, float time) {
	//if (player->checkDead() == true) { return new DeathState; }
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
	if (input == NO_CLIP) {
		return new NoClipState;
	}
	return nullptr;
}

//Hide State:
void Player::HideState::enter(Player* player, GameLayer* mainLayer, float time) {
	player->hideObject = player->objectToHideBehind;
	player->hide();
}
Player::State* Player::HideState::update(Player* player, GameLayer* mainLayer, float time) {
	//if (player->checkDead() == true) { return new DeathState; }
	if (player->isHit == true) {//force exit hiding if hit by enemy
		return player->prevState;
	}
	player->hiding();
	return nullptr;
}
Player::State* Player::HideState::handleInput(Player* player, GameLayer* mainLayer, float time, Input input) {
	if (input == HIDE) {
		return player->prevState;
	}
	if (input == MOVE_LEFT) {
		if (player->hittingLeft == false) {
			player->walk(input, time);
		}
	}
	else if (input == MOVE_RIGHT) {
		if (player->hittingRight == false) {
			player->walk(input, time);
		}
	}
	else if (input == STOP) {
		player->walk(input, time);
	}
	return nullptr;
}
void Player::HideState::exit(Player* player, GameLayer* mainLayer) {
	player->moveSpeed = 1.0f;
	player->setSpeed(player->moveSpeed);
	player->hide();
}

//Attack State(using items):
void Player::AttackState::enter(Player* player, GameLayer* mainLayer, float time) {
	player->moveSpeed = (0.4f);
	player->setSpeed(player->moveSpeed);
	player->attackPrepareTime = time;
	player->beginUseItem(0);
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
		player->setSpriteFrame(player->stab.animation->getFrames().at(0)->getSpriteFrame());
		player->heldItem->getPhysicsBody()->setEnabled(false);
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
			player->aimAngle = -90;
		}
		if (input == AIM_DOWN) {
			player->aimAngle = 90;
		}
		if (player->flippedX == true) {//only register left inputs for aiming while facing left
			if (input == AIM_UP_LEFT) {
				player->aimAngle = -45;
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
				player->aimAngle = -45;
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
			player->setSpriteFrame(player->stab.animation->getFrames().at(0)->getSpriteFrame());
		}
		//input for releasing attack
		if (input == USE_RELEASE) {
			player->attackRelease = true;
			player->walk(STOP, time);
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
		player->heldItem->initHeldItem();
	}
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
	player->getPhysicsBody()->setVelocityLimit(250);
	player->getPhysicsBody()->setGravityEnable(false);
	player->noclip();
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
	player->getPhysicsBody()->setVelocityLimit(10000);
	player->getPhysicsBody()->setGravityEnable(true);
	player->noclip();
	player->moveSpeed = 1;
	player->setSpeed(player->moveSpeed);
	//player->hidden = false;
}
void Player::noclip() {
	if (clip == false) {
		clip = true;
		getPhysicsBody()->setCollisionBitmask(0);
	}
	else {
		clip = false;
		getPhysicsBody()->setCollisionBitmask(30);
	}
}