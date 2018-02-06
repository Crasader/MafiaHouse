#include "StartScreen.h"
#include "DisplayHandler.h"
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

bool StartScreen::init()
{
	if (!Scene::init())
	{
		return false;
	}

	director = Director::getInstance();

	windowSize = DISPLAY->getWindowSizeAsVec2();


	sprite = Sprite::create("Start.png");

	auto visibleSize = director->getVisibleSize();
	Vec2 origin = director->getVisibleOrigin();

	float x = origin.x + visibleSize.width / 2;
	float y = origin.y + visibleSize.height / 2;

	sprite->setPosition(Vec2(x, y));

	this->addChild(sprite, 0);



	label = Label::createWithTTF("Press Space To Start", "fonts/Nexa_Bold.otf", 24);
	label->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 4));

	this->addChild(label, 1);


	/*this->schedule(schedule_selector(StartScreen::Step), 4.0f);*/

	this->scheduleUpdate();

	return true;
}

void StartScreen::Step(float dt)
{
	this->unschedule(schedule_selector(StartScreen::Step));
	CCDirector::sharedDirector()->purgeCachedData();
	CCScene *pScene = Scene::create();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.0f, pScene));
}

void StartScreen::update(float deltaTime)
{
	if (frameCount == 0)
	{
		label->runAction(FadeOut::create(1.0f));
	}
	frameCount++;
	if (frameCount == 50)
	{
		label->runAction(FadeIn::create(1.0f));
		frameCount = -50;
	}	
}