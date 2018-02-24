#include "Enemy.h"

Enemy::Enemy()
{
	//sprite properties
	//spriteName = "guard.png";
	scale = 1.5f;
	//physics body properties
	tag = 4;
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
	vision = Sprite::create("visionCone.png");
	vision->setScale(0.75);
	vision->setAnchorPoint(Vec2(0, 0));
	vision->setPositionNormalized(Vec2(1, 0.4));

	auto visionBody = PhysicsBody::createBox(vision->getContentSize());
	visionBody->setDynamic(false);
	visionBody->setCategoryBitmask(4);
	visionBody->setCollisionBitmask(1);
	visionBody->setContactTestBitmask(0xFFFFFFFF);
	visionBody->setTag(2);
	vision->setPhysicsBody(visionBody);

	this->addChild(vision);
}