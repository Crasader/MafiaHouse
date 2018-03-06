#include "Enemy.h"

Enemy::Enemy()
{
	name = "enemy";
	tag = 30000;//enemies will be 30000 - 39999
	//sprite properties
	zOrder = 4;
	scale = 1.0f;
	//physics body properties
	dynamic = true;
	category = 4;
	collision = 3;
}
Enemy::~Enemy(){
}

void Enemy::initObject(Vec2 startPos)
{
	GameObject::initObject(startPos);
	//initializing vision cone
	auto vision = GameObject::create("visionCone.png");
	vision->initObject();
	vision->setName("vision_cone");
	vision->setScale(0.75);
	vision->setPositionNormalized(Vec2(1, 0.4));
	vision->setGlobalZOrder(6);

	vision->getPhysicsBody()->setCategoryBitmask(4);
	vision->getPhysicsBody()->setCollisionBitmask(1);

	this->addChild(vision);
}