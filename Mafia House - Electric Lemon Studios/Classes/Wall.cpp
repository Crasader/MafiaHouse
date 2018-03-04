#include "Wall.h"

Wall::Wall()
{
	name = "wall";
	tag = 0;
	//sprite properties
	zOrder = 5;
	scale = 1.0f;
	//physics body properties
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

//Stair Class
Stair::Stair() {
	name = "stair";
	tag = 40000;
	//sprite properties
	zOrder = 2;
	scale = 1.0f;
	//physics body properties
	dynamic = false;
	category = 4;
	collision = 0xFFFFFFFF;
}

Stair::~Stair() {

}

Stair* Stair::create(const std::string& filename)
{
	Stair *sprite = new (std::nothrow) Stair();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

void Stair::initObject(StairData data, Vec2 roomPos, Size roomSize) {
	this->setContentSize(stairSize);//won't be needed once we have a proper sprite
	
	GameObject::initObject();
	this->setRoomPosition(roomPos, roomSize, data.position);
	
	type = data.type;
	//use type to set tag of the stairway
	if (type == 2) {
		this->setTag(tag + data.pairNum + 1000);//will have tag of it's partner stairway plus 1000
	}
	else if (type == 1){
		this->setTag(tag + data.pairNum);
	}
}

//Door Class
Door::Door() {
	name = "door";
	tag = 50000;
	//sprite properties
	zOrder = 5;
	scale = 1.0f;
	//physics body properties
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

	auto useRadius = Node::create();
	useRadius->setPositionNormalized(Vec2(0.5, 0.5));
	useRadius->setName("door_radius");

	auto radiusBody = PhysicsBody::createCircle(60.0f);
	radiusBody->setDynamic(false);
	radiusBody->setCategoryBitmask(4);
	radiusBody->setCollisionBitmask(1);
	radiusBody->setContactTestBitmask(0xFFFFFFFF);
	radiusBody->setTag(10000);
	useRadius->setPhysicsBody(radiusBody);

	this->addChild(useRadius);
}

void Door::use() {
	if (isOpen == false) {
		isOpen = true;
		this->getPhysicsBody()->setEnabled(false);
		this->setGlobalZOrder(2);
		this->setOpacity(100);
	}
	else {
		isOpen = false;
		this->getPhysicsBody()->setEnabled(true);
		this->setGlobalZOrder(5);
		this->setOpacity(225);
	}
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

//creates a room, made of 4 walls, can have doors and stairways
void Room::createRoom(vector<Door*> *doors, vector<Stair*> *stairs, vector<EnvObject*> *objects, vector<Item*> *items, vector<Enemy*> *enemies, Vec2 position, int width, int height, int door, vector<StairData> stairways)
{	//setting size of room
	this->setContentSize(Size(width, height));
	this->setAnchorPoint(Vec2(0, 0));

	//creating stairways
	if (stairways.size() > 0 && stairways[0].type != 0) {
		Stair* s;
		for (int i = 0; i < stairways.size(); i++) {
			s = Stair::create();
			s->initObject(stairways[i], position, Size(width, height));
			stairs->push_back(s);
		}
	}

	//creating walls
	Vec2 wallPos = position - Vec2(thick, thick);//the position for generating the walls of the room
	Wall* w = Wall::create();
	Door* d = Door::create();;

	//floor
	w->initObject(wallPos, Size(width + fullThick, thick));
	this->addChild(w);

	//ceiling
	w = Wall::create();
	w->initObject(wallPos + Vec2(0, height + thick), Size(width + fullThick, thick));
	this->addChild(w);

	if (door == 0) {//no doors
		//left wall
		w = Wall::create();
		w->initObject(wallPos + Vec2(0, thick), Size(thick, height));
		this->addChild(w);

		//right wall
		w = Wall::create();
		w->initObject(wallPos + Vec2(width + thick, thick), Size(thick, height));
		this->addChild(w);
	}
	else if (door == 1) {//door on right wall 
		//door
		d->initObject(position + Vec2(width, 0));
		doors->push_back(d);

		//left wall
		w = Wall::create();
		w->initObject(wallPos + Vec2(0, thick), Size(thick, height));
		this->addChild(w);

		//right wall
		w = Wall::create();
		w->initObject(wallPos + Vec2(width + thick, thick + d->doorSize.height), Size(thick, height - d->doorSize.height));
		this->addChild(w);
	}
	else if (door == 2) {//doors on both walls
		//door
		d->initObject(position + Vec2(width, 0));
		doors->push_back(d);

		//left wall
		w = Wall::create();
		w->initObject(wallPos + Vec2(0, thick + d->doorSize.height), Size(thick, height - d->doorSize.height));
		this->addChild(w);

		//right wall
		w = Wall::create();
		w->initObject(wallPos + Vec2(width + thick, thick + d->doorSize.height), Size(thick, height - d->doorSize.height));
		this->addChild(w);
	}
	else if (door == 3) {//door left wall
		//left wall
		w = Wall::create();
		w->initObject(wallPos + Vec2(0, thick + d->doorSize.height), Size(thick, height - d->doorSize.height));
		this->addChild(w);

		//right wall
		w = Wall::create();
		w->initObject(wallPos + Vec2(width + thick, thick), Size(thick, height));
		this->addChild(w);
	}
}
