#include "StartScreen.h"
#include "MenuScreen.h"
#include "DisplayHandler.h"
#include "InputHandler.h"
#include <iostream>

using namespace std;

cocos2d::Scene* StartScreen::createScene()
{
	//Create a scene without physics (we're implementing our own!)
	Scene* scene = Scene::create();

	//Create the main layer that is attached to the scene
	StartScreen* layer = StartScreen::create();

	//Add the layer to the scene
	scene->addChild(layer);

	//Return the newly built scene
	return scene;
}

// initialize the secene
bool StartScreen::init()
{
	// super init first
	if (!Scene::init())
	{
		return false;
	}

	// Init
	director = Director::getInstance();

	// get visible windows sizw and the origin position of gamw world
	auto visibleSize = director->getVisibleSize();
	Vec2 origin = director->getVisibleOrigin();

	// create game logo
	sprite = Sprite::create("Start.png");

	// position the sprite on the center of the screen
	float x = origin.x + visibleSize.width / 2;
	float y = origin.y + visibleSize.height / 2;
	sprite->setPosition(Vec2(x, y));

	// add the sprite as a child to this layer
	this->addChild(sprite, 0);

	// create a label for start game
	label = Label::createWithTTF("Press Space To Start", "fonts/Nexa_Bold.otf", 24);

	// set the the position of label
	label->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 4));

	// add the label as a child to this layer
	this->addChild(label, 1);

	//Schedule the use of the update function so the function actually gets called
	this->scheduleUpdate();

	return true;
}

// update function for every frame
void StartScreen::update(float deltaTime)
{
	// let lable fade out
	if (frameCount == 0)
	{
		label->runAction(FadeOut::create(1.0f));
	}

	// count for the frame, every 50 frames will fade in or fade out
	frameCount++;

	// let lable fade in
	if (frameCount == 50)
	{
		label->runAction(FadeIn::create(1.0f));
		frameCount = -50;
	}	

	// press space key for go to next frame
	if (INPUTS->getKeyPress(KeyCode::KEY_SPACE))
	{
		this->schedule(schedule_selector(StartScreen::Step), 0.0f);
	}
}


// Help to switch to next scene
void StartScreen::Step(float dt)
{
	// fade out this scene
	this->unschedule(schedule_selector(StartScreen::Step));
	director->purgeCachedData();

	// run next scene
	director->replaceScene(TransitionFade::create(2.0f, MenuScreen::create()));
}