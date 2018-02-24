#ifndef __MENU_SCREEN_H__
#define __MENU_SCREEN_H__

#include "cocos2d.h"
using namespace cocos2d;

class MenuScreen : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	// initialize the secene
	virtual bool init();

	// update function for every frame
	void update(float deltaTime);

	// implement the "static create()" method manually
	CREATE_FUNC(MenuScreen);

private:
	//--- Cocos Engine References ---//
	Director* director;
	Vec2 visibleSize;
	Vec2 origin;
	float gameTime = 0.0f;

	// all asset for Menu Screen 
	Sprite* background;
	Sprite* startGame;
	Sprite* selectLevel;
	Sprite* options;
	Sprite* exit;
	Sprite* gunSign;

	// position for gunSign
	Vec2 positionStart;
	Vec2 positionSelect;
	Vec2 positionOption;
	Vec2 positionExit;

	// scale size for menu sign
	Vec2 scale1;
	Vec2 scale2;
	Vec2 scale3;
	Vec2 scale4;

};

#endif // !__MENU_SCREEN_H__
