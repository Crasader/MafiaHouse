#include "Wall.h"

Wall::Wall()
{
	//sprite properties
	zOrder = 5;
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
	Wall* wall;
	Door* door;
	if (noSide == 0) {
		//ceiling
		wall = Wall::create();
		wall->initObject(position + Vec2(0, thick + height), Size(length + (thick * 2), thick));
		this->addChild(wall);
		//floor
		wall = Wall::create();
		wall->initObject(position, Size(length + (thick * 2), thick));
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
			door = Door::create();
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
			door = Door::create();
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
	//room with no left side
	else if (noSide == 1) {
		//ceiling
		wall = Wall::create();
		wall->initObject(position + Vec2(thick, thick + height), Size(length + thick, thick));
		this->addChild(wall);
		//floor
		wall = Wall::create();
		wall->initObject(position + Vec2(thick, 0), Size(length + thick, thick));
		this->addChild(wall);
		if (doorSide == 0 || doorSide == 1) {
			//right wall
			wall = Wall::create();
			wall->initObject(position + Vec2(thick + length, thick), Size(thick, height));
			this->addChild(wall);
		}
		else if (doorSide == 2) {
			//door
			door = Door::create();
			door->initObject(position + Vec2(thick + length, thick));
			this->addChild(door);
			//right wall
			wall = Wall::create();
			wall->initObject(position + Vec2(thick + length, thick + door->doorSize.height), Size(thick, height - door->doorSize.height));
			this->addChild(wall);
		}
	}
	//room with no floor
	else if (noSide == 2) {
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
			door = Door::create();
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
			door = Door::create();
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
	//room with no left side or floor
	else if (noSide == 3) {
		//ceiling
		wall = Wall::create();
		wall->initObject(position + Vec2(thick, thick + height), Size(length + thick, thick));
		this->addChild(wall);
		if (doorSide == 0 || doorSide == 1) {
			//right wall
			wall = Wall::create();
			wall->initObject(position + Vec2(thick + length, thick), Size(thick, height));
			this->addChild(wall);
		}
		else if (doorSide == 2) {
			//door
			door = Door::create();
			door->initObject(position + Vec2(thick + length, thick));
			this->addChild(door);
			//right wall
			wall = Wall::create();
			wall->initObject(position + Vec2(thick + length, thick + door->doorSize.height), Size(thick, height - door->doorSize.height));
			this->addChild(wall);
		}
	}
}

void createFloor(vector<Room*> *rooms, Vec2 position, int noSide, vector<RoomData> roomData, int height, int thick) {

	Room* room;

	for (int i = 0; i < roomData.size(); i++) {
		room = Room::create();
		if (i == 0) {
			room->createRoom(position, roomData[i].length, height, thick, roomData[i].doorSide, noSide);
		}
		else {
			room->createRoom(position, roomData[i].length, height, thick, roomData[i].doorSide, noSide + 1);
		}
		position = position + Vec2(roomData[i].length + thick, 0);//adding length of created room to set position for next room

		rooms->push_back(room);
	}
}

void createBuilding(vector<Room*> *rooms, Vec2 position, vector<int> floorHeights, vector<vector<RoomData>> roomDatas, int thick){

	for (int i = 0; i < floorHeights.size(); i++) {
		if (i == 0) {
			createFloor(rooms, position, 0, roomDatas[i], floorHeights[i], thick);
		}
		else {
			createFloor(rooms, position, 2, roomDatas[i], floorHeights[i], thick);
		}
		position = position + Vec2(0, floorHeights[i] + thick);//adding height of created floor to set position for next floor
	}
}

//Building Class
Building::Building() {

}
Building::~Building() {

}