#include "Player.h"

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

//functions for player actions:
void Player::resetActionChecks() {
	doorToUse = -1;
	stairToUse = -1;
	objectToHideBehind = -1;
	itemToPickUp = -1;
	bodyToPickUp = -1;
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