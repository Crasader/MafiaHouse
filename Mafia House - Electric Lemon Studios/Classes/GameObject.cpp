#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

GameObject* GameObject::create(const std::string& filename)
{
	GameObject *sprite = new (std::nothrow) GameObject();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

void GameObject::initObject(Vec2 startPos){
	//necessary stuff, will not change between objects:
	this->setAnchorPoint(Vec2(0, 0));

	//set scale of sprite
	this->setScale(scale);

	//set position of sprite
	this->setPosition(startPos);

	//creating physics body
	auto body = PhysicsBody::createBox(this->getContentSize());
	//auto size = sprite->getContentSize();
	//body->setPositionOffset(Vec2(size.width/2, size.height/2));

	//not necessary, will change from object to object:
	body->setDynamic(dynamic);
	body->setCategoryBitmask(category);
	body->setCollisionBitmask(collision);

	//necessary stuff, will not change between objects:
	body->setContactTestBitmask(0xFFFFFFFF);
	body->setTag(tag);

	this->setPhysicsBody(body);
}

void GameObject::initAnimations() {

}

Vector<SpriteFrame*> GameObject::getAnimation(const char *format, int count){
	auto spritecache = SpriteFrameCache::getInstance();
	Vector<SpriteFrame*> animFrames;
	char str[100];
	for (int i = 1; i <= count; i++)
	{
		sprintf(str, format, i);
		animFrames.pushBack(spritecache->getSpriteFrameByName(str));
	}
	return animFrames;
}