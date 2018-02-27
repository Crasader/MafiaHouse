#pragma once
#include "GameObject.h"
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

	void initObject(Vec2 startPos = Vec2(0, 0));//position, size

	const Size doorSize = Size(25, 175);
};

//made of walls
class Room : public Node
{
public:
	Room();
	~Room();

	static Room* Room::create();

	void createRoom(Vec2 position, int length, int height, int thick = 25, int doorSide = 0, int noSide = 0);
	//door: 0 = no door, 1 = left wall door, 2 = right wall door
	//noside: used to tell if the room doesn't need a certain wall
	//0 = make all walls, 1 = no left wall, 2 = left wall or ceiling
};

class Building : public Node
{
public:
	Building();
	~Building();

	static Building* Building::create();

	void createBuilding(Vec2 position, int numFloors);

	std::vector<Room*> floor;
};