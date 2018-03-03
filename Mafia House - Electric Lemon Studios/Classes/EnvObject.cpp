#include "EnvObject.h"



EnvObject::EnvObject()
{
	//sprite properties
	zOrder = 3;
	scale = 2.0f;
	name = "env_object";//used for collision detection
	//physics body properties
	tag = 20000;//each object will have a unique tag number
	dynamic = true;
	category = 16;
	collision = 1;
}


EnvObject::~EnvObject()
{
}

EnvObject* EnvObject::create(const std::string& filename)
{
	EnvObject *sprite = new (std::nothrow) EnvObject();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}
/*
void EnvObject::initObject(Vec2 startPos) {
	GameObject::initObject(startPos);
	this->setFlippedX(true);
	//initializing pickup radius
	pickUpRadius = Node::create();
	pickUpRadius->setPositionNormalized(Vec2(0.5, 0.5));

	auto pickUpRadiusBody = PhysicsBody::createCircle(100.0f);
	pickUpRadiusBody->setDynamic(false);
	pickUpRadiusBody->setCategoryBitmask(4);
	pickUpRadiusBody->setCollisionBitmask(1);
	pickUpRadiusBody->setContactTestBitmask(0xFFFFFFFF);
	pickUpRadiusBody->setTag(3);
	pickUpRadius->setPhysicsBody(pickUpRadiusBody);

	this->addChild(pickUpRadius);
}*/
/*
void EnvObject::initObject() {
	scale = 0.4f;
	tag += 100;//adding 100 to tag to differentiate items player is carrying from items in level
	GameObject::initObject();
	this->setPositionNormalized(Vec2(1, 0.5));
	this->setFlippedX(true);
	this->getPhysicsBody()->setMass(0);
}*/