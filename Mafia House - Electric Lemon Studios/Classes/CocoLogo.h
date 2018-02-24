#ifndef __COCO_LOGO_H__
#define __COCO_LOGO_H__

#include "cocos2d.h"
using namespace cocos2d;

class CocoLogo : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	// initialize the secene
	virtual bool init();

	// update function for every frame
	void update(float deltaTime);

	// implement the "static create()" method manually
	CREATE_FUNC(CocoLogo);

	// help to switch to next scene
	void Step(float dt);

private:
	//--- Cocos Engine References ---//
	Director* director;

	// sprite for cocos logo
	Sprite* sprite;
};

#endif // __COCO_LOGO_H__