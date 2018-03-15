#include "SimpleAudioEngine.h"	//to play music when this scene starts
#include "MenuScreen.h"
#include "DisplayHandler.h"
#include "InputHandler.h"
#include "HelloWorldScene.h"
#include "Stages.h"
#include "Tutorial.h"
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

// initialize the secene
bool MenuScreen::init()
{
	// super init first
	if (!Scene::init())
	{
		return false;
	}

	// Init
	director = Director::getInstance();

	// get visible windows sizw and the origin position of gamw world
	visibleSize = director->getVisibleSize();
	origin = director->getVisibleOrigin();


	////Setup everything//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// start playing background music 
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic("Audio/menu.wav");
	audio->playBackgroundMusic("Audio/menu.wav");

	//initialize(preload) sound effects here 
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/boom.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/LoadBullet.wav");

	//////////////////////////
	// Setup the background

	background = Sprite::create("MainMenu.png");

	// setup the different scale size
	scale1 = Vec2(visibleSize.x / background->getContentSize().width, visibleSize.y / background->getContentSize().height);
	scale2 = Vec2(visibleSize.x / background->getContentSize().width*0.8, visibleSize.y / background->getContentSize().height*0.8);
	scale3 = Vec2(visibleSize.x / background->getContentSize().width*0.7, visibleSize.y / background->getContentSize().height*0.7);
	scale4 = Vec2(visibleSize.x / background->getContentSize().width*0.6, visibleSize.y / background->getContentSize().height*0.6);

	// set the background size same as window size
	background->setScale(scale1.x, scale1.y);

	// position the background on the center of the screen
	float x = origin.x + visibleSize.x / 2;
	float y = origin.y + visibleSize.y / 2;
	background->setPosition(Vec2(x, y));

	// add the background as a child to this layer
	this->addChild(background, 0);

	//////////////////////////////
	// Setup the Start Game Sign

	startGame = Sprite::create("StartGame.png");

	startGame->setScale(scale1.x, scale1.y);

	x = origin.x + visibleSize.x / 2;
	y = origin.y + visibleSize.y * (2.6 / 6);

	startGame->setPosition(Vec2(x, y));

	this->addChild(startGame, 1);

	///////////////////////////////
	// Setup the Select Level Sign

	selectLevel = Sprite::create("SelectLevel.png");

	selectLevel->setScale(scale2.x, scale2.y);

	x = origin.x + visibleSize.x / 2;
	y = origin.y + visibleSize.y * (1.9 / 6);

	selectLevel->setPosition(Vec2(x, y));

	this->addChild(selectLevel, 1);

	////////////////////////////////
	// Setup the Options Sign

	options = Sprite::create("Options.png");

	options->setScale(scale3.x, scale3.y);

	x = origin.x + visibleSize.x / 2;
	y = origin.y + visibleSize.y * (1.2 / 6);

	options->setPosition(Vec2(x, y));

	this->addChild(options, 1);

	////////////////////////////////
	// Setup the Exit Sign

	exit = Sprite::create("Exit.png");

	exit->setScale(scale4.x, scale4.y);

	x = origin.x + visibleSize.x / 2;
	y = origin.y + visibleSize.y * (0.5 / 6);

	exit->setPosition(Vec2(x, y));

	this->addChild(exit, 1);

	////////////////////////////////
	// Setup the Gun Sign

	gunSign = Sprite::create("MenuGun.png");

	gunSign->setScale(scale1.x, scale1.y);

	// set the different position of gunsign
	positionStart = Vec2(origin.x + visibleSize.x * (3.5 / 11), origin.y + visibleSize.y * (2.6 / 6));
	positionSelect = Vec2(origin.x + visibleSize.x * (3.5 / 11), origin.y + visibleSize.y * (1.9 / 6));
	positionOption = Vec2(origin.x + visibleSize.x * (3.5 / 11), origin.y + visibleSize.y * (1.2 / 6));
	positionExit = Vec2(origin.x + visibleSize.x * (3.5 / 11), origin.y + visibleSize.y * (0.5 / 6));

	gunSign->setPosition(positionStart);

	this->addChild(gunSign, 1);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//Schedule the use of the update function so the function actually gets called
	this->scheduleUpdate();

	return true;
}

// update function for every frame
void MenuScreen::update(float deltaTime)
{
	gameTime += deltaTime;

	////////////////////////////////////////////////
	// Move gun sign to select different menu sign

	// use S key to move down
	// move from option to exit
	if (INPUTS->getKeyPress(KeyCode::KEY_S) && (gunSign->getPosition() == positionOption))
	{
		// play the music of loadbullet
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/LoadBullet.wav");
		// blink the sign
		exit->runAction(Blink::create(0.1f, 1));
		// set the gun to new position
		gunSign->setPosition(positionExit);
		// scale all menu sign to right size
		startGame->setScale(scale4.x, scale4.y);
		selectLevel->setScale(scale3.x, scale3.y);
		options->setScale(scale2.x, scale2.y);
		exit->setScale(scale1.x, scale1.y);
	}
	// move from selectlevel to option
	if (INPUTS->getKeyPress(KeyCode::KEY_S) && (gunSign->getPosition() == positionSelect))
	{
		// play the music of loadbullet
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/LoadBullet.wav");
		// blink the sign
		options->runAction(Blink::create(0.1f, 1));
		// set the gun to new position
		gunSign->setPosition(positionOption);
		// scale all menu sign to right size
		startGame->setScale(scale3.x, scale3.y);
		selectLevel->setScale(scale2.x, scale2.y);
		options->setScale(scale1.x, scale1.y);
		exit->setScale(scale2.x, scale2.y);
	}
	// move from startgame to selectlevel
	if (INPUTS-> getKeyPress(KeyCode::KEY_S) && (gunSign->getPosition() == positionStart))
	{
		// play the music of loadbullet
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/LoadBullet.wav");
		// blink the sign
		selectLevel->runAction(Blink::create(0.1f, 1));
		// set the gun to new position
		gunSign->setPosition(positionSelect);
		// scale all menu sign to right size
		startGame->setScale(scale2.x, scale2.y);
		selectLevel->setScale(scale1.x, scale1.y);
		options->setScale(scale2.x, scale2.y);
		exit->setScale(scale3.x, scale3.y);
	}

	// use W key to move up
	// move from selectlevel to startgame
	if (INPUTS->getKeyPress(KeyCode::KEY_W) && (gunSign->getPosition() == positionSelect))
	{
		// play the music of loadbullet
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/LoadBullet.wav");
		// blink the sign
		startGame->runAction(Blink::create(0.1f, 1));
		// set the gun to new position
		gunSign->setPosition(positionStart);
		// scale all menu sign to right size
		startGame->setScale(scale1.x, scale1.y);
		selectLevel->setScale(scale2.x, scale2.y);
		options->setScale(scale3.x, scale3.y);
		exit->setScale(scale4.x, scale4.y);
	}
	// move from option to selectlevel
	if (INPUTS->getKeyPress(KeyCode::KEY_W) && (gunSign->getPosition() == positionOption))
	{
		// play the music of loadbullet
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/LoadBullet.wav");
		// blink the sign
		selectLevel->runAction(Blink::create(0.1f, 1));
		// set the gun to new position
		gunSign->setPosition(positionSelect);
		// scale all menu sign to right size
		startGame->setScale(scale2.x, scale2.y);
		selectLevel->setScale(scale1.x, scale1.y);
		options->setScale(scale2.x, scale2.y);
		exit->setScale(scale3.x, scale3.y);
	}
	// move from exit to option
	if (INPUTS->getKeyPress(KeyCode::KEY_W) && (gunSign->getPosition() == positionExit))
	{
		// play the music of loadbullet
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/LoadBullet.wav");
		// blink the sign
		options->runAction(Blink::create(0.1f, 1));
		// set the gun to new position
		gunSign->setPosition(positionOption);
		// scale all menu sign to right size
		startGame->setScale(scale3.x, scale3.y);
		selectLevel->setScale(scale2.x, scale2.y);
		options->setScale(scale1.x, scale1.y);
		exit->setScale(scale2.x, scale2.y);
	}

	//////////////////////////////////////////////////
	// Press Enter for go to different section
	// Start Game
	if (INPUTS->getKeyPress(KeyCode::KEY_ENTER) && (gunSign->getPosition() == positionStart))
	{
		//will make a gun shooting noise 
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/boom.wav");
		// switch to tutorial scene
		director->replaceScene(Stage1::createScene());
	}
	// Select Level
	if (INPUTS->getKeyPress(KeyCode::KEY_ENTER) && (gunSign->getPosition() == positionSelect))
	{
		//will make a gun shooting noise 
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/boom.wav");
	}
	// Options
	if (INPUTS->getKeyPress(KeyCode::KEY_ENTER) && (gunSign->getPosition() == positionOption))
	{
		//will make a gun shooting noise 
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/boom.wav");
	}
	// Exit
	if (INPUTS->getKeyPress(KeyCode::KEY_ENTER) && (gunSign->getPosition() == positionExit))
	{
		// will make a gun shooting noise 
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/boom.wav");
		// exit the game
		director->end();
	}

	//update the keybord each frame
	INPUTS->clearForNextFrame();
}