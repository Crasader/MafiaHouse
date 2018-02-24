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

	//initializing vision cone
	/*heldItem = Item::create();
	heldItem->setScale(0.75);
	heldItem->setAnchorPoint(Vec2(0, 0));
	heldItem->setPositionNormalized(Vec2(1, 0.4));
	heldItem->setTag(22);

	auto visionBody = PhysicsBody::createBox(heldItem->getContentSize());
	visionBody->setDynamic(false);
	visionBody->setCategoryBitmask(4);
	visionBody->setCollisionBitmask(1);
	visionBody->setContactTestBitmask(0xFFFFFFFF);
	visionBody->setTag(2);
	heldItem->setPhysicsBody(visionBody);
	this->addChild(heldItem);*/
}

//functions for player actions:

//called if space bar is pressed and player is colliding with item
void Player::pickUpItem() {
	if (itemToPickUp != -1) {
		Item* item;
		if (itemToPickUp == 100) {
			item = Item::create();
		}

		items.push_back(item);
		items.back()->initObject();
		items.back()->setPositionNormalized(Vec2(1, 0.5));

		this->addChild(items.back());

		itemToPickUp = -1;
	}
}
