#pragma once

#include "cocos2d.h"
#include "InputHandler.h"
#include "Level.h"
USING_NS_CC;

class Stage1 : public Level
{
public:
	CREATE_SCENE_FUNC(Stage1);
	CREATE_FUNC(Stage1);

	bool init();
};

class Stage2 : public Level
{
public:
	CREATE_SCENE_FUNC(Stage2);
	CREATE_FUNC(Stage2);

	bool init();
};