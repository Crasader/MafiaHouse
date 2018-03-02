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
	player->setPosition(800, 25);
	//setting camera to player position
	camPos->setPosition(player->getPosition() + camOffset);

	//Rooms
	//this will all be done automatically taking the data from a text file!!!
	vector<FloorData> floorData;
	FloorData floor;
	RoomData roomData;
	StairData stair;
	//first floor:
	floor.height = 400;
	roomData.width = 1000;
	roomData.door = 1;
	stair.pairNum = 1;
	stair.type = 1;
	stair.position = Vec2(0.2, 0.2);
	roomData.stairs.push_back(stair);
	floor.rooms.push_back(roomData);
	roomData.stairs.clear();//clearing stair data

	roomData.width = 800;
	roomData.door = 3;
	stair.pairNum = 2;
	stair.type = 1;
	stair.position = Vec2(0.7, 0.7);
	roomData.stairs.push_back(stair);
	floor.rooms.push_back(roomData);
	roomData.stairs.clear();//clearing stair data

	roomData.width = 300;
	roomData.door = 0;
	stair.pairNum = 3;
	stair.type = 1;
	stair.position = Vec2(0.5, 0.5);
	roomData.stairs.push_back(stair);
	floor.rooms.push_back(roomData);
	floorData.push_back(floor);
	roomData.stairs.clear();//clearing stair data
	floor.rooms.clear();//clearing room data

	//next floor:
	floor.height = 600;
	roomData.width = 600;
	roomData.door = 1;
	stair.type = 0;
	roomData.stairs.push_back(stair);
	floor.rooms.push_back(roomData);
	roomData.stairs.clear();//clearing stair data

	roomData.width = 1500;
	roomData.door = 2;
	stair.pairNum = 4;
	stair.type = 1;
	stair.position = Vec2(0.3, 0.3);
	roomData.stairs.push_back(stair);
	stair.pairNum = 1;
	stair.type = 2;
	stair.position = Vec2(0.9, 0.9);
	roomData.stairs.push_back(stair);
	floor.rooms.push_back(roomData);
	roomData.stairs.clear();//clearing stair data

	roomData.width = 400;
	roomData.door = 3;
	stair.pairNum = 2;
	stair.type = 2;
	stair.position = Vec2(0.5, 0.5);
	roomData.stairs.push_back(stair);
	floor.rooms.push_back(roomData);
	floorData.push_back(floor);
	roomData.stairs.clear();//clearing stair data
	floor.rooms.clear();//clearing room data

	//next floor:
	floor.height = 500;
	roomData.width = 400;
	roomData.door = 1;
	stair.pairNum = 4;
	stair.type = 2;
	stair.position = Vec2(0.8, 0.8);
	roomData.stairs.push_back(stair);
	floor.rooms.push_back(roomData);
	roomData.stairs.clear();//clearing stair data

	roomData.width = 200;
	roomData.door = 2;
	stair.type = 0;
	roomData.stairs.push_back(stair);
	floor.rooms.push_back(roomData);
	roomData.stairs.clear();//clearing stair data

	roomData.width = 700;
	roomData.door = 3;
	stair.pairNum = 3;
	stair.type = 2;
	stair.position = Vec2(0.4, 0.4);
	roomData.stairs.push_back(stair);
	floor.rooms.push_back(roomData);
	floorData.push_back(floor);

	rooms = createBuilding(background->getPosition(), background->getContentSize().width, floorData);

	for (int i = 0; i < rooms.size(); i++) {
		rooms[i]->setTag(rooms[i]->getTag() + i);//giving a unique tag to each room
		mainLayer->addChild(rooms[i]);
	}

	//Env. Objects
	auto object = EnvObject::create();
	object->initObject(Vec2(1000,25));
	objects.push_back(object);
	for (int i = 0; i < objects.size(); i++) {
		objects[i]->setTag(objects[i]->getTag() + i);//giving a unique tag to each object
		mainLayer->addChild(objects[i]);
	}
	
	//Items
	Item* knife = Knife::create();
	knife->initObject(Vec2(900, 25));
	items.push_back(knife);
	for (int i = 0; i < items.size(); i++) {
		items[i]->setTag(items[i]->getTag() + i);//giving a unique tag to each item
		mainLayer->addChild(items[i]);
	}

	//Enemies
	auto guard = Enemy::create();
	guard->initObject(Vec2(1300, 25));

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
