#ifndef TUTORIAL_H
#define TUTORIAL_H

#include "cocos2d.h"
using namespace cocos2d;

class Tutorial : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	// initialize the secene
	virtual bool init();

	// update function for every frame
	void update(float deltaTime);

	// implement the "static create()" method manually
	CREATE_FUNC(Tutorial);

	//setup to help initialize the scene
	void setup();

private:
	//--- Cocos Engine References ---//
	Director* director;
	Vec2 windowSize;
	float gameTime = 0.0f;

	// add your stuff here



};

#endif // TUTORIAL_H
