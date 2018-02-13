#include "MenuScreen.h"
#include "DisplayHandler.h"
#include "InputHandler.h"
#include <iostream>

using namespace std;

cocos2d::Scene* MenuScreen::createScene()
{
	//Create a scene without physics
	Scene* scene = Scene::create();

	//Create the main layer that is attached to the scene
	MenuScreen* layer = MenuScreen::create();

	//Add the layer to the scene
	scene->addChild(layer);

	//Return the newly built scene
	return scene;
}

bool MenuScreen::init()
{
	if (!Scene::init())
	{
		return false;
	}

	director = Director::getInstance();

	windowSize = DISPLAY->getWindowSizeAsVec2();

	auto visibleSize = director->getVisibleSize();
	Vec2 origin = director->getVisibleOrigin();


	//Initialize a Sprite
	sprite = Sprite::create("MainMenu.png");

	float x = origin.x + visibleSize.width / 2;
	float y = origin.y + visibleSize.height / 2;

	sprite->setPosition(Vec2(x, y));

	this->addChild(sprite, 0);



	this->scheduleUpdate();

	return true;
}

void MenuScreen::update(float deltaTime)
{
	gameTime += deltaTime;

	//put update here
	if (INPUTS->getKeyPress(KeyCode::KEY_SPACE))
	{
		//Example for press space key
	}
}