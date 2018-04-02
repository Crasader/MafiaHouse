#include "Stages.h"

bool Tutorial1::init()
{
	if (!Scene::initWithPhysics()) { return false; }

	//initlializes stuff that is the same for all levels
	setup();
	//initialize background
	setBackground("backgrounds/back.png", 0.5f);
	//create level
	//initLevel("level_1_test.txt");
	initLevel("tutorial_1.txt");
	levelNum = 0;

	return true;
}

bool Stage1::init()
{
	if (!Scene::initWithPhysics()) { return false; }

	//initlializes stuff that is the same for all levels
	setup();
	//initialize background
	setBackground("backgrounds/back.png", 0.5f);
	//create level
	//initLevel("level_1_test.txt");
	initLevel("main_level.txt");
	levelNum = 1;

	return true;
} 

bool Stage2::init()
{
	if (!Scene::initWithPhysics()) { return false; }

	//initlializes stuff that is the same for all levels
	setup();
	//initialize background
	setBackground("backgrounds/back.png", 1.0f);
	//create level
	initLevel("level_1_test.txt");
	levelNum = 2;

	return true;
}

bool Stage3::init()
{
	if (!Scene::initWithPhysics()) { return false; }

	//initlializes stuff that is the same for all levels
	setup();
	//initialize background
	setBackground("backgrounds/back.png", 1.0f);
	//create level
	initLevel("stage2.txt");
	levelNum = 3;

	return true;
}

bool Stage4::init()
{
	if (!Scene::initWithPhysics()) { return false; }

	//initlializes stuff that is the same for all levels
	setup();
	//initialize background
	setBackground("backgrounds/back.png", 1.0f);
	//create level
	initLevel("stage2.txt");
	levelNum = 4;

	return true;
}

