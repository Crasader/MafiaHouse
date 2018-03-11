#include "GameObject.h"

GameObject::GameObject(){
}
GameObject::~GameObject(){
}

void GameObject::initObject() {
	this->setPositionZ(0.0f);
	//set the anchor point to bottom left corner
	this->setAnchorPoint(Vec2(0, 0));

	//disabling anti-aliasing!!! (looks like blurry poop without this)
	Texture2D::TexParams texParams = { GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };
	this->getTexture()->setTexParameters(texParams);

	//set name of sprite
	this->setName(name);

	//set Z order
	this->setGlobalZOrder(zOrder);

	//set scale of sprite
	this->setScale(scale);

	//set whether sprite is flipped or not
	this->setFlippedX(startFlippedX);

	//set tag
	this->setTag(tag);

	//creating physics body
	auto body = PhysicsBody::createBox(this->getContentSize());
	body->setTag(tag);
	body->setName(name);

	//not necessary, will change from object to object:
	body->setDynamic(dynamic);
	body->setCategoryBitmask(category);
	body->setCollisionBitmask(collision);

	//necessary stuff, will not change between objects:
	body->setRotationEnable(false);
	body->setContactTestBitmask(0xFFFFFFFF);

	body->setVelocityLimit(maxSpeed);//max object speed

	this->setPhysicsBody(body);
}

void GameObject::initObject(Vec2 startPos) {
	//set position of sprite
	this->setPosition(startPos);

	GameObject::initObject();
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

void GameObject::setRoomPositionNormalized(Vec2 roomPos, Size roomSize, Vec2 position) {
	Vec2 offset = Vec2(position.x * roomSize.width, position.y * roomSize.height);
	this->setPosition(roomPos + offset);
}

void GameObject::setRoomPosition(Vec2 roomPos, Vec2 position) {
	this->setPosition(roomPos + position);
}

void GameObject::stop() {
	this->getPhysicsBody()->setVelocity(Vec2(0, 0));
	this->getPhysicsBody()->resetForces();
}

void GameObject::move(Vec2 velocity) {
	auto mass = this->getPhysicsBody()->getMass();

	Vec2 force = mass * velocity;

	force.x = flippedX == true ? force.x * -1 : force.x;
	force.y = flippedY == true ? force.y * -1 : force.y;

	this->getPhysicsBody()->applyImpulse(force);
}

void GameObject::flipX() {
	this->setScaleX(this->getScaleX() * -1);//flips sprite and it's children by inverting x scale
	if (flippedX == false) {
		flippedX = true;
		this->setAnchorPoint(Vec2(1, 0));//have to change anchor point to opposite corner after flipping or the sprite will change position
	}
	else {
		flippedX = false;
		this->setAnchorPoint(Vec2(0, 0));
	}
}
