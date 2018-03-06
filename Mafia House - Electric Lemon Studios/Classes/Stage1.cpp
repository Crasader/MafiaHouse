#include "Stage1.h"

USING_NS_CC;

Scene* Stage1::createScene()
{
	auto scene = Scene::createWithPhysics();
	auto layer = Stage1::create();

	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

	scene->addChild(layer);
	layer->getDefaultCamera()->removeFromParentAndCleanup(true);

	return scene;
}

bool Stage1::init()
{
	if (!Scene::initWithPhysics())
	{
		return false;
	}

	//initlializes stuff that is the same for all levels
	setup();
	//initialize background
	setBackground("backscroll.png", 1.0f);
	//create level
	initLevel("test_level.txt");

	return true;
}
