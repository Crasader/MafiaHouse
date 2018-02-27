#include "Player.h"

Player::Player()
{
	//sprite properties
	zOrder = 4;
	scale = 1.5f;
	name = "player";
	//physics body properties
	tag = 1;
	dynamic = true;
	category = 1;
	collision = 30;

	init();
}

Player::~Player()
{
}

Player* Player::create(const std::string& filename)
{
	Player *sprite = new (std::nothrow) Player();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

void Player::initObject(Vec2 startPos) {
	GameObject::initObject(startPos);
	
	this->getPhysicsBody()->setVelocityLimit(180);//max player speed
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
