#ifndef __START_SCREEN_H__
#define __START_SCREEN_H__

#include "cocos2d.h"
using namespace cocos2d;

class StartScreen : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	// initialize the secene
	virtual bool init();

	// update function for every frame
	void update(float deltaTime);

	// implement the "static create()" method manually
	CREATE_FUNC(StartScreen);

	// help to switch to next scene
	void Step(float dt);

private:
	//--- Cocos Engine References ---//
	Director* director;

	// sprite for game logo
	Sprite* sprite;

	// label for start game
	Label* label;

	// count for how many frame passed
	int frameCount = 0;
};

#endif // __START_SCREEN_H__
