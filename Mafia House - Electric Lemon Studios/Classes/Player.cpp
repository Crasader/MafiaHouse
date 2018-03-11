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

//called if space bar is pressed and player is colliding with item
void Player::pickUpItem() {
	if (itemToPickUp != -1) {
		Item* item = NULL;
		if (itemToPickUp >= 10100 && itemToPickUp <= 10199) {//10100 - 10199 for knives
			item = Knife::create();
		}

		if (item != NULL) {
			item->initHeldItem(itemToPickUp);

			items.push_back(item);

			this->removeChildByName("held_item", true);

			heldItem = items.back();

			this->addChild(heldItem);

			itemToPickUp = -1;
		}
	}
}

void Player::noclip() {
	if (clip == false) {
		clip = true;
		this->getPhysicsBody()->setCollisionBitmask(20);
	}
	else {
		clip = false;
		this->getPhysicsBody()->setCollisionBitmask(22);
	}
}