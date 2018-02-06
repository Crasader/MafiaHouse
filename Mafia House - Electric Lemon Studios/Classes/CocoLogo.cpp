#include "CocoLogo.h"
#include "GroupLogo.h"
#include "DisplayHandler.h"
#include <iostream>

using namespace std;

cocos2d::Scene* CocoLogo::createScene()
{
	//Create a scene without physics (we're implementing our own!)
	Scene* scene = Scene::create();

	//Create the main layer that is attached to the scene
	CocoLogo* layer = CocoLogo::create();

	//Add the layer to the scene
	scene->addChild(layer);

	//Return the newly built scene
	return scene;
}

bool CocoLogo::init()
{
	if (!Scene::init())
	{
		return false;
	}

	director = Director::getInstance();

	windowSize = DISPLAY->getWindowSizeAsVec2();


	sprite = Sprite::create("HelloWorld.png");
		
	auto visibleSize = director->getVisibleSize();
	Vec2 origin = director->getVisibleOrigin();

	float x = origin.x + visibleSize.width / 2;
	float y = origin.y + visibleSize.height / 2;
	
	sprite->setPosition(Vec2(x, y));

	this->addChild(sprite, 0);

	this->schedule(schedule_selector(CocoLogo::Step), 3.0f);

	this->scheduleUpdate();

	return true;
}

void CocoLogo::Step(float dt)
{
	this->unschedule(schedule_selector(CocoLogo::Step));
	CCDirector::sharedDirector()->purgeCachedData();
	CCScene *pScene = GroupLogo::create();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(3.0f, pScene));
}

