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
	//set position of sprite
	this->setPosition(startPos);

	GameObject::initObject();
}

void GameObject::initObject() {
	//necessary stuff, will not change between objects:
	this->setAnchorPoint(Vec2(0, 0));

	//set Z order
	this->setGlobalZOrder(zOrder);

	//set scale of sprite
	this->setScale(scale);

	//set name of sprite
	this->setName(name);

	//set tag
	this->setTag(tag);

	//creating physics body
	auto body = PhysicsBody::createBox(this->getContentSize());
	//auto size = sprite->getContentSize();
	//body->setPositionOffset(Vec2(size.width/2, size.height/2));

	//not necessary, will change from object to object:
	body->setDynamic(dynamic);
	body->setCategoryBitmask(category);
	body->setCollisionBitmask(collision);

	//necessary stuff, will not change between objects:
	body->setRotationEnable(false);
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

void GameObject::flip() {
	this->setScaleX(this->getScaleX() * -1);//flips sprite and it's children by inverting x scale
	if (this->flipped == true) {
		this->setAnchorPoint(Vec2(1, 0));//have to change anchor point to opposite corner after flipping or the sprite will change position
	}
	else {
		this->setAnchorPoint(Vec2(0, 0));
	}
	//this->setPositionX(this->getPositionX() + this->getContentSize().width);
}