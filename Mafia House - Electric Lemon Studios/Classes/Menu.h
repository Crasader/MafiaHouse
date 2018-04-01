#pragma once
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "InputHandler.h"
#include <vector>
#include <string>
#include<fstream>
#include "Stages.h"
USING_NS_CC;
using std::vector;
using std::string;
using std::atoi;
using std::atof;

#define CREATE_MENU_FUNC(_TYPE_) \
static Scene* createScene() \
{ \
	Scene* scene = Scene::create(); \
	_TYPE_* layer = _TYPE_::create(); \
	scene->addChild(layer); \
	return scene; \
}

class MenuOption : public Sprite {
public:
	MenuOption() {}
	~MenuOption() {}

	static MenuOption* create(const std::string& filename = "menu/bullet.png"){
		MenuOption *sprite = new(std::nothrow) MenuOption();
		if (sprite && sprite->initWithFile(filename)){
			sprite->autorelease();
			return sprite;
		}
			CC_SAFE_DELETE(sprite);
			return nullptr;
	}

	void initOption(int num, string labelText) {
		optionNumber = num;
		label = Label::createWithTTF(labelText, "fonts/pixelFJ8pt1__.ttf", 22);
		label->getFontAtlas()->setAliasTexParameters();
		label->setPositionNormalized(Vec2(0.48, 0.49));
		addChild(label);
	}


	int optionNumber;
	Label* label;
};

class LevelMenuOption : public MenuOption {
public:
	LevelMenuOption() {}
	~LevelMenuOption() {}
	static LevelMenuOption* create(const std::string& filename = "menu/bullet.png") {
		LevelMenuOption *sprite = new(std::nothrow) LevelMenuOption();
		if (sprite && sprite->initWithFile(filename)) {
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}

	void initLevelOption(int num, string labelText, bool checkComplete, bool checkAchievement1, bool checkAchievement2, string recordedTime) {
		optionNumber = num;

		label = Label::createWithTTF(labelText, "fonts/pixelFJ8pt1__.ttf", 31);
		label->getFontAtlas()->setAliasTexParameters();
		label->setPositionNormalized(Vec2(0.48, 0.5));
		addChild(label);

		/*if (checkComplete == true) {
			complete->setPositionNormalized(Vec2(2.0, 0.5));
			complete->setGlobalZOrder(10);
			addChild(complete);
		}*/

		if (checkAchievement1 == true) {
			achievement1->setPositionNormalized(Vec2(0.17, 0.5));
			achievement1->setGlobalZOrder(10);
			addChild(achievement1);
		}

		if (checkAchievement2 == true) {
			achievement2->setPositionNormalized(Vec2(0.80, 0.5));
			achievement2->setGlobalZOrder(10);
			addChild(achievement2);
		}

		if (recordedTime != "0.000") {
			bestTime = Label::createWithTTF("Best Time: " + recordedTime, "fonts/pixelFJ8pt1__.ttf", 24);
			bestTime->getFontAtlas()->setAliasTexParameters();
			bestTime->setPositionNormalized(Vec2(1.4,0.5));
			bestTime->setGlobalZOrder(10);
			addChild(bestTime);
		}
	}

	Sprite* achievement1 = Sprite::create("menu/achievement1.png");
	Sprite* achievement2 = Sprite::create("menu/achievement2.png");
	Sprite* complete = Sprite::create("menu/complete.png");
	Label* bestTime;

	bool didBeat = false;
	bool gotAchievement1 = false;
	bool gotAchievement2 = false;
};


class GameMenu : public Scene{
protected:
	vector<MenuOption*> options;

	MenuOption* selectedOption;

	Sprite* selectionIndicator;

	Sprite* background;

	Node* mainLayer;

	int scrollNum = 0;

	int selectedOptionNum = 0;

	Director* director;
	Vec2 visibleSize;
	Vec2 origin;
	Vec2 centre;
};


class MainMenu : public GameMenu {
public:
	CREATE_FUNC(MainMenu);
	CREATE_MENU_FUNC(MainMenu);

	bool init();

	void update(float deltaTime);

	void onStart(float deltaTime);
};

class LevelSelectMenu : public GameMenu {
public:
	CREATE_FUNC(LevelSelectMenu);
	CREATE_MENU_FUNC(LevelSelectMenu);

	bool init();

	bool initMenu(string filename);

	void update(float deltaTime);

	void onStart(float deltaTime);

private:
	vector<LevelMenuOption*> levels;

	LevelMenuOption* selectedLevel;
};
