#include "Player.h"
#include "Wall.h"

Player::Player()
{
	//sprite properties
	zOrder = 5;
	scale = 1.0f;
	name = "player";
	//physics body properties
	tag = 1;
	dynamic = true;
	category = 1;
	collision = 22;

	maxSpeed = 150;
}
Player::~Player(){
}

void Player::initObject(Vec2 startPos) {
	GameObject::initObject(startPos);
}

void Player::initAnimations() {
	//auto frames = getAnimation("player/stand/%04d.png", 10);//change number of frames to correct number
	//standAnimation = Animation::createWithSpriteFrames(frames, 1.0f / 8.0f);//change number to correct speed for 
	//frames = getAnimation("player/walk/%04d.png", 10);//change number of frames to correct number
	//walkAnimation = Animation::createWithSpriteFrames(frames, 1.0f / 8.0f);//change number to correct speed for animation
	//etc...
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
		if (turned == false) {
			//run walking animation
			turned = true;
			flipX();
		}
		move(Vec2(10.0f, 0));
	}
	else if (input == MOVE_RIGHT) {
		if (turned == true) {
			//run walking animation
			turned = false;
			flipX();
		}
		move(Vec2(10.0f, 0));
	}
	else if (input == STOP) {
		//run standing animation
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

void Player::useItem() {
	runAction(Animate::create(stabAnimation));
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
		setGlobalZOrder(getGlobalZOrder() - 3);
		if (heldItem != NULL) {
			heldItem->setGlobalZOrder(heldItem->getGlobalZOrder() - 3);
		}
		hideObject->setOpacity(175);
	}
	else {
		hidden = false;
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

//Input Handling:
void Player::handleInput(Input input, Node* mainLayer) {
	newState = state->handleInput(this, input, mainLayer);
	if (newState != NULL)
	{
		state->exit(this, mainLayer);

		if (prevState != NULL && newState != prevState){delete prevState;}
		prevState = state;
		state = newState;
		newState = NULL;

		state->enter(this, mainLayer);
	}
}
//Update Checking:
void Player::update(float time, Node* mainLayer) {
	newState = state->update(this, time, mainLayer);
	if (newState != NULL)
	{
		state->exit(this, mainLayer);

		if (prevState != NULL && newState != prevState) { delete prevState; }
		prevState = state;
		state = newState;
		newState = NULL;

		state->enter(this, mainLayer);
	}
}

//Player States:
void Player::PlayerState::enter(Player* player, Node* mainLayer) {
}
Player::PlayerState* Player::PlayerState::handleInput(Player* player, Input input, Node* mainLayer) {
	return nullptr;
}
Player::PlayerState* Player::PlayerState::update(Player* player, float time, Node* mainLayer) {
	return nullptr;
}
void Player::PlayerState::exit(Player* player, Node* mainLayer) {
}

//Neutral State:
Player::PlayerState* Player::NeutralState::handleInput(Player* player, Input input, Node* mainLayer) {
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
void Player::HideState::enter(Player* player, Node* mainLayer) {
	player->hide(mainLayer);
}
Player::PlayerState* Player::HideState::update(Player* player, float time, Node* mainLayer) {
	player->hiding(mainLayer);
	return nullptr;
}
Player::PlayerState* Player::HideState::handleInput(Player* player, Input input, Node* mainLayer) {
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
void Player::AttackState::enter(Player* player, Node* mainLayer) {
	player->useItem();
}

//No Clip state:
void Player::NoClipState::enter(Player* player, Node* mainLayer) {
	player->noclip();
}
Player::PlayerState* Player::NoClipState::handleInput(Player* player, Input input, Node* mainLayer) {
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