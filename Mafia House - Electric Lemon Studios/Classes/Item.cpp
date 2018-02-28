#include "Item.h"

Item::Item()
{
	//sprite properties
	zOrder = 5;
	scale = 0.6f;
	name = "item";
	//physics body properties
	tag = 10000;//each item type will be identified by the second and third digit: 10100 - 10199 for knives
	dynamic = false;
	category = 8;
	collision = 1;
}

Item::~Item()
{
}

Item* Item::create(const std::string& filename)
{
	Item *sprite = new (std::nothrow) Item();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

void Item::initObject(Vec2 startPos) {
	GameObject::initObject(startPos);
	this->setFlippedX(true);
	//initializing pickup radius
	/*pickUpRadius = Node::create();
	pickUpRadius->setPositionNormalized(Vec2(0.5, 0.5));

	auto pickUpRadiusBody = PhysicsBody::createCircle(100.0f);
	pickUpRadiusBody->setDynamic(false);
	pickUpRadiusBody->setCategoryBitmask(4);
	pickUpRadiusBody->setCollisionBitmask(1);
	pickUpRadiusBody->setContactTestBitmask(0xFFFFFFFF);
	pickUpRadiusBody->setTag(3);
	pickUpRadius->setPhysicsBody(pickUpRadiusBody);

	this->addChild(pickUpRadius);*/
}

//used when player picks up item
void Item::initHeldItem(int itemTag) {
	tag = itemTag;
	name = "held_item";
	scale = 0.4f;
	GameObject::initObject();
	this->setPositionNormalized(Vec2(1, 0.5));
	this->setFlippedX(true);
	this->getPhysicsBody()->setMass(0);
}

//Knife Class:
Knife::Knife()
{
	//sprite properties
	zOrder = 5;
	scale = 0.6f;
	name = "item";
	//physics body properties
	tag = 10100;//10100 - 10199 for knives 10101, 10102, ... 10187
	dynamic = false;
	category = 8;
	collision = 1;
}

Knife::~Knife()
{
}

Knife* Knife::create(const std::string& filename)
{
	Knife *sprite = new (std::nothrow) Knife();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}
