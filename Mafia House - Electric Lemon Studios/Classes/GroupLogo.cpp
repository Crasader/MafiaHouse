#include "GroupLogo.h"
#include "StartScreen.h"
#include "DisplayHandler.h"
#include <iostream>

using namespace std;

cocos2d::Scene* GroupLogo::createScene()
{
	//Create a scene without physics (we're implementing our own!)
	Scene* scene = Scene::create();

	//Create the main layer that is attached to the scene
	GroupLogo* layer = GroupLogo::create();

	//Add the layer to the scene
	scene->addChild(layer);

	//Return the newly built scene
	return scene;
}

bool GroupLogo::init()
{
	if (!Scene::init())
	{
		return false;
	}

	director = Director::getInstance();

	windowSize = DISPLAY->getWindowSizeAsVec2();


	sprite = Sprite::create("Electric Lemon.png");

	auto visibleSize = director->getVisibleSize();
	Vec2 origin = director->getVisibleOrigin();

	float x = origin.x + visibleSize.width / 2;
	float y = origin.y + visibleSize.height / 2;

	sprite->setPosition(Vec2(x, y));

	this->addChild(sprite, 0);

	this->schedule(schedule_selector(GroupLogo::Step), 4.0f);

	this->scheduleUpdate();

	return true;
}

void GroupLogo::Step(float dt)
{
	this->unschedule(schedule_selector(GroupLogo::Step));
	CCDirector::sharedDirector()->purgeCachedData();
	CCScene *pScene = StartScreen::create();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.0f, pScene));
}