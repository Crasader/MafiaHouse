#include "Item.h"



Item::Item()
{
	//sprite properties
	//spriteName = "knife.png";
	scale = 0.6f;
	//physics body properties
	tag = 100;//eat item will have a different tag number
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

void Item::initObject() {
	scale = 0.4f;
	GameObject::initObject();
	this->setFlippedX(true);
}