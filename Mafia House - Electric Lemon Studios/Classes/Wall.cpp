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

//Stair Class
Stair::Stair() {
	//sprite properties
	name = "stair";
	zOrder = 2;
	scale = 1.0f;
	//physics body properties
	tag = 40000;
	dynamic = false;
	category = 2;
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
	//use type to set tag of the stairway
	if (data.type == 2) {
		this->setTag(tag + data.pairNum + 1000);//will have tag of it's partner stairway plus 1000
	}
	else {
		this->setTag(tag + data.pairNum);
	}
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

//creates a room, made of 4 walls, can have doors and stairways
void Room::createRoom(Vec2 position, int width, int height, int door, vector<StairData> stairs)
{	//setting size of room
	this->setContentSize(Size(width, height));

	//creating stairways
	if (stairs.size() > 0 && stairs[0].type != 0) {
		Stair* s;
		for (int i = 0; i < stairs.size(); i++) {
			s = Stair::create();
			s->initObject(stairs[i], position, Size(width, height));
			this->addChild(s);
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
		this->addChild(d);

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
		this->addChild(d);

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

vector<Room*> createFloor(Vec2 position, vector<RoomData> roomData, int height)
{
	vector<Room*> rooms;
	Room* room;

	for (int i = 0; i < roomData.size(); i++) {
		room = Room::create();

		room->createRoom(position, roomData[i].width, height, roomData[i].door, roomData[i].stairs);

		position = position + Vec2(roomData[i].width + room->fullThick, 0);//adding length of created room to set position for next room

		rooms.push_back(room);
	}

	return rooms;
}

vector<Room*> createBuilding(Vec2 position, float levelWidth, vector<FloorData> floorData)
{
	Room r;
	vector<Room*> rooms;//stores all rooms in the level
	vector<Room*> floor;//used per floor

	//calculating width differences between floors and first floor
	int firstFloorWidth = 0;
	vector<int> floorOffsets;
	for (int i = 0; i < floorData.size(); i++) {
		int totalWidth = 0;
		for (int j = 0; j < floorData[i].rooms.size(); j++) {
			totalWidth += floorData[i].rooms[j].width;
		}
		if (i == 0) {
			firstFloorWidth = totalWidth;
		}
		floorOffsets.push_back((totalWidth - firstFloorWidth) / 2);
	}

	position = position + Vec2((levelWidth / 2) - (firstFloorWidth / 2), 0);//setting position to be in centre of background

	//generating floors
	for (int i = 0; i < floorData.size(); i++) {
		floor = createFloor(position - Vec2(floorOffsets[i], 0), floorData[i].rooms, floorData[i].height);

		position = position + Vec2(0, floorData[i].height + r.fullThick);//adding height of created floor to set position for next floor

		rooms.insert(std::end(rooms), std::begin(floor), std::end(floor));//adding floor's list of rooms to list of all rooms
	}

	return rooms;
}
