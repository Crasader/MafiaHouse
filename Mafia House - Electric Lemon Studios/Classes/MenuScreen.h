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

	void Step(float dt);

private:
	//--- Cocos Engine References ---//
	Director* director;
	Vec2 windowSize;
	float gameTime = 0.0f;

	//Put your code here
	Sprite* background;
	Sprite* startGame;
	Sprite* selectLevel;
	Sprite* options;
	Sprite* exit;
	Sprite* gunSign;
};

#endif // !__MENU_SCREEN_H__
