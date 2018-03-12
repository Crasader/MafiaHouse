#include "Item.h"

Item::Item()
{
	name = "item";
	//sprite properties
	zOrder = 6;
	//physics body properties
	category = 8;
	collision = 3;
	tag = 10000;//eac_h item type will be identified by the second and third digit: 10100 - 10199 for knives
	dynamic = true;
	rotate = true;
}
Item::~Item(){
}

void Item::initObject(Vec2 startPos)
{
	GameObject::initObject(startPos);
	retain();
	initRadius();
}
//initializing pickup radius:
void Item::initRadius() {
	Size pickUpBox = getContentSize() * 2.0;
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

	addChild(pickUpRadius);
}

//used when player picks up item
void Item::initHeldItem() {
	removeChildByName("item_radius", true);
	getPhysicsBody()->setDynamic(false);
	setName("held_item");
	getPhysicsBody()->setName("held_item");
	setPosition(Vec2(24, 39));
	if (flippedX == true) {
		flipX();
	}
}
//used when player drops item
void Item::initDroppedItem(Vec2 pos, bool flip) {
	getPhysicsBody()->setDynamic(true);
	setName("item");
	getPhysicsBody()->setName("item");
	if (flip == true) {
		flipX();
		setAnchorPoint(Vec2(0, 0));
	}
	setPosition(pos);
	initRadius();
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
