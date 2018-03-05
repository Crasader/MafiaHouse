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

	void initObject(Vec2 startPos = Vec2(0, 0));

	void use();

	bool isOpen = false;

	const Size doorSize = Size(20, 110);
};

class RoomData {
public:
	RoomData() {}
	~RoomData() {}

	int width;//length of the room

	int door;//does the room have doors: 0 = no doors, 1 = right wall door, 2 = both wall doors, 3 = left wall door

	Vec2 room = Vec2(0, 0);
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

	void createRoom(vector<Door*> *doors, vector<Stair*> *stairs, vector<EnvObject*> *objects, vector<Item*> *items, vector<Enemy*> *enemies, Player* player, Vec2 position, int width, int height, int door, Vec2 room);
};
