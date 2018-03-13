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
		stop();
	}
}

void Player::pickUpItem(Node* mainLayer) {
	if (itemToPickUp != -1 && heldItem == NULL) {
		heldItem = static_cast<Item*>(mainLayer->getChildByTag(itemToPickUp));
		mainLayer->removeChildByTag(itemToPickUp, true);

		removeChild(heldItem, true);
		addChild(heldItem);
		heldItem->initHeldItem();
		inventory.push_back(heldItem);

		itemToPickUp = -1;
	}
}

void Player::dropItem(Node* mainLayer) {
	if (heldItem != NULL) {
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
}

void Player::useItem() {
	runAction(Animate::create(stabAnimation));
}

void Player::useDoor(Node* mainLayer) {
	if (doorToUse != -1) {
		Door* door = static_cast<Door*>(mainLayer->getChildByTag(doorToUse));
		door->use();
	}
}

void Player::useStair(Node* mainLayer) {
	if (stairToUse != -1) {
		Stair* stair = static_cast<Stair*>(mainLayer->getChildByTag(stairToUse));
		stair->use(this, mainLayer);
	}
}

void Player::hide(Node* mainLayer) {
	if (objectToHideBehind != -1) {
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
}
//have player stay behind object they are hiding behind
void Player::hiding(Node* mainLayer) {
	if (hidden == true) {
		auto hideObject = mainLayer->getChildByTag(objectToHideBehind);
		followBox(this, hideObject, Vec2((hideObject->getContentSize().width / 2.0f) - (getContentSize().width / 2.0f), hideObject->getContentSize().height / 2.0f), Vec2((hideObject->getContentSize().width / 2.0f) - (getContentSize().width / 2.0f), hideObject->getContentSize().height / 2.0f));
	}
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

void Player::handleInput(Input input) {
	newState = state->handleInput(this, input);
	if (newState != NULL)
	{
		//delete state;
		state = newState;
		newState = NULL;

		state->enter(this);
	}
}

//Player States:
Player::PlayerState::~PlayerState() {}

Player::PlayerState* Player::PlayerState::handleInput(Player* player, Input input) {
	return new PlayerState;
}

void Player::PlayerState::enter(Player* player) {}

Player::PlayerState* Player::NeutralState::handleInput(Player* player, Input input) {
	if (input == USE_ITEM) {
		return new AttackState();
	}
}

void Player::AttackState::enter(Player* player) {
	player->useItem();
}