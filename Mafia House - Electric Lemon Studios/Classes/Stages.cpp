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
	//initLevel("tutorial_1.txt");
	initLevel("levels/tutorial_1.txt");
	levelNum = 0;

	auto buttonDisplay = Sprite::createWithSpriteFrameName("icons/walkButtons.png");
	buttonDisplay->setPosition(Vec2(-2105, 125));
	mainLayer->addChild(buttonDisplay);
	buttonDisplay = Sprite::createWithSpriteFrameName("icons/pickupButton.png");
	buttonDisplay->setPosition(Vec2(-1955, 100));
	mainLayer->addChild(buttonDisplay);
	buttonDisplay = Sprite::createWithSpriteFrameName("icons/useButton.png");
	buttonDisplay->setPosition(Vec2(-1755, 110));
	mainLayer->addChild(buttonDisplay);
	buttonDisplay = Sprite::createWithSpriteFrameName("icons/interactButton.png");
	buttonDisplay->setPosition(Vec2(-1250, 100));
	mainLayer->addChild(buttonDisplay);
	buttonDisplay = Sprite::createWithSpriteFrameName("icons/upDownButtons.png");
	buttonDisplay->setPosition(Vec2(-930, 80));
	mainLayer->addChild(buttonDisplay);
	buttonDisplay = Sprite::createWithSpriteFrameName("icons/interactButton.png");
	buttonDisplay->setPosition(Vec2(-820, 100));
	mainLayer->addChild(buttonDisplay);
	buttonDisplay = Sprite::createWithSpriteFrameName("icons/crouchPickupButtons.png");
	buttonDisplay->setPosition(Vec2(-690, 120));
	mainLayer->addChild(buttonDisplay);
	buttonDisplay = Sprite::createWithSpriteFrameName("icons/useButton.png");
	buttonDisplay->setPosition(Vec2(-600, 110));
	mainLayer->addChild(buttonDisplay);
	

	return true;
}

bool Tutorial2::init()
{
	if (!Scene::initWithPhysics()) { return false; }

	//initlializes stuff that is the same for all levels
	setup();
	//initialize background
	setBackground("backgrounds/back.png", 0.5f);
	//create level
	//initLevel("level_1_test.txt");
	//initLevel("tutorial_1.txt");
	initLevel("levels/tutorial_2.txt");
	levelNum = 0;

	auto buttonDisplay = Sprite::createWithSpriteFrameName("icons/crouchButton.png");
	buttonDisplay->setPosition(Vec2(-1670, 110));
	mainLayer->addChild(buttonDisplay);
	buttonDisplay = Sprite::createWithSpriteFrameName("icons/interactButton.png");
	buttonDisplay->setPosition(Vec2(-1325, 120));
	mainLayer->addChild(buttonDisplay);
	buttonDisplay = Sprite::createWithSpriteFrameName("icons/interactButton.png");
	buttonDisplay->setPosition(Vec2(-840, 240));
	mainLayer->addChild(buttonDisplay);
	buttonDisplay = Sprite::createWithSpriteFrameName("icons/throwButton.png");
	buttonDisplay->setScale(1.25);
	buttonDisplay->setPosition(Vec2(-770, 145));
	mainLayer->addChild(buttonDisplay);

	return true;
}

bool Tutorial3::init()
{
	if (!Scene::initWithPhysics()) { return false; }

	//initlializes stuff that is the same for all levels
	setup();
	//initialize background
	setBackground("backgrounds/back.png", 0.5f);
	//create level
	//initLevel("level_1_test.txt");
	//initLevel("tutorial_1.txt");
	initLevel("levels/tutorial_3.txt");
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
	initLevel("test_level.txt");
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

