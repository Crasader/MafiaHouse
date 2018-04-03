#pragma once

#include "cocos2d.h"
#include "InputHandler.h"
#include "Level.h"
#include "Menu.h"
USING_NS_CC;

class Tutorial1 : public Level
{
public:
	CREATE_SCENE_FUNC(Tutorial1);
	CREATE_FUNC(Tutorial1);

	RESET_LEVEL;

	void startNextLevel();

	bool init();
};

class Tutorial2 : public Level
{
public:
	CREATE_SCENE_FUNC(Tutorial2);
	CREATE_FUNC(Tutorial2);

	RESET_LEVEL;
	void startNextLevel();

	bool init();
};

class Tutorial3 : public Level
{
public:
	CREATE_SCENE_FUNC(Tutorial3);
	CREATE_FUNC(Tutorial3);

	RESET_LEVEL;
	void startNextLevel();

	bool init();
};

class Stage1 : public Level
{
public:
	CREATE_SCENE_FUNC(Stage1);
	CREATE_FUNC(Stage1);

	RESET_LEVEL;
	void startNextLevel();

	bool init();
};

class Stage2 : public Level
{
public:
	CREATE_SCENE_FUNC(Stage2);
	CREATE_FUNC(Stage2);

	RESET_LEVEL;
	void startNextLevel();

	bool init();
};

class Stage3 : public Level
{
public:
	CREATE_SCENE_FUNC(Stage3);
	CREATE_FUNC(Stage3);

	RESET_LEVEL;
	void startNextLevel();

	bool init();
};

class Stage4 : public Level
{
public:
	CREATE_SCENE_FUNC(Stage4);
	CREATE_FUNC(Stage4);

	RESET_LEVEL;
	void startNextLevel();

	bool init();
};