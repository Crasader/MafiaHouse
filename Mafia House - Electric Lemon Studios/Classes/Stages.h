#pragma once

#include "cocos2d.h"
#include "InputHandler.h"
#include "Level.h"
USING_NS_CC;

class Tutorial1 : public Level
{
public:
	CREATE_SCENE_FUNC(Tutorial1);
	CREATE_FUNC(Tutorial1);

	RESET_LEVEL;

	bool init();
};

class Stage1 : public Level
{
public:
	CREATE_SCENE_FUNC(Stage1);
	CREATE_FUNC(Stage1);

	RESET_LEVEL;

	bool init();
};

class Stage2 : public Level
{
public:
	CREATE_SCENE_FUNC(Stage2);
	CREATE_FUNC(Stage2);

	RESET_LEVEL;

	bool init();
};

class Stage3 : public Level
{
public:
	CREATE_SCENE_FUNC(Stage2);
	CREATE_FUNC(Stage2);

	RESET_LEVEL;

	bool init();
};