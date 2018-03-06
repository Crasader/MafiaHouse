#include "Item.h"

Item::Item()
{
	name = "item";
	//sprite properties
	zOrder = 5;
	//physics body properties
	category = 8;
	collision = 3;
	tag = 10000;//eac_h item type will be identified by the second and third digit: 10100 - 10199 for knives
	dynamic = true;
}
Item::~Item(){
}

void Item::initObject(Vec2 startPos)
{
	GameObject::initObject(startPos);
	//initializing pickup radius
	Size pickUpBox = this->getContentSize() * 2.0;
	auto pickUpRadius = Node::create();
	pickUpRadius->setPositionNormalized(Vec2(0.5, 0.5));
	pickUpRadius->setName("item_radius");

	auto pickUpRadiusBody = PhysicsBody::createBox(pickUpBox);
	pickUpRadiusBody->setDynamic(false);
	pickUpRadiusBody->setCategoryBitmask(4);
	pickUpRadiusBody->setCollisionBitmask(1);
	pickUpRadiusBody->setContactTestBitmask(0xFFFFFFFF);
	pickUpRadiusBody->setTag(10000);
	pickUpRadius->setPhysicsBody(pickUpRadiusBody);

	this->addChild(pickUpRadius);
}

//used when player picks up item
void Item::initHeldItem(int itemTag)
{
	tag = itemTag;
	name = "held_item";
	dynamic = false;
	category = 4;
	GameObject::initObject();
	this->setPositionNormalized(Vec2(0.86, 0.415));
	//this->getPhysicsBody()->setMass(0);
}

//Knife Class:
Knife::Knife()
{
	Item::Item();
	//sprite properties
	//scale = 0.38f;
	//flippedX = true;
	//physics body properties
	tag = 10100;//10100 - 10199 for knives
}
Knife::~Knife(){
}
