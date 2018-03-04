#pragma once
#include "GameObject.h"
#include "Enemy.h"
#include "Item.h"
#include "EnvObject.h"
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

	void initObject(Vec2 startPos = Vec2(0, 0));

	void use();

	bool isOpen = false;

	const Size doorSize = Size(20, 110);
};

class StairData {
public:
	StairData() {}
	~StairData() {}

	int pairNum = 0;//the number of the pair of stairways this stairway belongs to; ex. stairways 1A and 1B are both part of the number 1 pair
	int type = 0;//whether the Stairway is A or B: 0 = No Stairway, 1 = A, 2 = B
	Vec2 position = Vec2(0, 0);//position of the stairwayin the room
};

class Stair : public GameObject
{
public:
	Stair();
	~Stair();

	static Stair* create(const std::string& filename = "stair.png");

	void initObject(StairData data, Vec2 roomPos, Size roomSize);

	int type;

	const Size stairSize = Size(45, 105);
};

class RoomData {
public:
	RoomData() {}
	~RoomData() {}

	int width;//length of the room

	int door;//does the room have doors: 0 = no doors, 1 = right wall door, 2 = both wall doors, 3 = left wall door

	vector<StairData> stairs;//data for what stairways are in the room
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

	void createRoom(vector<Door*> *doors, vector<Stair*> *stairs, vector<EnvObject*> *objects, vector<Item*> *items, vector<Enemy*> *enemies, Vec2 position, int width, int height, int door, vector<StairData> stairways = vector<StairData>());
};
