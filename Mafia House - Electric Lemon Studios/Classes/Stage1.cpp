#include "Stage1.h"

USING_NS_CC;

Scene* Stage1::createScene()
{
	auto scene = Scene::createWithPhysics();
	auto layer = Stage1::create();

	//scene->getPhysicsWorld()->setGravity(Vec2(0, -100));
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

	scene->addChild(layer);

	return scene;
}

bool Stage1::init()
{
	if (!Scene::initWithPhysics())
	{
		return false;
	}

	//set the background name before calling setup
	backgroundName = "backscroll.png";

	//initlializes stuff that is the same for all levels
	setup();

	//set player starting position
	player->setPosition(200, 25);
	//setting camera to player position
	camPos->setPosition(player->getPosition() + camOffset);

	//Rooms
	/*vector<vector<RoomData>> roomData;
	vector<int> floorHeights;

	int numFloors = 3;
	for (int i = 1; i <= numFloors; i++) {
		int height = 400;
		floorHeights.push_back(height);

		int numRooms = 4;
		vector<RoomData> dataSet;
		for (int j = 1; j <= numRooms; j++) {
			RoomData data;
			if (j != numRooms) {
				data.doorSide = 2;
			}
			else {
				data.doorSide = 0;
			}
			data.length = 800;
			dataSet.push_back(data);
		}
		roomData.push_back(dataSet);
	}
	createBuilding(&rooms, Vec2(0,0), floorHeights, roomData);
	for (int i = 0; i < rooms.size(); i++) {
		rooms[i]->setTag(rooms[i]->getTag() + i);//giving a unique tag to each room?
		mainLayer->addChild(rooms[i]);
	}*/
	vector<StairData> stair;
	auto room = Room::create();
	room->createRoom(background->getPosition() + Vec2(room->thick, room->thick), 1000, 500, 3, stair);
	mainLayer->addChild(room);

	//Env. Objects
	auto object = EnvObject::create();
	object->initObject(Vec2(300,25));
	objects.push_back(object);
	for (int i = 0; i < objects.size(); i++) {
		objects[i]->setTag(objects[i]->getTag() + i);//giving a unique tag to each object
		mainLayer->addChild(objects[i]);
	}
	
	//Items
	Item* knife = Knife::create();
	knife->initObject(Vec2(400, 25));
	items.push_back(knife);
	for (int i = 0; i < items.size(); i++) {
		items[i]->setTag(items[i]->getTag() + i);//giving a unique tag to each item
		mainLayer->addChild(items[i]);
	}

	//Enemies
	auto guard = Enemy::create();
	guard->initObject(Vec2(500, 25));

	//guard moves automatically, put this into Enemy class
	auto movement = MoveBy::create(5, Vec2(400, 0));
	auto turn = ScaleBy::create(0.0f,-1.0f, 1.0f);
	auto wait = MoveBy::create(0.5, Vec2(0, 0));
	auto moveback = MoveBy::create(5, Vec2(-400, 0));
	auto sequence = Sequence::create(movement, wait, turn, moveback, wait, turn, NULL);
	guard->runAction(RepeatForever::create(sequence));

	enemies.push_back(guard);
	for (int i = 0; i < enemies.size(); i++) {
		enemies[i]->setTag(enemies[i]->getTag() + i);//giving a unique tag to each enemy
		mainLayer->addChild(enemies[i]);
	}

	return true;
}
