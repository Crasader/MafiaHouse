#include "Player.h"
#include "Wall.h"

Player::Player()
{
	//reversedX = true;
	//sprite properties
	zOrder = 5;
	scale = 1.0f;
	name = "player";
	//physics body properties
	tag = 1;
	dynamic = true;
	category = 1;
	collision = 22;

	maxSpeed = 70;
}
Player::~Player(){
}

void Player::initObject(Vec2 startPos) {
	GameObject::initObject(startPos);
}

void Player::initAnimations() {
	//auto frames = getAnimation("player/stand/%04d.png", 10);//change number of frames to correct number
}

//functions for player actions:
void Player::resetActionChecks() {
	doorToUse = -1;
	stairToUse = -1;
	objectToHideBehind = -1;
	itemToPickUp = -1;
	bodyToPickUp = -1;
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
		setSpriteFrame(standing.animation->getFrames().at(0)->getSpriteFrame());
		stopActionByTag(WALK);
		stopActionByTag(MOONWALK);
		stopX();
	}
}

void Player::pickUpItem(Node* mainLayer) {
	heldItem = static_cast<Item*>(mainLayer->getChildByTag(itemToPickUp));
	mainLayer->removeChildByTag(itemToPickUp, true);

	removeChild(heldItem, true);
	addChild(heldItem);
	heldItem->initHeldItem();
	inventory.push_back(heldItem);

	itemToPickUp = -1;
}

void Player::dropItem(Node* mainLayer) {
	//removing dropped item from inventory
	for (int i = 0; i < inventory.size(); i++) {
		if (inventory[i] == heldItem) {
			inventory.erase(inventory.begin() + i);
			break;
		}
	}
	heldItem->initDroppedItem(convertToWorldSpace(heldItem->getPosition()), flippedX);

	removeChild(heldItem, true);
	mainLayer->addChild(heldItem);
	heldItem = NULL;
}

void Player::beginUseItem() {
	if (heldItem->getAttackType() == Item::STAB) {
		heldItem->beginStab();
		setSpriteFrame(stabbing.animation->getFrames().at(0)->getSpriteFrame());//setting player sprite to first frame of stab animation
	}
	else if (heldItem->getAttackType() == Item::SWING) {
		heldItem->beginSwing();
		//setSpriteFrame(swingAnimation->getFrames[0]);//setting player sprite to first frame of stab animation
	}
}

void Player::useItem() {
	heldItem->getPhysicsBody()->setEnabled(true);
	if (heldItem->getAttackType() == Item::STAB) {
		heldItem->stabSequence();
		setSpriteFrame(stabbing.animation->getFrames().at(1)->getSpriteFrame());
		//runAction(Animate::create(stabAnimation));//runs stabbing animation
	}
	else if (heldItem->getAttackType() == Item::SWING) {
		heldItem->swingSequence();
		//runAction(Animate::create(swingAnimation));//runs swinging animation
	}
}

void Player::useDoor(Node* mainLayer) {
	Door* door = static_cast<Door*>(mainLayer->getChildByTag(doorToUse));
	door->use();
}

void Player::useStair(Node* mainLayer) {
	Stair* stair = static_cast<Stair*>(mainLayer->getChildByTag(stairToUse));
	stair->use(this, mainLayer);
}

void Player::hide(Node* mainLayer) {
	auto hideObject = mainLayer->getChildByTag(objectToHideBehind);
	if (hidden == false) {
		hidden = true;
		getPhysicsBody()->setTag(2);//for enemy vision rays
		setGlobalZOrder(getGlobalZOrder() - 3);
		if (heldItem != NULL) {
			heldItem->setGlobalZOrder(heldItem->getGlobalZOrder() - 3);
		}
		hideObject->setOpacity(175);
	}
	else {
		hidden = false;
		getPhysicsBody()->setTag(1);//for enemy vision rays
		setGlobalZOrder(getGlobalZOrder() + 3);
		if (heldItem != NULL) {
			heldItem->setGlobalZOrder(heldItem->getGlobalZOrder() + 3);
		}
		hideObject->setOpacity(255);
	}
}
//have player stay behind object they are hiding behind
void Player::hiding(Node* mainLayer) {
	auto hideObject = mainLayer->getChildByTag(objectToHideBehind);
	followBox(this, hideObject, Vec2((hideObject->getContentSize().width / 2.0f) - (getContentSize().width / 2.0f), hideObject->getContentSize().height / 2.0f), Vec2((hideObject->getContentSize().width / 2.0f) - (getContentSize().width / 2.0f), hideObject->getContentSize().height / 2.0f));
}

void Player::noclip() {
	if (clip == false) {
		clip = true;
		getPhysicsBody()->setCollisionBitmask(20);
	}
	else {
		clip = false;
		getPhysicsBody()->setCollisionBitmask(22);
	}
}

//Update Checking:
void Player::update(Node* mainLayer, float time) {
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
void Player::handleInput(Node* mainLayer, float time, Input input) {
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
void Player::PlayerState::enter(Player* player, Node* mainLayer, float time) {
}
Player::PlayerState* Player::PlayerState::handleInput(Player* player, Node* mainLayer, float time, Input input) {
	return nullptr;
}
Player::PlayerState* Player::PlayerState::update(Player* player, Node* mainLayer, float time) {
	return nullptr;
}
void Player::PlayerState::exit(Player* player, Node* mainLayer) {
}

void Player::NeutralState::enter(Player* player, Node* mainLayer, float time) {
	player->setSpriteFrame(player->standing.animation->getFrames().at(0)->getSpriteFrame());
}
//Neutral State:
Player::PlayerState* Player::NeutralState::handleInput(Player* player, Node* mainLayer, float time, Input input) {
	if (input == USE_DOOR) {
		player->useDoor(mainLayer);
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
void Player::HideState::enter(Player* player, Node* mainLayer, float time) {
	player->hide(mainLayer);
}
Player::PlayerState* Player::HideState::update(Player* player, Node* mainLayer, float time) {
	player->hiding(mainLayer);
	return nullptr;
}
Player::PlayerState* Player::HideState::handleInput(Player* player, Node* mainLayer, float time, Input input) {
	if (input == HIDE) {
		return player->prevState;
	}
	if (input == MOVE_LEFT || input == MOVE_RIGHT || input == STOP) {
		player->walk(input);
	}
	return nullptr;
}
void Player::HideState::exit(Player* player, Node* mainLayer) {
	player->hide(mainLayer);
}

//Attack State(using items):
void Player::AttackState::enter(Player* player, Node* mainLayer, float time) {
	player->moveSpeed = (0.3f);
	player->setSpeed(player->moveSpeed);
	player->attackPrepareTime = time;
	player->beginUseItem();
}
Player::PlayerState* Player::AttackState::update(Player* player, Node* mainLayer, float time) {
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
Player::PlayerState* Player::AttackState::handleInput(Player* player, Node* mainLayer, float time, Input input) {
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
void Player::AttackState::exit(Player* player, Node* mainLayer) {
	player->moveSpeed = (1.0f);
	player->setSpeed(player->moveSpeed);
	player->heldItem->initHeldItem();
}

//No Clip state:
void Player::NoClipState::enter(Player* player, Node* mainLayer, float time) {
	player->noclip();
}
Player::PlayerState* Player::NoClipState::handleInput(Player* player, Node* mainLayer, float time, Input input) {
	if (input == USE_DOOR) {
		player->useDoor(mainLayer);
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
void Player::NoClipState::exit(Player* player, Node* mainLayer) {
	player->noclip();
}