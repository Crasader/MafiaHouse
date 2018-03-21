#pragma once
#include "GameObject.h"
#include "Enemy.h"
#include "Item.h"
#include "EnvObject.h"
#include "Player.h"
#include "Stair.h"
#include "Functions.h"
using std::vector;
class Wall : public GameObject
{
public:
	Wall();
	~Wall();
	CREATE_SPRITE_FUNC(Wall, "wall.png");
	CREATE_WITH_FRAME(Wall);
	CREATE_WITH_FRAME_NAME(Wall, "objects/wall.png");

	void initObject(Vec2 startPos = Vec2(0, 0), Size size = Size(25, 25));//position, size
};

class Room : public Node
{
public:
	Room();
	~Room();

	Sprite* background;

	CREATE_FUNC(Room);

	int fullThick = 20;//thickness of the walls for level generation
	int thick = fullThick / 2;//thickness of an individual wall

	void createWall(vector<Door*> *doors, int orientation, int type, Vec2 position, Size sizefloat, vector<DoorData> doorData = vector<DoorData>(), string name = "floor");

	void createRoom(vector<Door*> *doors, vector<Stair*> *stairs, vector<HideObject*> *hideObjects, vector<Item*> *items, vector<Enemy*> *enemies, vector<PathNode*> *pathNodes, Player* player, Vec2 position, RoomData roomData, int height);
};
