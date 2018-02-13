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


    return true;
}

// Run this function once
void HelloWorld::setup()
{
	auto visibleSize = director->getVisibleSize();
	Vec2 origin = director->getVisibleOrigin();

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
	auto closeItem = MenuItemImage::create(
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
		float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
		float y = origin.y + closeItem->getContentSize().height / 2;
		closeItem->setPosition(Vec2(x, y));
	}

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

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

	background = Sprite::create("background.png");
	if (background == nullptr)
	{
		problemLoading("'background.png'");
	}
	else
	{
		// position the sprite on the center of the screen
		background->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));

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
		float x = origin.x + character->getContentSize().width / 2;
		float y = origin.y + visibleSize.height / 3;
		// position the sprite on the center of the screen
		character->setPosition(Vec2(x, y));

		// add the sprite as a child to this layer
		this->addChild(character, 2);
	}



	/*this->schedule(schedule_selector(HelloWorld::Step), 3.0f);*/
}

// Update per frame
void HelloWorld::update(float deltaTime)
{
	gameTime += deltaTime;

	
	/*sprite->runAction(FadeOut::create(1));*/
	

	Vec2 MousePos = INPUTS->getMousePosition();

	if (INPUTS->getKey(KeyCode::KEY_W))
		characterPosition += Vec2(0, +1) * characterSpeed;

	if (INPUTS->getKey(KeyCode::KEY_S))
		characterPosition += Vec2(0, -1) * characterSpeed;

	if (INPUTS->getKey(KeyCode::KEY_A))
	{
		characterPosition += Vec2(-1, 0) * characterSpeed;
		if (!character->isFlippedX())
		{
			auto flipxAction = FlipX::create(true);
			character->runAction(flipxAction);
		}
	}

	if (INPUTS->getKey(KeyCode::KEY_D))
	{
		characterPosition += Vec2(+1, 0) * characterSpeed;
		if (character->isFlippedX())
		{
			auto flipxAction = FlipX::create(false);
			character->runAction(flipxAction);
		}
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


	if(character != nullptr)
		character->setPosition(characterPosition);

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
