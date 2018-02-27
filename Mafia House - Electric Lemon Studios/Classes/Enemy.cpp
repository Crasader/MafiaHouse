#include "Enemy.h"

Enemy::Enemy()
{
	//sprite properties
	zOrder = 4;
	scale = 1.5f;
	//physics body properties
	tag = 30000;//enemies will be 30000 - 29999
	dynamic = true;
	category = 4;
	collision = 3;
}


Enemy::~Enemy()
{
}

Enemy* Enemy::create(const std::string& filename)
{
	Enemy *sprite = new (std::nothrow) Enemy();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

void Enemy::initObject(Vec2 startPos) {
	GameObject::initObject(startPos);
	//initializing vision cone
	vision = GameObject::create("visionCone.png");
	vision->initObject();
	vision->setName("vision_cone");
	vision->setScale(0.75);
	vision->setPositionNormalized(Vec2(1, 0.4));
	vision->setGlobalZOrder(6);

	vision->getPhysicsBody()->setCategoryBitmask(4);
	vision->getPhysicsBody()->setCollisionBitmask(1);

	this->addChild(vision);
}