#include "Player.h"

Player::Player()
{
	//sprite properties
	//spriteName = "player.png";
	scale = 1.5f;
	//physics body properties
	tag = 1;
	dynamic = true;
	category = 1;
	collision = 14;

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
		if (itemToPickUp == 100) {
			item = Item::create();
		}

		if (item != NULL) {
			item->initObject();

			items.push_back(item);

			this->removeChildByName("item", true);

			this->addChild(items.back());

			heldItem = items.back();

			itemToPickUp = -1;
		}
	}
}
