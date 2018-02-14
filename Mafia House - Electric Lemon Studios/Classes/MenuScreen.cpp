#include "SimpleAudioEngine.h"	//to play music when this scene starts
#include "MenuScreen.h"
#include "DisplayHandler.h"
#include "InputHandler.h"
#include "HelloWorldScene.h"
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
	////Setup everything//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//start playing music 
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic("Audio/menu.wav");
	audio->playBackgroundMusic("Audio/menu.wav");

	//can initialize(preload) sound effects here 
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/boom.wav");





	//Setup the background
	background = Sprite::create("MainMenu.png");

	background->setScale(visibleSize.width / background->getContentSize().width, visibleSize.height / background->getContentSize().height);

	float x = origin.x + visibleSize.width / 2;
	float y = origin.y + visibleSize.height / 2;

	background->setPosition(Vec2(x, y));

	this->addChild(background, 0);

	//Setup the Start Game Sign

	startGame = Sprite::create("StartGame.png");

	startGame->setScale(visibleSize.width / background->getContentSize().width, visibleSize.height / background->getContentSize().height);

	x = origin.x + visibleSize.width / 2;
	y = origin.y + visibleSize.height * (2.6 / 6);

	startGame->setPosition(Vec2(x, y));

	this->addChild(startGame, 1);

	//Setup the Select Level Sign

	selectLevel = Sprite::create("SelectLevel.png");

	selectLevel->setScale(visibleSize.width / background->getContentSize().width*0.9, visibleSize.height / background->getContentSize().height*0.8);

	x = origin.x + visibleSize.width / 2;
	y = origin.y + visibleSize.height * (1.9 / 6);

	selectLevel->setPosition(Vec2(x, y));

	this->addChild(selectLevel, 1);

	//Setup the Options Sign

	options = Sprite::create("Options.png");

	options->setScale(visibleSize.width / background->getContentSize().width*0.8, visibleSize.height / background->getContentSize().height*0.7);

	x = origin.x + visibleSize.width / 2;
	y = origin.y + visibleSize.height * (1.2 / 6);

	options->setPosition(Vec2(x, y));

	this->addChild(options, 1);

	//Setup the Exit Sign

	exit = Sprite::create("Exit.png");

	exit->setScale(visibleSize.width / background->getContentSize().width*0.7, visibleSize.height / background->getContentSize().height*0.6);

	x = origin.x + visibleSize.width / 2;
	y = origin.y + visibleSize.height * (0.5 / 6);

	exit->setPosition(Vec2(x, y));

	this->addChild(exit, 1);

	//Setup the Gun Sign

	gunSign = Sprite::create("MenuGun.png");

	gunSign->setScale(visibleSize.width / background->getContentSize().width, visibleSize.height / background->getContentSize().height);

	x = origin.x + visibleSize.width * (3.5 / 11);
	y = origin.y + visibleSize.height * (2.6 / 6);

	gunSign->setPosition(Vec2(x, y));

	this->addChild(gunSign, 1);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	this->scheduleUpdate();

	return true;
}

void MenuScreen::update(float deltaTime)
{
	gameTime += deltaTime;

	auto visibleSize = director->getVisibleSize();
	Vec2 origin = director->getVisibleOrigin();

	//select different menu sign
	if (INPUTS->getKeyPress(KeyCode::KEY_S) && (gunSign->getPosition().y < origin.y + visibleSize.height * (1.2 / 6) + 1) && (gunSign->getPosition().y > origin.y + visibleSize.height * (1.2 / 6) - 1))
	{
		float x = origin.x + visibleSize.width * (3.5 / 11);
		float y = origin.y + visibleSize.height * (0.5 / 6);
		gunSign->setPosition(x, y);
		exit->runAction(Blink::create(0.1f, 1));
		options->setScale(visibleSize.width / background->getContentSize().width*0.8, visibleSize.height / background->getContentSize().height*0.8);
		exit->setScale(visibleSize.width / background->getContentSize().width, visibleSize.height / background->getContentSize().height);
		selectLevel->setScale(visibleSize.width / background->getContentSize().width*0.7, visibleSize.height / background->getContentSize().height*0.7);
		startGame->setScale(visibleSize.width / background->getContentSize().width*0.6, visibleSize.height / background->getContentSize().height*0.6);
	}
	if (INPUTS->getKeyPress(KeyCode::KEY_S) && (gunSign->getPosition().y == origin.y + visibleSize.height * (1.9 / 6)))
	{
		float x = origin.x + visibleSize.width * (3.5 / 11);
		float y = origin.y + visibleSize.height * (1.2 / 6);
		gunSign->setPosition(x, y);
		options->runAction(Blink::create(0.1f, 1));
		selectLevel->setScale(visibleSize.width / background->getContentSize().width*0.8, visibleSize.height / background->getContentSize().height*0.8);
		options->setScale(visibleSize.width / background->getContentSize().width, visibleSize.height / background->getContentSize().height);
		startGame->setScale(visibleSize.width / background->getContentSize().width*0.7, visibleSize.height / background->getContentSize().height*0.7);
		exit->setScale(visibleSize.width / background->getContentSize().width*0.8, visibleSize.height / background->getContentSize().height*0.8);
	}
	if (INPUTS-> getKeyPress(KeyCode::KEY_S) && (gunSign->getPosition().y == origin.y + visibleSize.height * (2.6 / 6)))
	{
		float x = origin.x + visibleSize.width * (3.5 / 11);
		float y = origin.y + visibleSize.height * (1.9 / 6);
		gunSign->setPosition(x, y);
		selectLevel->runAction(Blink::create(0.1f, 1));
		exit->setScale(visibleSize.width / background->getContentSize().width*0.7, visibleSize.height / background->getContentSize().height*0.7);
		startGame->setScale(visibleSize.width / background->getContentSize().width*0.8, visibleSize.height / background->getContentSize().height*0.8);
		selectLevel->setScale(visibleSize.width / background->getContentSize().width, visibleSize.height / background->getContentSize().height);
		options->setScale(visibleSize.width / background->getContentSize().width*0.8, visibleSize.height / background->getContentSize().height*0.8);
	}
	if (INPUTS->getKeyPress(KeyCode::KEY_W) && (gunSign->getPosition().y == origin.y + visibleSize.height * (1.9 / 6)))
	{
		float x = origin.x + visibleSize.width * (3.5 / 11);
		float y = origin.y + visibleSize.height * (2.6 / 6);
		gunSign->setPosition(x, y);
		startGame->runAction(Blink::create(0.1f, 1));
		selectLevel->setScale(visibleSize.width / background->getContentSize().width*0.8, visibleSize.height / background->getContentSize().height*0.8);
		startGame->setScale(visibleSize.width / background->getContentSize().width, visibleSize.height / background->getContentSize().height);
		options->setScale(visibleSize.width / background->getContentSize().width*0.7, visibleSize.height / background->getContentSize().height*0.7);
		exit->setScale(visibleSize.width / background->getContentSize().width*0.6, visibleSize.height / background->getContentSize().height*0.6);
	}
	if (INPUTS->getKeyPress(KeyCode::KEY_W) && (gunSign->getPosition().y < origin.y + visibleSize.height * (1.2 / 6) + 1) && (gunSign->getPosition().y > origin.y + visibleSize.height * (1.2 / 6) - 1))
	{
		float x = origin.x + visibleSize.width * (3.5 / 11);
		float y = origin.y + visibleSize.height * (1.9 / 6);
		gunSign->setPosition(x, y);
		selectLevel->runAction(Blink::create(0.1f, 1));
		options->setScale(visibleSize.width / background->getContentSize().width*0.8, visibleSize.height / background->getContentSize().height*0.8);
		selectLevel->setScale(visibleSize.width / background->getContentSize().width, visibleSize.height / background->getContentSize().height);
		exit->setScale(visibleSize.width / background->getContentSize().width*0.7, visibleSize.height / background->getContentSize().height*0.7);
		startGame->setScale(visibleSize.width / background->getContentSize().width*0.8, visibleSize.height / background->getContentSize().height*0.8);
	}
	if (INPUTS->getKeyPress(KeyCode::KEY_W) && (gunSign->getPosition().y == origin.y + visibleSize.height * (0.5 / 6)))
	{
		float x = origin.x + visibleSize.width * (3.5 / 11);
		float y = origin.y + visibleSize.height * (1.2 / 6);
		gunSign->setPosition(x, y);
		options->runAction(Blink::create(0.1f, 1));
		startGame->setScale(visibleSize.width / background->getContentSize().width*0.7, visibleSize.height / background->getContentSize().height*0.7);
		exit->setScale(visibleSize.width / background->getContentSize().width*0.8, visibleSize.height / background->getContentSize().height*0.8);
		options->setScale(visibleSize.width / background->getContentSize().width, visibleSize.height / background->getContentSize().height);
		selectLevel->setScale(visibleSize.width / background->getContentSize().width*0.8, visibleSize.height / background->getContentSize().height*0.8);
	}

	//update for enter different section
	//Exit
	if (INPUTS->getKeyPress(KeyCode::KEY_ENTER) && (gunSign->getPosition().y == origin.y + visibleSize.height * (0.5 / 6)))
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/boom.wav");
		director->end();
	}
	//Options
	if (INPUTS->getKeyPress(KeyCode::KEY_ENTER) && (gunSign->getPosition().y < origin.y + visibleSize.height * (1.2 / 6) + 1) && (gunSign->getPosition().y > origin.y + visibleSize.height * (1.2 / 6) - 1))
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/boom.wav");
	}
	//Select Level
	if (INPUTS->getKeyPress(KeyCode::KEY_ENTER) && (gunSign->getPosition().y == origin.y + visibleSize.height * (1.9 / 6)))
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/boom.wav");
	}
	//Start Game
	if (INPUTS->getKeyPress(KeyCode::KEY_ENTER) && (gunSign->getPosition().y == origin.y + visibleSize.height * (2.6 / 6)))
	{
		//will make a gun shooting noise 
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/boom.wav");
		Step(2.0f);
	}


	//update the keybord each frame
	INPUTS->clearForNextFrame();
}

void MenuScreen::Step(float dt)
{
	this->unschedule(schedule_selector(MenuScreen::Step));
	CCDirector::sharedDirector()->purgeCachedData();
	CCScene *pScene = HelloWorld::create();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(2.0f, pScene));
}