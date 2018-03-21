#include "Stages.h"

bool Stage1::init()
{
	if (!Scene::initWithPhysics()) { return false; }

	//initlializes stuff that is the same for all levels
	setup();
	//initialize background
	setBackground("backgrounds/back.png", 0.5f);
	//create level
	initLevel("test_level.txt");

	return true;
}

bool Stage2::init()
{
	if (!Scene::initWithPhysics()) { return false; }

	//initlializes stuff that is the same for all levels
	setup();
	//initialize background
	setBackground("background2.png", 1.0f);
	//create level
	initLevel("stage2.txt");

	return true;
}
