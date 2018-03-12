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
	pickUpRadiusBody->setName("item_radius");
	pickUpRadius->setPhysicsBody(pickUpRadiusBody);

	this->addChild(pickUpRadius);
}

//used when player picks up item
void Item::initHeldItem() {
	this->removeChildByName("item_radius", true);
	this->getPhysicsBody()->setDynamic(false);
	this->setName("held_item");
	this->getPhysicsBody()->setName("held_item");
	this->setPositionNormalized(Vec2(0.86, 0.415));
}

//Knife Class:
Knife::Knife()
{
	Item::Item();
	//sprite properties
	//scale = 0.38f;
	//physics body properties
	tag = 10100;//10100 - 10199 for knives
}
Knife::~Knife(){
}
