#include "Wall.h"

Wall::Wall()
{
	name = "wall";
	tag = 0;
	//sprite properties
	zOrder = 6;
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

//Door Class
Door::Door() {
	name = "door";
	tag = 50000;
	//sprite properties
	zOrder = 6;
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
	this->setContentSize(size);//set the size of the wall
	GameObject::initObject(startPos);

	auto useRadius = Node::create();
	useRadius->setPositionNormalized(Vec2(0.5, 0.5));
	useRadius->setName("door_radius");

	auto radiusBody = PhysicsBody::createBox(useBox);
	radiusBody->setDynamic(false);
	radiusBody->setCategoryBitmask(4);
	radiusBody->setCollisionBitmask(1);
	radiusBody->setContactTestBitmask(0xFFFFFFFF);
	radiusBody->setTag(10000);
	useRadius->setPhysicsBody(radiusBody);

	this->addChild(useRadius);
}

void Door::initObject(int orient, Vec2 startPos) {
	if (orient == 2) {//horizontal
		size = Size(110, 20);
		useBox = Size(110, radius);
	}
	else if (orient == 1) {//vertical
		size = Size(20, 110);
		useBox = Size(radius, 110);
	}
	Door::initObject(startPos);
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
		this->setOpacity(255);
	}
}

//Vent Class
Vent::Vent() {
}
Vent::~Vent() {

}

Vent* Vent::create(const std::string& filename)
{
	Vent *sprite = new (std::nothrow) Vent();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

void Vent::initObject(int orient, Vec2 startPos) {
	radius = 46.0f;
	name = "vent";
	tag = 60000;
	//sprite properties
	zOrder = 6;
	scale = 1.0f;
	//physics body properties
	dynamic = false;
	category = 2;
	collision = 0xFFFFFFFF;
	if (orient == 2) {//horizontal
		size = Size(50, 20);
		useBox = Size(55 + radius / 2, radius);
	}
	else if (orient == 1) {//vertical
		size = Size(20, 50);
		useBox = Size(radius, 55 + radius / 2);
	}
	Door::initObject(startPos);
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

bool sortByPosition(DoorData a, DoorData b) {return a.pos < b.pos;}

void Room::createWall(vector<Door*> *doors, int orientation, int type, Vec2 position, Size size, vector<DoorData> doorData)
{
	Wall* w;
	if (doorData.size() > 0) {//has doors or vents

		int loops = (doorData.size() * 2);
		std::sort(doorData.begin(), doorData.end(), sortByPosition);//sorting doors in order from least to greatest

		//used purely for getting door and vent's proper size based on their orientation
		Door* door = Door::create();
		door->initObject(orientation);
		Vent* vent = Vent::create();
		vent->initObject(orientation);
	
		Door* d;
		float length;
		Vec2 newPos = position;
		int j = 0;
		if (orientation == 1) {//vertical
			for (int i = 0; i < loops; i++) {
				if (i % 2 == 0) {//on even number iterations, make a wall segment
					//length = (size.height * doorData[j].pos) - newPos.y + position.y;
					length =  doorData[j].pos - newPos.y + position.y;
					w = Wall::create();
					w->initObject(newPos, Size(size.width, length));
					if (length > 0) {
						this->addChild(w);
					}
				}
				else {//on odd number iterations, make a door or vent
					if (doorData[j].type == 1) {
						d = Door::create();
						d->initObject(orientation, newPos);
					}
					else if (doorData[j].type == 2) {
						d = Vent::create();
						d->initObject(orientation, newPos);
					}
					length = d->size.height;
					if (type != 2) {//only add doors or vents if right wall or ceiling
						doors->push_back(d);
					}
					j++;
				}
				newPos = newPos + Vec2(0, length);//setting position for next chunk of the wall
			}
			length = size.height - newPos.y + position.y;
			w = Wall::create();
			w->initObject(newPos, Size(size.width, length));
			this->addChild(w);
		}
		else if (orientation == 2) {//horizontal
			for (int i = 0; i < loops; i++) {
				if (i % 2 == 0) {//on even number iterations, make a wall segment
					//length = (size.width * doorData[j].pos) - newPos.x + position.x;
					length = doorData[j].pos - newPos.x + position.x;
					w = Wall::create();
					w->initObject(newPos, Size(length, size.height));
					if (length > 0) {
						this->addChild(w);
					}
				}
				else {//on odd number iterations, make a door or vent
					if (doorData[j].type == 1) {
						d = Door::create();
						d->initObject(orientation, newPos);
					}
					else if (doorData[j].type == 2) {
						d = Vent::create();
						d->initObject(orientation, newPos);
					}
					length = d->size.width;
					if (type != 2) {//only add doors or vents if right wall or ceiling
						doors->push_back(d);
					}
					j++;
				}
				newPos = newPos + Vec2(length, 0);//setting position for next chunk of the wall
			}
			length = size.width - newPos.x + position.x;
			w = Wall::create();
			w->initObject(newPos, Size(length, size.height));
			this->addChild(w);
		}
	}
	else {//no doors or vents
		w = Wall::create();
		w->initObject(position, size);
		this->addChild(w);
	}
}

//creates a room, made of 4 walls, can have doors/vents and stairways
void Room::createRoom(vector<Door*> *doors, vector<Stair*> *stairs, vector<EnvObject*> *objects, vector<Item*> *items, vector<Enemy*> *enemies, Player* player, Vec2 position, RoomData roomData, int height)
{	//setting size of room
	this->setContentSize(Size(roomData.width, height));
	this->setAnchorPoint(Vec2(0, 0));

	//setting player position
	if (player->startRoom == roomData.room) {
		player->setRoomPositionNormalized(position, Size(roomData.width, height), player->roomStartPos);
	}
	//setting stairway positions
	if (stairs->size() > 0) {
		for (int i = 0; i < stairs->size(); i++) {
			if ((*stairs)[i]->startRoom == roomData.room) {
				(*stairs)[i]->setRoomPositionNormalized(position, Size(roomData.width, height), (*stairs)[i]->roomStartPos);
			}
		}
	}
	//setting object positions
	if (objects->size() > 0) {
		for (int i = 0; i < objects->size(); i++) {
			if ((*objects)[i]->startRoom == roomData.room) {
				(*objects)[i]->setRoomPositionNormalized(position, Size(roomData.width, height), (*objects)[i]->roomStartPos);
			}
		}
	}
	//setting item positions
	if (items->size() > 0) {
		for (int i = 0; i < items->size(); i++) {
			if ((*items)[i]->startRoom == roomData.room) {
				(*items)[i]->setRoomPositionNormalized(position, Size(roomData.width, height), (*items)[i]->roomStartPos);
			}
		}
	}
	//setting enemy positions
	if (enemies->size() > 0) {
		for (int i = 0; i < enemies->size(); i++) {
			if ((*enemies)[i]->startRoom == roomData.room) {
				(*enemies)[i]->setRoomPositionNormalized(position, Size(roomData.width, height), (*enemies)[i]->roomStartPos);
			}
		}
	}

	//creating walls
	Vec2 wallPos = position - Vec2(thick, thick);//the position for generating the walls of the room

	//floor
	createWall(doors, 2, 2, wallPos, Size(roomData.width + fullThick, thick), roomData.bottomDoors);

	//ceiling
	createWall(doors, 2, 1, wallPos + Vec2(0, height + thick), Size(roomData.width + fullThick, thick), roomData.ceilingDoors);

	//left wall
	createWall(doors, 1, 2, wallPos + Vec2(0, thick), Size(thick, height), roomData.leftDoors);

	//right wall
	createWall(doors, 1, 1, wallPos + Vec2(roomData.width + thick, thick), Size(thick, height), roomData.rightDoors);
}
