#pragma once

#include "cocos2d.h"
#include "InputHandler.h"
#include "Player.h"
#include "Level.h"
USING_NS_CC;

class Stage1 : public Level
{
public:
	static Scene* createScene();
	virtual bool init();

	CREATE_FUNC(Stage1);
};
