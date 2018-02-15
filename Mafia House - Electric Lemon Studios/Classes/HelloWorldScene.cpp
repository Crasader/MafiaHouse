#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "DisplayHandler.h"
#include "InputHandler.h"
#include <iostream>
#include <vector>

using namespace std;

USING_NS_CC;

Scene* HelloWorld::createScene()
{
	//Create a scene without physics (we're implementing our own!)
	Scene* scene = Scene::create();


	// for debug window
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);



	//Create the main layer that is attached to the scene
	HelloWorld* layer = HelloWorld::create();

	//Add the layer to the scene
	scene->addChild(layer);



	return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }


    //auto visibleSize = Director::getInstance()->getVisibleSize();
    //Vec2 origin = Director::getInstance()->getVisibleOrigin();


	// Init
	director = Director::getInstance();

	// Background Color
	/*director->setClearColor(Color4F(255, 255, 255, 255));*/

	// Get Window Size
	windowSize = DISPLAY->getWindowSizeAsVec2();

	// Setup Stuff
	setup();
	
	
	
	//Schedule the use of the update function so the function actually gets called
	this->scheduleUpdate();

	//CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("Audio/mainMenu.mp3");
	//CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Audio/mainMenu.mp3");

    return true;
}

// Run this function once
void HelloWorld::setup()
{

	auto visibleSize = director->getVisibleSize();
	Vec2 origin = director->getVisibleOrigin();


	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/equip.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/walk.wav");


	//set collision box foe screen
	/*auto edgebody = PhysicsBody::createEdgeBox(visibleSize, { 0, 0, 0 }, 10);
	edgebody->setContactTestBitmask(true);
	auto edgeNote = Node::create();
	edgeNote->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	edgeNote->setPhysicsBody(edgebody);
	this->addChild(edgeNote);
*/



	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	/*auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

	if (closeItem == nullptr ||
		closeItem->getContentSize().width <= 0 ||
		closeItem->getContentSize().height <= 0)
	{
		problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
	}
	else
	{
		float x = origin.x + closeItem->getContentSize().width / 2;
		float y = origin.y + visibleSize.height - closeItem->getContentSize().height / 2;
		closeItem->setPosition(Vec2(x, y));
	}*/

	//// create menu, it's an autorelease object
	//auto menu = Menu::create(closeItem, NULL);
	//menu->setPosition(Vec2::ZERO);
	//this->addChild(menu, 1);

	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "Hello World"
	// create and initialize a label

	//auto label = Label::createWithTTF("Here is Mafia House.", "fonts/Marker Felt.ttf", 24);
	//if (label == nullptr)
	//{
	//	problemLoading("'fonts/Marker Felt.ttf'");
	//}
	//else
	//{
	//	// position the label on the center of the screen
	//	label->setPosition(Vec2(origin.x + visibleSize.width / 2,
	//		origin.y + visibleSize.height - label->getContentSize().height));

	//	// add the label as a child to this layer
	//	this->addChild(label, 1);
	//}

	// add "HelloWorld" splash screen"
	//sprite = Sprite::create("HelloWorld.png");
	//if (sprite == nullptr)
	//{
	//	problemLoading("'HelloWorld.png'");
	//}
	//else
	//{
	//	float x = origin.x + visibleSize.width / 2;
	//	float y = origin.y + visibleSize.height / 2;
	//	// position the sprite on the center of the screen
	//	sprite->setPosition(Vec2(x, y));

	//	// add the sprite as a child to this layer
	//	this->addChild(sprite, 0);
	//	/*sprite->runAction(FadeOut::create(1));*/
	//}

	//// add group logo
	//auto sprite2 = Sprite::create("Electric Lemon.png");
	//if (sprite2 == nullptr)
	//{
	//	problemLoading("'Electric Lemon.png'");
	//}
	//else
	//{
	//	float x = origin.x + visibleSize.width - sprite2->getContentSize().width / 2;
	//	float y = origin.y + visibleSize.height - sprite2->getContentSize().height / 2;
	//	// position the sprite on the center of the screen
	//	sprite2->setPosition(Vec2(x, y));

	//	// add the sprite as a child to this layer
	//	this->addChild(sprite2, 0);
	//}

	//set up background

	background = Sprite::create("backscroll.png");
	if (background == nullptr)
	{
		problemLoading("'backscroll.png'");
	}
	else
	{
		background->setScale(visibleSize.height / background->getContentSize().height, visibleSize.height / background->getContentSize().height);
		// position the sprite on the center of the screen
		background->setPosition(Vec2(origin.x + (background->getContentSize().width/2)*(visibleSize.height / background->getContentSize().height), origin.y + visibleSize.height / 2));

		// add the sprite as a child to this layer
		this->addChild(background, 0);
	}

	//setup character
	character = Sprite::create("char.png");
	if (character == nullptr)
	{
		problemLoading("'char.png'");
	}
	else
	{
		float x = origin.x + visibleSize.width / 2;
		float y = origin.y + visibleSize.height / 8;
		characterPosition = Vec2(x, y);
		// position the sprite on the center of the screen
		character->setPosition(Vec2(x, y));

		// add the sprite as a child to this layer
		this->addChild(character, 4);
	}

	//setup inventery Box
	box = Sprite::create("box.png");
	if (box == nullptr)
	{
		problemLoading("'box.png'");
	}
	else
	{
		float x = origin.x + visibleSize.width - 50;
		float y = origin.y + visibleSize.height - 50;
		boxPosition = Vec2(x, y);
		// position the sprite on the center of the screen
		box->setPosition(Vec2(x, y));

		// add the sprite as a child to this layer
		this->addChild(box, 2);
	}

	//setup Knife
	knife = Sprite::create("knife.png");
	if (knife == nullptr)
	{
		problemLoading("'knife.png'");
	}
	else
	{
		knife->setScale(0.7, 0.7);
		float x = origin.x + visibleSize.width / 4 * 3;
		float y = origin.y + visibleSize.height/ 20;
		knifePosition = Vec2(x, y);
		// position the sprite on the center of the screen
		knife->setPosition(Vec2(x, y));

		// add the sprite as a child to this layer
		this->addChild(knife, 3);
	}

	//setup enemy
	enemy = Sprite::create("enemy.png");
	if (enemy == nullptr)
	{
		problemLoading("'enemy.png'");
	}
	else
	{
		enemy->runAction(FlipX::create(true));
		float x = origin.x + visibleSize.width / 2 * 3;
		float y = origin.y + visibleSize.height / 8;
		enemyPosition = Vec2(x, y);
		// position the sprite on the center of the screen
		enemy->setPosition(Vec2(x, y));

		// add the sprite as a child to this layer
		this->addChild(enemy, 3);
	}

	//setup exclamation mark
	exmark = Sprite::create("exmark.png");
	if (exmark == nullptr)
	{
		problemLoading("'exmark.png'");
	}
	else
	{
		/*float x = origin.x + visibleSize.width / 2 * 3;
		float y = origin.y + visibleSize.height / 8 + 110;*/
		// position the sprite on the center of the screen
		exmark->setPosition(Vec2(-100, -100));

		// add the sprite as a child to this layer
		this->addChild(exmark, 3);
	}

	//lables
	label2 = Label::createWithTTF("<-A Left or D Right->", "fonts/Nexa_Bold.otf", 24);
	label2->setPosition(Vec2(-200,-200));

	this->addChild(label2, 4);

	label1 = Label::createWithTTF("Space for Pick Up", "fonts/Nexa_Bold.otf", 24);
	label1->setPosition(Vec2(-200, -200));

	this->addChild(label1, 4);

	/*this->schedule(schedule_selector(HelloWorld::Step), 3.0f);*/
}

// Update per frame
void HelloWorld::update(float deltaTime)
{
	gameTime += deltaTime;

	auto visibleSize = director->getVisibleSize();
	Vec2 origin = director->getVisibleOrigin();
	
	/*sprite->runAction(FadeOut::create(1));*/
	

	Vec2 MousePos = INPUTS->getMousePosition();

	//if (INPUTS->getKey(KeyCode::KEY_W))
	//	characterPosition += Vec2(0, +1) * characterSpeed;

	//if (INPUTS->getKey(KeyCode::KEY_S))
	//	characterPosition += Vec2(0, -1) * characterSpeed;

	if (INPUTS->getKey(KeyCode::KEY_A) && characterPosition.x > visibleSize.width / 2)
	{
		if (knifePosition == boxPosition)
		{
			knifePosition += Vec2(-1, 0) * characterSpeed;
		}
		boxPosition += Vec2(-1, 0) * characterSpeed;
		characterPosition += Vec2(-1, 0) * characterSpeed;
		if (!character->isFlippedX())
		{
			auto flipxAction = FlipX::create(true);
			character->runAction(flipxAction);
		}
	}
	if (INPUTS->getKeyPress(KeyCode::KEY_A) && characterPosition.x > visibleSize.width / 2)
	{
		id = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/walk.wav", true, 2.0, 0.0f, 2.0f);
	}
	if (INPUTS->getKeyRelease(KeyCode::KEY_A) && characterPosition.x > visibleSize.width / 2)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(id);
	}

	if (INPUTS->getKey(KeyCode::KEY_D) && characterPosition.x < enemyPosition.x-60/*background->getContentSize().width)*(visibleSize.height / background->getContentSize().height) - visibleSize.width / 2*/)
	{
		if (knifePosition == boxPosition)
		{
			knifePosition += Vec2(+1, 0) * characterSpeed;
		}
		boxPosition += Vec2(+1, 0) * characterSpeed;
		characterPosition += Vec2(+1, 0) * characterSpeed;
		if (character->isFlippedX())
		{
			auto flipxAction = FlipX::create(false);
			character->runAction(flipxAction);
		}
	}

	if (INPUTS->getKeyPress(KeyCode::KEY_D) && characterPosition.x < (background->getContentSize().width)*(visibleSize.height / background->getContentSize().height) - visibleSize.width / 2)
	{
		id = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/walk.wav", true);
	}
	if (INPUTS->getKeyRelease(KeyCode::KEY_D) && characterPosition.x < (background->getContentSize().width)*(visibleSize.height / background->getContentSize().height) - visibleSize.width / 2)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(id);
	}

	if (abs(knifePosition.x - characterPosition.x) < 50)
	{
		////////
		label1->setPosition(characterPosition.x, characterPosition.y + 150);
		if (gameTime - a > 0.3)
		{
			knife->runAction(Blink::create(0.2f, 1));
			a = gameTime;
		}
		if (INPUTS->getKey(KeyCode::KEY_SPACE))
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/equip.wav");
			knife->setScale(1, 1);
			knifePosition = boxPosition;
		}
	}
	else
	{
		label1->setPosition(Vec2(-200, -200));
	}

	if (abs(enemyPosition.x - characterPosition.x) < 400 && enemy->isFlipX())
	{
		float x = enemyPosition.x;
		float y = origin.y + visibleSize.height / 8 + 110;
		exmark->setPosition(Vec2(x, y));
		if (gameTime - a > 0.5)
		{
			exmark->runAction(Blink::create(0.3f, 1));
			a = gameTime;
		}
	}
	else
	{
		exmark->setPosition(Vec2(-100, -100));
		if (enemy->isFlipX())
		{
			enemyPosition.x -= 3;
		}
		if (!enemy->isFlipX())
		{
			enemyPosition.x += 3;
		}
		if (enemyPosition.x < 1900)
		{
			enemy->setFlipX(false);
		}
		if (enemyPosition.x > 2702)
		{
			enemy->setFlipX(true);
		}
	}

	if (characterPosition.x < 1150)
	{
		label2->setPosition(characterPosition.x, characterPosition.y + 150);
	}
	else
	{
		label2->setPosition(Vec2(-200, -200));
	}


	
	/*if (INPUTS->getKeyPress(KeyCode::KEY_Q))
	{
		CCJumpTo* jump = JumpTo::create(1, ccp(director->getVisibleOrigin().x / 6, director->getVisibleOrigin().y / 8), director->getVisibleOrigin().y / 1.2f, 1);
		if (a == 0)
		{
			character->runAction(FadeOut::create(1.0f));
			a = 1;
		}		
	}*/
	//if (INPUTS->getKeyPress(KeyCode::KEY_E))
	//{
	//	if (a == 1)
	//	{
	//		character->runAction(FadeIn::create(1.0f));
	//		a = 0;
	//	}
	//}

	auto cam = Camera::getDefaultCamera();
	cam->setPosition(director->getVisibleOrigin().x + characterPosition.x, director->getVisibleOrigin().y + director->getVisibleSize().height / 2);

	if(character != nullptr)
		character->setPosition(characterPosition);
	if (box != nullptr)
		box->setPosition(boxPosition);
	if (knife != nullptr)
		knife->setPosition(knifePosition);
	if (enemy != nullptr)
		enemy->setPosition(enemyPosition);

	//update the keybord each frame
	INPUTS->clearForNextFrame();
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

void HelloWorld::Step(float dt)
{
	this->unschedule(schedule_selector(HelloWorld::Step));
	CCDirector::sharedDirector()->purgeCachedData();
	CCScene *pScene = Scene::create();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(2.0f, pScene));
}
