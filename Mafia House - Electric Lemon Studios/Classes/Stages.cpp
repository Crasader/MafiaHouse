#include "Stages.h"

bool Tutorial1::init()
{
	if (!Scene::initWithPhysics()) { return false; }

	// start playing background music 
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("Audio/beastly.wav");

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
	buttonDisplay->setPosition(Vec2(-2115, 125));
	mainLayer->addChild(buttonDisplay);
	buttonDisplay = Sprite::createWithSpriteFrameName("icons/pickupButton.png");
	buttonDisplay->setPosition(Vec2(-1955, 100));
	mainLayer->addChild(buttonDisplay);
	buttonDisplay = Sprite::createWithSpriteFrameName("icons/useButton.png");
	buttonDisplay->setPosition(Vec2(-1755, 110));
	mainLayer->addChild(buttonDisplay);
	buttonDisplay = Sprite::createWithSpriteFrameName("icons/interactButton.png");
	buttonDisplay->setPosition(Vec2(-1260, 80));
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
void Tutorial1::startNextLevel() {
	auto nextLevel = Tutorial2::createScene();
	director->replaceScene(nextLevel);
}

bool Tutorial2::init()
{
	if (!Scene::initWithPhysics()) { return false; }

	// start playing background music 
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("Audio/beastly.wav");

	//initlializes stuff that is the same for all levels
	setup();
	//initialize background
	setBackground("backgrounds/back.png", 0.5f);
	//create level
	//initLevel("level_1_test.txt");
	//initLevel("tutorial_1.txt");
	initLevel("levels/tutorial_2.txt");
	levelNum = 1;

	auto buttonDisplay = Sprite::createWithSpriteFrameName("icons/crouchButton.png");
	buttonDisplay->setPosition(Vec2(-1670, 110));
	mainLayer->addChild(buttonDisplay);
	buttonDisplay = Sprite::createWithSpriteFrameName("icons/interactButton.png");
	buttonDisplay->setPosition(Vec2(-1325, 120));
	mainLayer->addChild(buttonDisplay);
	buttonDisplay = Sprite::createWithSpriteFrameName("icons/useButton.png");
	buttonDisplay->setPosition(Vec2(-1250, 330));
	mainLayer->addChild(buttonDisplay);
	buttonDisplay = Sprite::createWithSpriteFrameName("icons/interactButton.png");
	buttonDisplay->setPosition(Vec2(-840, 240));
	mainLayer->addChild(buttonDisplay);
	buttonDisplay = Sprite::createWithSpriteFrameName("icons/throwButton.png");
	buttonDisplay->setGlobalZOrder(7);
	buttonDisplay->setScale(1.25);
	buttonDisplay->setPosition(Vec2(-770, 145));
	mainLayer->addChild(buttonDisplay);

	return true;
}
void Tutorial2::startNextLevel() {
	auto nextLevel = Tutorial3::createScene();
	director->replaceScene(nextLevel);
}

bool Tutorial3::init()
{
	if (!Scene::initWithPhysics()) { return false; }

	// start playing background music 
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("Audio/beastly.wav");

	//initlializes stuff that is the same for all levels
	setup();
	//initialize background
	setBackground("backgrounds/back.png", 0.5f);
	//create level
	//initLevel("level_1_test.txt");
	//initLevel("tutorial_1.txt");
	initLevel("levels/tutorial_3.txt");
	levelNum = 2;

	for (int i = 0; i < enemies.size(); i++) {
		enemies[i]->turnTime = 2.0f;
		enemies[i]->visionRadius = 170;
	}

	auto buttonDisplay = Sprite::createWithSpriteFrameName("icons/climbButton.png");
	buttonDisplay->setGlobalZOrder(7);
	buttonDisplay->setPosition(Vec2(-1620, 35));
	mainLayer->addChild(buttonDisplay);
	buttonDisplay = Sprite::createWithSpriteFrameName("icons/jumpClimbButtons.png");
	buttonDisplay->setPosition(Vec2(-1660, 130));
	mainLayer->addChild(buttonDisplay);
	//buttonDisplay = Sprite::createWithSpriteFrameName("icons/jumpClimbButtons.png");
	//buttonDisplay->setPosition(Vec2(-1695, 235));
	//mainLayer->addChild(buttonDisplay);
	buttonDisplay = Sprite::createWithSpriteFrameName("icons/interactButton.png");
	buttonDisplay->setGlobalZOrder(7);
	buttonDisplay->setPosition(Vec2(-1620, 280));
	mainLayer->addChild(buttonDisplay);
	buttonDisplay = Sprite::createWithSpriteFrameName("icons/rollButtons.png");
	buttonDisplay->setGlobalZOrder(7);
	buttonDisplay->setPosition(Vec2(-1540, 400));
	mainLayer->addChild(buttonDisplay);
	buttonDisplay = Sprite::createWithSpriteFrameName("icons/pickupButton.png");
	buttonDisplay->setPosition(Vec2(-920, 360));
	mainLayer->addChild(buttonDisplay);
	buttonDisplay = Sprite::createWithSpriteFrameName("icons/crouchAttackButtons.png");
	buttonDisplay->setPosition(Vec2(-1000, 130));
	mainLayer->addChild(buttonDisplay);


	return true;
}
void Tutorial3::startNextLevel() {
	auto nextLevel = Stage1::createScene();
	director->replaceScene(nextLevel);
}

bool Stage1::init()
{
	if (!Scene::initWithPhysics()) { return false; }

	// start playing background music 
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("Audio/rainy.wav");

	//initlializes stuff that is the same for all levels
	setup();
	//initialize background
	setBackground("backgrounds/back.png", 0.5f);
	//create level
	//initLevel("level_1_test.txt");
	initLevel("levels/stage_1.txt");
	levelNum = 3;
	
	return true;
}
void Stage1::startNextLevel() {
	auto nextLevel = Stage2::createScene();
	director->replaceScene(nextLevel);
}

bool Stage2::init()
{
	if (!Scene::initWithPhysics()) { return false; }

	// start playing background music 
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("Audio/rainy.wav");

	//initlializes stuff that is the same for all levels
	setup();
	//initialize background
	setBackground("backgrounds/back.png", 1.0f);
	//create level
	initLevel("levels/stage_3.txt");
	levelNum = 4;
	return true;
}
void Stage2::startNextLevel() {
	auto nextLevel = Stage3::createScene();
	director->replaceScene(nextLevel);
}

bool Stage3::init()
{
	if (!Scene::initWithPhysics()) { return false; }

	// start playing background music 
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("Audio/rainy.wav");

	//initlializes stuff that is the same for all levels
	setup();
	//initialize background
	setBackground("backgrounds/back.png", 1.0f);
	//create level
	initLevel("levels/stage_4.txt");
	levelNum = 5;
	return true;
}
void Stage3::startNextLevel() {
	auto nextLevel = Stage4::createScene();
	director->replaceScene(nextLevel);
}

bool Stage4::init()
{
	if (!Scene::initWithPhysics()) { return false; }

	// start playing background music 
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("Audio/rainy.wav");

	//initlializes stuff that is the same for all levels
	setup();
	//initialize background
	setBackground("backgrounds/back.png", 1.0f);
	//create level
	initLevel("level_1_test.txt");
	levelNum = 6;
	return true;
}
void Stage4::startNextLevel() {
	auto nextLevel = LevelSelectMenu::createScene();
	director->replaceScene(nextLevel);
}
