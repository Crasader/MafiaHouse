#ifndef __GROUP_LOGO_H__
#define __GROUP_LOGO_H__

#include "cocos2d.h"
using namespace cocos2d;

class GroupLogo : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(GroupLogo);

	void Step(float dt);

private:
	//--- Cocos Engine References ---//
	Director* director;
	Vec2 windowSize;
	float gameTime = 0.0f;

	//character
	Sprite* sprite;
};

#endif // __GROUP_LOGO_H__
