#ifndef __COCO_LOGO_H__
#define __COCO_LOGO_H__

#include "cocos2d.h"
using namespace cocos2d;

class CocoLogo : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void update(float deltaTime);

	CREATE_FUNC(CocoLogo);

	void Step(float dt);

private:
	//--- Cocos Engine References ---//
	Director* director;
	Vec2 windowSize;

	//character
	Sprite* sprite;
};

#endif // __COCO_LOGO_H__