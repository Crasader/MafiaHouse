#ifndef __GROUP_LOGO_H__
#define __GROUP_LOGO_H__

#include "cocos2d.h"
using namespace cocos2d;

class GroupLogo : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	// initialize the secene
	virtual bool init();

	// update function for every frame
	void update(float deltaTime);

	// implement the "static create()" method manually
	CREATE_FUNC(GroupLogo);

	// help to switch to next scene
	void Step(float dt);

private:
	//--- Cocos Engine References ---//
	Director* director;

	// sprite for group logo
	Sprite* sprite;
};

#endif // __GROUP_LOGO_H__
