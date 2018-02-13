#ifndef __MENU_SCREEN_H__
#define __MENU_SCREEN_H__

#include "cocos2d.h"
using namespace cocos2d;

class MenuScreen : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void update(float deltaTime);

	CREATE_FUNC(MenuScreen);

private:
	//--- Cocos Engine References ---//
	Director* director;
	Vec2 windowSize;
	float gameTime = 0.0f;

	//Put your code here
	Sprite* sprite;
};

#endif // !__MENU_SCREEN_H__
