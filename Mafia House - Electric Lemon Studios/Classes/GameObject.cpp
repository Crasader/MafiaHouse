#include "GameObject.h"

GameObject::GameObject(){
}
GameObject::~GameObject(){
}

void GameObject::initObject() {
	setPositionZ(0.0f);
	//set the anchor point to bottom left corner. nevermind this messes up animations an
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

	initAutoBody();
}

void GameObject::initObjectNoPhysics() {
	setPositionZ(0.0f);
	//set the anchor point to bottom left corner. nevermind this messes up animations an
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
}

void GameObject::initAutoBody() {
	//creating physics body
	mainBody = PhysicsBody::createBox(getContentSize());
	mainBody->setTag(tag);
	mainBody->setName(name);

	//not necessary, will change from object to object:
	mainBody->setDynamic(dynamic);
	mainBody->setCategoryBitmask(category);
	mainBody->setCollisionBitmask(collision);

	//necessary stuff, will not change between objects:
	mainBody->setRotationEnable(rotate);
	mainBody->setContactTestBitmask(0xFFFFFFFF);

	mainBody->setVelocityLimit(maxSpeed);//max object speed
	mainBody->retain();

	setPhysicsBody(mainBody);
}

void GameObject::initBoxBody(Size size) {
	//creating physics body
	mainBody = PhysicsBody::createBox(size);
	mainBody->setTag(tag);
	mainBody->setName(name);

	//not necessary, will change from object to object:
	mainBody->setDynamic(dynamic);
	mainBody->setCategoryBitmask(category);
	mainBody->setCollisionBitmask(collision);

	//necessary stuff, will not change between objects:
	mainBody->setRotationEnable(rotate);
	mainBody->setContactTestBitmask(0xFFFFFFFF);

	mainBody->setVelocityLimit(maxSpeed);//max object speed
	mainBody->retain();

	setPhysicsBody(mainBody);
}

void GameObject::initObject(Vec2 startPos) {
	//set position of sprite
	setPosition(startPos);
	initObject();
}

void GameObject::initObjectNoPhysics(Vec2 startPos) {
	//set position of sprite
	setPosition(startPos);
	initObjectNoPhysics();
}

void GameObject::createOutline(string name) {
	Texture2D::TexParams texParams = { GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };
	outline = Sprite::createWithSpriteFrameName(name);
	outline->getTexture()->setTexParameters(texParams);
	outline->setPositionNormalized(Vec2(0.5, 0.5));
	outline->setGlobalZOrder(zOrder);
	addChild(outline);
}

void GameObject::setRoomPositionNormalized(Vec2 roomPos, Size roomSize, Vec2 position) {
	Vec2 offset = Vec2(position.x * roomSize.width, position.y * roomSize.height);
	setPosition(roomPos + offset);
	initialPos = getPosition();
}

void GameObject::setRoomPosition(Vec2 roomPos, Vec2 position) {
	setPosition(roomPos + position);
	initialPos = getPosition();
}

void GameObject::updateFloor(vector<FloorData> floors) {
	for (int i = 0; i < floors.size(); i++) {
		if ((getPositionY() > floors[i].bot - 10) && (getPositionY() + getContentSize().height < floors[i].top + 10)) {//player in on the floor, inbetween top and bottom
			currentFloor = i;
			break;
		}
	}
}
void GameObject::updateRoom(vector<RoomData> rooms) {
	for (int i = 0; i < rooms.size(); i++) {
		if ((getPositionX() > rooms[i].left - 10) && (getPositionX() + getContentSize().width < rooms[i].right + 10)) {//player in on the floor, inbetween top and bottom
			currentRoom = i;
			break;
		}
	}
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
	if (abs(getPhysicsBody()->getVelocity().x) < maxSpeed) {//only apply force if they're under thier max speed
		auto mass = getPhysicsBody()->getMass();

		Vec2 force = mass * velocity;

		//reversing direction of movement if character is flipped
		force.x = flippedX == true ? force.x * -1 : force.x;
		force.y = flippedY == true ? force.y * -1 : force.y;

		getPhysicsBody()->applyImpulse(force);
	}
}

void GameObject::moveAbsolute(Vec2 velocity) {//positive values will always move them forward/up relative to the direction they are facing
	if (abs(getPhysicsBody()->getVelocity().x) < maxSpeed) {//only apply force if they're under thier max speed
		auto mass = getPhysicsBody()->getMass();

		Vec2 force = mass * velocity;

		getPhysicsBody()->applyImpulse(force);
	}
}

void GameObject::setSpeed(float speed) {
	//getPhysicsBody()->setVelocityLimit(maxSpeed * speed);//max object speed
	maxSpeed = baseSpeed * speed;
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

Vector<SpriteFrame*> getAnimation(const char *format, int count) {
	auto framecache = SpriteFrameCache::getInstance();
	Vector<SpriteFrame*> frames;
	char str[100];

	for (int i = 1; i <= count; i++){
		sprintf(str, format, i);
		frames.pushBack(framecache->getSpriteFrameByName(str));
	}
	return frames;
}