#pragma once
#include "GameObject.h"
#include "Enemy.h"
#include "Item.h"
#include "EnvObject.h"
#include "Player.h"
#include "Stair.h"
using std::vector;
class Wall : public GameObject
{
public:
	Wall();
	~Wall();

	static Wall* create(const std::string& filename = "wall.png");

	void initObject(Vec2 startPos = Vec2(0, 0), Size size = Size(25, 25));//position, size
};

class Door : public GameObject
{
public:
	Door();
	~Door();

	static Door* create(const std::string& filename = "door.png");

	virtual void initObject(Vec2 startPos = Vec2(0, 0));

	virtual void initObject(int orient, Vec2 startPos = Vec2(0, 0));

	void use();

	bool isOpen = false;

	float radius = 50.0f;

	Size size = Size(20, 110);
};

class Vent : public Door
{
public:
	Vent();
	~Vent();

	static Vent* create(const std::string& filename = "door.png");

	void initObject(int orient, Vec2 startPos = Vec2(0, 0));
};

class DoorData {
public:
	DoorData() {}
	~DoorData() {}

	float pos = 0.0f;

	int type = 1;//1 = door, 2 = vent
};

class RoomData {
public:
	RoomData() {}
	~RoomData() {}

	int width;//length of the room

	vector<DoorData> rightDoors;//the locations of the vents and doors on the right wall
	vector<DoorData> leftDoors;//the locations of the vents and doors on the left wall
	vector<DoorData> ceilingDoors;//the locations of the vents and doors on the ceiling
	vector<DoorData> bottomDoors;//the locations of the vents and doors on the ground

	Vec2 room = Vec2(0, 0);//the room's coordinates
};

class FloorData {
public:
	FloorData() {}
	~FloorData() {}

	int height;//height of the floor
	vector<RoomData> rooms;//rooms the floor contains
};

class Room : public Node
{
public:
	Room();
	~Room();

	static Room* Room::create();

	int fullThick = 20;//thickness of the walls for level generation
	int thick = fullThick / 2;//thickness of an individual wall

	void createWall(vector<Door*> *doors, int orientation, int type, Vec2 position, Size sizefloat, vector<DoorData> doorData = vector<DoorData>());

	void createRoom(vector<Door*> *doors, vector<Stair*> *stairs, vector<EnvObject*> *objects, vector<Item*> *items, vector<Enemy*> *enemies, Player* player, Vec2 position, RoomData roomData, int height);
};
