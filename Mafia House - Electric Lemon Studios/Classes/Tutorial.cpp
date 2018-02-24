#include "SimpleAudioEngine.h"
#include "Tutorial.h"
#include "DisplayHandler.h"
#include "InputHandler.h"
#include <iostream>

using namespace std;

cocos2d::Scene* Tutorial::createScene()
{
	//Create a scene without physics
	Scene* scene = Scene::create();

	//Create the main layer that is attached to the scene
	Tutorial* layer = Tutorial::create();

	//Add the layer to the scene
	scene->addChild(layer);

	//Return the newly built scene
	return scene;
}


// initialize the secene
bool Tutorial::init()
{
	// super init first
	if (!Scene::init())
	{
		return false;
	}

	// Init
	director = Director::getInstance();

	// Background Color
	director->setClearColor(Color4F(255, 255, 255, 255));

	// Get Window Size
	windowSize = DISPLAY->getWindowSizeAsVec2();

	// Setup Stuff
	setup();

	//Schedule the use of the update function so the function actually gets called
	this->scheduleUpdate();

	return true;
}

// Run this function once for setup the scene
void Tutorial::setup()
{
	// get visible windows sizw and the origin position of gamw world
	auto visibleSize = director->getVisibleSize();
	Vec2 origin = director->getVisibleOrigin();

	///////////////////////////////////////////////////////////////////////////////////////
	// Setup Everthing Here



}


// update function for every frame
void Tutorial::update(float deltaTime)
{
	// update the game time
	gameTime += deltaTime;

	///////////////////////////////////////////////////////////////////////////////////////
	// put update function here



	//update the keybord each frame
	INPUTS->clearForNextFrame();
}