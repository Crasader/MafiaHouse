#include "Player.h"
#include "Wall.h"

Player::Player()
{
	//sprite properties
	zOrder = 5;
	scale = 1.0f;
	name = "player";
	//physics body properties
	bodySize = Size(26, 90);
	tag = 1;
	dynamic = true;
	category = 1;
	collision = 30;
	//max movement speed
	maxSpeed = 70;
	//initializing animations
	standing = GameAnimation(STAND, "player/stand/%03d.png", 11, 10 FRAMES);
	moonwalking = GameAnimation(MOONWALK, "player/walk2/%03d.png", 7, 8 FRAMES);
	walking = GameAnimation(WALK, "player/walk/%03d.png", 6, 10 FRAMES);
	stabbing = GameAnimation(WALK, "player/stab/%03d.png", 2, 10 FRAMES);
}
Player::~Player(){
}

//functions for player actions:
void Player::resetCollisionChecks() {
	doorToUse = NULL;
	stairToUse = NULL;
	objectToHideBehind = NULL;
	itemToPickUp = NULL;
	//bodyToPickUp = NULL;
}

void Player::walk(Input input) {
	if (input == MOVE_LEFT) {
		if (moveDirection == 0){
			moveDirection = 1;
			if (turned == false) {
				turned = true;
				flipX();
			}
			stopActionByTag(MOONWALK);
			if (getActionByTag(WALK) == NULL) {
				walkingSound = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/walk.wav", true);
				runAction(walking.action);
			}
			setSpeed(moveSpeed);
		}
		if (moveDirection == 1) {
			walking.action->setSpeed(moveSpeed);
			moveAbsolute(Vec2(-9.0f * moveSpeed, 0));
			//run walking animation
		}
		else if (moveDirection == 2) {
			setSpeed(moveSpeed * 1.4f);
			moonwalking.action->setSpeed(moveSpeed);
			if (turned == false) {
				turned = true;
				flipX();
				stopActionByTag(WALK);
				if (getActionByTag(MOONWALK) == NULL) {
					runAction(moonwalking.action);
				}
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
			stopActionByTag(MOONWALK);
			if (getActionByTag(WALK) == NULL) {
				walkingSound = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/walk.wav", true);
				runAction(walking.action);
			}
			setSpeed(moveSpeed);
		}
		if (moveDirection == 2) {
			walking.action->setSpeed(moveSpeed);
			moveAbsolute(Vec2(9.0f * moveSpeed, 0));
			//run walking animation
		}
		else if (moveDirection == 1) {
			setSpeed(moveSpeed * 1.4f);
			moonwalking.action->setSpeed(moveSpeed);
			if (turned == true) {
				turned = false;
				flipX();
				stopActionByTag(WALK);
				if (getActionByTag(MOONWALK) == NULL) {
					runAction(moonwalking.action);
				}
			}
		}
	}
	if (input == STOP) {
		moveDirection = 0;
		//run standing animation
		CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(walkingSound);
		setSpriteFrame(standing.animation->getFrames().at(0)->getSpriteFrame());
		stopActionByTag(WALK);
		stopActionByTag(MOONWALK);
		stopX();
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
		hidden = true;
		setTag(2);
		getPhysicsBody()->setTag(2);//for enemy vision rays
		setGlobalZOrder(getGlobalZOrder() - 3);
		if (heldItem != NULL) {
			heldItem->setGlobalZOrder(heldItem->getGlobalZOrder() - 3);
		}
		hideObject->setOpacity(175);
	}
	else {
		hidden = false;
		setTag(1);
		getPhysicsBody()->setTag(1);//for enemy vision rays
		setGlobalZOrder(getGlobalZOrder() + 3);
		if (heldItem != NULL) {
			heldItem->setGlobalZOrder(heldItem->getGlobalZOrder() + 3);
		}
		hideObject->setOpacity(255);
	}
}
//have player stay behind object they are hiding behind
void Player::hiding() {
	followBox(this, hideObject, Vec2((hideObject->getContentSize().width / 2.0f) - (bodySize.width / 2.0f), hideObject->getContentSize().height / 2.0f), Vec2((hideObject->getContentSize().width / 2.0f) - (getContentSize().width / 2.0f), hideObject->getContentSize().height / 2.0f));
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

//Update Checking:
void Player::update(GameLayer* mainLayer, float time) {
	updateFloor(mainLayer->floors);//checking if floor has changed
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
	newState = state->handleInput(this, mainLayer, time, input);
	if (newState != NULL)
	{
		state->exit(this, mainLayer);

		if (prevState != NULL && newState != prevState){delete prevState;}
		prevState = state;
		state = newState;
		newState = NULL;

		state->enter(this, mainLayer, time);
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
	player->setSpriteFrame(player->standing.animation->getFrames().at(0)->getSpriteFrame());
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
		player->walk(input);
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
	if (input == MOVE_LEFT || input == MOVE_RIGHT || input == STOP) {
		player->walk(input);
	}
	return nullptr;
}
void Player::HideState::exit(Player* player, GameLayer* mainLayer) {
	player->hide();
}

//Attack State(using items):
void Player::AttackState::enter(Player* player, GameLayer* mainLayer, float time) {
	player->moveSpeed = (0.3f);
	player->setSpeed(player->moveSpeed);
	player->attackPrepareTime = time;
	player->beginUseItem();
}
Player::State* Player::AttackState::update(Player* player, GameLayer* mainLayer, float time) {
	if (player->attackRelease == true && player->attackPrepareTime != -1.0f && time - player->attackPrepareTime >= player->heldItem->getStartTime()) {
		player->attackStartTime = time;
		player->useItem();
		player->attackPrepareTime = -1.0f;
	}
	if (player->attackStartTime != -1.0f && time - player->attackStartTime >= player->heldItem->getAttackTime()) {
		player->setSpriteFrame(player->stabbing.animation->getFrames().at(0)->getSpriteFrame());
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
	if ((player->attackRelease == false) && (input == MOVE_LEFT || input == MOVE_RIGHT || input == STOP)) {
		player->walk(input);
		player->setSpriteFrame(player->stabbing.animation->getFrames().at(0)->getSpriteFrame());
	}
	if (input == USE_RELEASE) {
		player->attackRelease = true;
		player->walk(STOP);
	}
	return nullptr;
}
void Player::AttackState::exit(Player* player, GameLayer* mainLayer) {
	player->moveSpeed = (1.0f);
	player->setSpeed(player->moveSpeed);
	if (player->heldItem->checkUsed() == true) {
		player->breakItem();
	}
	else{
		player->heldItem->initHeldItem();
	}
}

//No Clip state:
void Player::NoClipState::enter(Player* player, GameLayer* mainLayer, float time) {
	player->noclip();
	player->moveSpeed = 3;
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
		player->walk(input);
	}
	if (input == MOVE_UP) {
		player->move(Vec2(0, 25));
	}
	else if (input == MOVE_DOWN) {
		player->move(Vec2(0, -25));
	}
	if (input == NO_CLIP) {
		return new NeutralState;
	}
	return nullptr;
}
void Player::NoClipState::exit(Player* player, GameLayer* mainLayer) {
	player->noclip();
	player->moveSpeed = 1;
}