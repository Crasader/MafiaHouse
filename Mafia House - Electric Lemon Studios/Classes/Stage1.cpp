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

	//Walls
	auto wall = Wall::create();
	wall->initObject(Vec2(100,25), Size(25, 650));
	walls.push_back(wall);

	wall = Wall::create();
	wall->initObject(Vec2(1075, 25), Size(25, 650));
	walls.push_back(wall);

	wall = Wall::create();
	wall->initObject(Vec2(100, 0), Size(1000, 25));
	walls.push_back(wall);

	wall = Wall::create();
	wall->initObject(Vec2(100, 675), Size(1000, 25));
	walls.push_back(wall);

	for (int i = 0; i < walls.size(); i++) {
		mainLayer->addChild(walls[i], 1);
	}
	
	//Items
	auto item = Item::create();
	item->initObject(Vec2(400, 25));
	item->setFlippedX(true);
	items.push_back(item);
	mainLayer->addChild(items[0], 3);

	//Enemies
	auto guard = Enemy::create();
	guard->initObject(Vec2(500, 25));

	//guard moves automatically
	auto movement = MoveBy::create(5, Vec2(400, 0));
	auto turn = ScaleBy::create(0.0f,-1.0f, 1.0f);
	auto wait = MoveBy::create(0.5, Vec2(0, 0));
	auto moveback = MoveBy::create(5, Vec2(-400, 0));
	auto sequence = Sequence::create(movement, wait, turn, moveback, wait, turn, NULL);
	guard->runAction(RepeatForever::create(sequence));

	enemies.push_back(guard);
	mainLayer->addChild(enemies[0], 2);

	return true;
}
