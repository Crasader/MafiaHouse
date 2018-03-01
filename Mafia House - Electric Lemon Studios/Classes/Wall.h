#pragma once
#include "GameObject.h"
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

	const Size doorSize = Size(30, 175);
};

class RoomData {
public:
	RoomData() {}
	~RoomData() {}

	int width;//length of the room

	bool door;//is there a door on the right wall or not

	int stair;//whether the Stairway is A or B: 0 = No Stairway, 1 = A, 2 = B
	Vec2 stairPos;//posiiton of the stairway
};

class FloorData {
public:
	FloorData() {}
	~FloorData() {}

	int height;//height of the floor
	RoomData rooms;//rooms the floor contains
};

//made of walls
class Room : public Node
{
public:
	Room();
	~Room();

	static Room* Room::create();

	//door: 0 = no door, 1 = left wall door, 2 = right wall door
	//noside: used to tell if the room doesn't need a certain wall:
		//0 = make all walls, 1 = no left wall, 2 = left wall or ceiling
	void createRoom(Vec2 position, int length, int height, int thick = 25, int doorSide = 0, int noSide = 0);
};

void createFloor(vector<Room*> *rooms, Vec2 position, int noSide, vector<RoomData> roomData, int height = 500, int thick = 25);

void createBuilding(vector<Room*> *rooms, Vec2 position, vector<int> floorHeight, vector<vector<RoomData>> roomDatas, int thick = 25);

class Building
{
public:
	Building();
	~Building();

	vector<Room*> rooms;//contains all rooms in the level
};
