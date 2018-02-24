#include "CocoLogo.h"
#include "GroupLogo.h"
#include "DisplayHandler.h"
#include "InputHandler.h"
#include <iostream>

using namespace std;

cocos2d::Scene* CocoLogo::createScene()
{
	//Create a scene without physics
	Scene* scene = Scene::create();

	//Create the main layer that is attached to the scene
	CocoLogo* layer = CocoLogo::create();

	//Add the layer to the scene
	scene->addChild(layer);

	//Return the newly built scene
	return scene;
}


// initialize the secene
bool CocoLogo::init()
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

	// create cocos logo
	sprite = Sprite::create("HelloWorld.png");
	
	// position the sprite on the center of the screen
	float x = origin.x + visibleSize.width / 2;
	float y = origin.y + visibleSize.height / 2;
	sprite->setPosition(Vec2(x, y));

	// add the sprite as a child to this layer
	this->addChild(sprite, 0);

	// switch to next scene
	this->schedule(schedule_selector(CocoLogo::Step), 3.0f);

	//Schedule the use of the update function so the function actually gets called
	this->scheduleUpdate();

	return true;
}


// update function for every frame
void CocoLogo::update(float deltaTime)
{
	// press space for immidiate switch to next scene
	if (INPUTS->getKeyPress(KeyCode::KEY_SPACE))
	{
		Scene *scene = GroupLogo::create();
		director->replaceScene(TransitionFade::create(0.0f, scene));
	}

	//update the keybord each frame
	INPUTS->clearForNextFrame();
}

// Help to switch to next scene
void CocoLogo::Step(float dt)
{
	// fade out this scene
	this->unschedule(schedule_selector(CocoLogo::Step));
	director->purgeCachedData();

	// run next scene
	director->replaceScene(CCTransitionFade::create(3.0f, GroupLogo::create()));
}

