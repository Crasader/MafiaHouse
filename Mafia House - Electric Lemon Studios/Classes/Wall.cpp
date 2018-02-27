#include "Wall.h"

Wall::Wall()
{
	//sprite properties
	zOrder = 2;
	scale = 1.0f;
	//physics body properties
	tag = 0;
	dynamic = false;
	category = 2;
	collision = 0xFFFFFFFF;
}

Wall::~Wall()
{
}

Wall* Wall::create(const std::string& filename)
{
	Wall *sprite = new (std::nothrow) Wall();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

void Wall::initObject(Vec2 startPos, Size size) {
	this->setContentSize(size);//set the size of the wall
	GameObject::initObject(startPos);
}

//Door Class
Door::Door() {
	//sprite properties
	zOrder = 2;
	scale = 1.0f;
	//physics body properties
	tag = 0;
	dynamic = false;
	category = 2;
	collision = 0xFFFFFFFF;
}
Door::~Door() {

}

Door* Door::create(const std::string& filename)
{
	Door *sprite = new (std::nothrow) Door();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

void Door::initObject(Vec2 startPos) {
	this->setContentSize(doorSize);//set the size of the wall
	GameObject::initObject(startPos);
}

//Room Class
Room::Room() {

}
Room::~Room() {

}

Room* Room::create() {
	Room * ret = new (std::nothrow) Room();
	if (ret && ret->init())
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

//creates a room made of 4 walls
void Room::createRoom(Vec2 position, int length, int height, int thick, int doorSide, int noSide) {
	//floor
	auto wall = Wall::create();
	wall->initObject(position, Size(length + (thick * 2), thick));
	this->addChild(wall);
	//ceiling
	wall = Wall::create();
	wall->initObject(position + Vec2(0, thick + height), Size(length + (thick * 2), thick));
	this->addChild(wall);
	if (doorSide == 0) {
		//left wall
		wall = Wall::create();
		wall->initObject(position + Vec2(0, thick), Size(thick, height));
		this->addChild(wall);
		//right wall
		wall = Wall::create();
		wall->initObject(position + Vec2(thick + length, thick), Size(thick, height));
		this->addChild(wall);
	}
	else if (doorSide == 1) {
		//door
		auto door = Door::create();
		door->initObject(position + Vec2(0, thick));
		this->addChild(door);
		//left wall
		wall = Wall::create();
		wall->initObject(position + Vec2(0, thick + door->doorSize.height), Size(thick, height - door->doorSize.height));
		this->addChild(wall);
		//right wall
		wall = Wall::create();
		wall->initObject(position + Vec2(thick + length, thick), Size(thick, height));
		this->addChild(wall);
	}
	else if (doorSide == 2) {
		//door
		auto door = Door::create();
		door->initObject(position + Vec2(thick + length, thick));
		this->addChild(door);
		//left wall
		wall = Wall::create();
		wall->initObject(position + Vec2(0, thick), Size(thick, height));
		this->addChild(wall);
		//right wall
		wall = Wall::create();
		wall->initObject(position + Vec2(thick + length, thick + door->doorSize.height), Size(thick, height - door->doorSize.height));
		this->addChild(wall);
	}
}