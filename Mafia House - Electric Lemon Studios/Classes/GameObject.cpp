#include "GameObject.h"

GameObject::GameObject(){
}
GameObject::~GameObject(){
}

void GameObject::initObject() {
	setPositionZ(0.0f);
	//set the anchor point to bottom left corner
	setAnchorPoint(Vec2(0, 0));

	//disabling anti-aliasing!!! (looks like blurry poop without this)
	Texture2D::TexParams texParams = { GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };
	getTexture()->setTexParameters(texParams);

	//set name of sprite
	setName(name);

	//set Z order
	setGlobalZOrder(zOrder);

	//set scale of sprite
	setScale(scale);

	//set whether sprite is flipped or not
	setFlippedX(reversedX);

	//set tag
	setTag(tag);

	//creating physics body
	auto body = PhysicsBody::createBox(getContentSize());
	body->setTag(tag);
	body->setName(name);

	//not necessary, will change from object to object:
	body->setDynamic(dynamic);
	body->setCategoryBitmask(category);
	body->setCollisionBitmask(collision);

	//necessary stuff, will not change between objects:
	body->setRotationEnable(rotate);
	body->setContactTestBitmask(0xFFFFFFFF);

	body->setVelocityLimit(maxSpeed);//max object speed

	setPhysicsBody(body);
}

void GameObject::initObject(Vec2 startPos) {
	//set position of sprite
	setPosition(startPos);

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
	setPosition(roomPos + offset);
}

void GameObject::setRoomPosition(Vec2 roomPos, Vec2 position) {
	setPosition(roomPos + position);
}

void GameObject::stopX() {
	getPhysicsBody()->setVelocity(Vec2(0, getPhysicsBody()->getVelocity().y));
	getPhysicsBody()->resetForces();
}

void GameObject::stop() {
	getPhysicsBody()->setVelocity(Vec2(0, 0));
	getPhysicsBody()->resetForces();
}

void GameObject::slowStop() {
	getPhysicsBody()->resetForces();
	Vec2 force = getPhysicsBody()->getMass() * getPhysicsBody()->getVelocity();
	getPhysicsBody()->applyForce(-force);
}

void GameObject::move(Vec2 velocity) {//positive values will always move them forward/up relative to the direction they are facing
	auto mass = getPhysicsBody()->getMass();

	Vec2 force = mass * velocity;

	//reversing direction of movement if character is flipped
	force.x = flippedX == true ? force.x * -1 : force.x;
	force.y = flippedY == true ? force.y * -1 : force.y;

	getPhysicsBody()->applyImpulse(force);
}

void GameObject::flipX() {
	setScaleX(getScaleX() * -1);//flips sprite and it's children by inverting x scale
	if (flippedX == false) {
		flippedX = true;
		setAnchorPoint(Vec2(1, 0));//have to change anchor point to opposite corner after flipping or the sprite will change position
	}
	else {
		flippedX = false;
		setAnchorPoint(Vec2(0, 0));
	}
}
