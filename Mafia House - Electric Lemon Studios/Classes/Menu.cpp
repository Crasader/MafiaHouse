#include "Menu.h"
#include "MenuScreen.h"

bool MainMenu::init() {
	if (!Scene::init()) { return false; }

	director = Director::getInstance();
	visibleSize = director->getVisibleSize();
	origin = director->getVisibleOrigin();
	centre = Vec2(origin.x + visibleSize.x / 2, origin.y + visibleSize.y / 2);

	mainLayer = Node::create();
	addChild(mainLayer, 1);

	background = Sprite::create("menu/MainMenu.png");
	background->setScale(visibleSize.x / background->getContentSize().width, visibleSize.y / background->getContentSize().height);
	background->setPosition(centre.x, centre.y);
	addChild(background, 0);

	selectionIndicator = Sprite::create("menu/MenuGun.png");
	//selectionIndicator->setScale(2.0f);
	mainLayer->addChild(selectionIndicator, 2);

	Vec2 optionPosition = Vec2(centre.x, origin.y + visibleSize.y - 600);//horizontal centre, at top of screen
	selectionIndicator->setPosition(optionPosition + Vec2(-550, -62));

	MenuOption* option = MenuOption::create();
	option->initOption(0, "SELECT LEVEL");
	options.push_back(option);
	option = MenuOption::create();
	option->initOption(1, "CONTROLS & TIPS");
	options.push_back(option);
	option = MenuOption::create();
	option->initOption(2, "CREDITS");
	options.push_back(option);
	option = MenuOption::create();
	option->initOption(3, "QUIT GAME");
	options.push_back(option);
	for (int i = 0; i < options.size(); i++) {
		options[i]->setPosition(optionPosition);
		options[i]->setScale(0.75f);
		mainLayer->addChild(options[i], 1);

		optionPosition += Vec2(0, -175);//setting position for next option
	}

	selectedOption = options[0];
	selectedOptionNum = 0;

	schedule(schedule_selector(LevelSelectMenu::onStart));

	return true;

	return true;
}

void MainMenu::onStart(float deltaTime) {
	unschedule(schedule_selector(LevelSelectMenu::onStart));
	scheduleUpdate();
}

void MainMenu::update(float deltaTime) {
	for (int i = 0; i < options.size(); i++) {//increaseing size of selected option
		if (options[i] == selectedOption) {
			options[i]->setScale(1.25);
		}
		else {
			options[i]->setScale(0.85);
		}
	}

	if (INPUTS->getKeyPress(KeyCode::KEY_W)) {//move seletor up
		if (selectedOptionNum >  0) {//number of selected option is greater than 0, first option in menu
			selectedOptionNum--;//go up one option
			selectedOption = options[selectedOptionNum];//setting currently selected menu option
			selectionIndicator->setPosition(selectedOption->getPosition() + Vec2(-550, -62));//setting position of seletion indicator

			if (scrollNum >= 1) {
				mainLayer->setPosition(mainLayer->getPosition() - Vec2(0, 185));
				scrollNum--;
			}
		}
	}
	else if (INPUTS->getKeyPress(KeyCode::KEY_S)) {//move selector down
		if (selectedOptionNum < options.size() - 1) {//number of selected option is greater then last option in menu
			selectedOptionNum++;//go down one option
			selectedOption = options[selectedOptionNum];//setting currently selected menu option
			selectionIndicator->setPosition(selectedOption->getPosition() + Vec2(-550, -62));//setting position of seletion indicator

			if (selectedOptionNum >= 2) {
				mainLayer->setPosition(mainLayer->getPosition() + Vec2(0, 185));
				scrollNum++;
			}
		}
	}
	else if (INPUTS->getKeyPress(KeyCode::KEY_SPACE) || INPUTS->getKeyPress(KeyCode::KEY_ENTER)) {//select currently selected level
		if (selectedOption->optionNumber == 0) {
			director->replaceScene(LevelSelectMenu::createScene());
		}
		else if (selectedOption->optionNumber == 1) {
			//Display How To Play Screen
		}
		else if (selectedOption->optionNumber == 2) {
			//Display Credits Screen
		}
		else if (selectedOption->optionNumber == 3) {
			director->end();
		}
	}

	INPUTS->clearForNextFrame();
}

bool LevelSelectMenu::init() {
	if (!Scene::init()) { return false; }

	director = Director::getInstance();
	visibleSize = director->getVisibleSize();
	origin = director->getVisibleOrigin();
	centre = Vec2(origin.x + visibleSize.x / 2, origin.y + visibleSize.y / 2);

	mainLayer = Node::create();
	addChild(mainLayer,1);

	background = Sprite::create("menu/MainMenu.png");
	background->setScale(visibleSize.x / background->getContentSize().width, visibleSize.y / background->getContentSize().height);
	background->setPosition(centre.x, centre.y);
	addChild(background, 0);

	selectionIndicator = Sprite::create("menu/MenuGun.png");
	selectionIndicator->setScale(2.0f);
	mainLayer->addChild(selectionIndicator, 2);

	auto label = Label::createWithTTF("Backspace:\nReturn to Main Menu", "fonts/pixelFJ8pt1__.ttf", 22);
	label->getFontAtlas()->setAliasTexParameters();
	label->setPositionNormalized(Vec2(0.105, 0.94));
	addChild(label);

	Vec2 optionPosition = Vec2(centre.x, origin.y + visibleSize.y - 600);//horizontal centre, at top of screen
	selectionIndicator->setPosition(optionPosition + Vec2(-550, -62));

	initMenu("menu/levels.txt");
	for (int i = 0; i < levels.size(); i++) {
		levels[i]->setPosition(optionPosition);
		mainLayer->addChild(levels[i], 1);

		optionPosition += Vec2(0, -175);//setting position for next option
	}

	selectedLevel = levels[0];
	selectedOptionNum = 0;

	schedule(schedule_selector(MainMenu::onStart));

	return true;
}

bool LevelSelectMenu::initMenu(string filename) {
	std::ifstream file;
	file.open(filename);
	if (!file) {
		CCLOG("LEVEL FILE NOT FOUND");
		return false;
	}

	int i = 0;
	string line;
	while (getline(file, line)) {//each interation gets a single line from the file; gets data for a single room
		if (line[0] == '#') { continue; }//ignoring comment lines, which begin with the '#' character

		//getting single pieces of data:
		vector<string> pieces;
		std::istringstream sstream(line);//putting line into a string stream
		string piece;
		while (getline(sstream, piece, ',')) {//each iteration gets a piece of the chunck, delmimited by the ',' character; gets a single piece of data for a component
			pieces.push_back(piece);
		}

		LevelMenuOption* option = LevelMenuOption::create();
		//option number, level name, is level complete, is first achievement complete, is second achievement complete, best time
		option->initLevelOption(i, pieces[0], atoi(pieces[1].c_str()), atoi(pieces[2].c_str()), atoi(pieces[2].c_str()), pieces[4]);

		levels.push_back(option);

		i++;
	}
	file.close();


}

void LevelSelectMenu::onStart(float deltaTime) {
	unschedule(schedule_selector(LevelSelectMenu::onStart));
	scheduleUpdate();
}

void LevelSelectMenu::update(float deltaTime) {
	for (int i = 0; i < levels.size(); i++) {//increaseing size of selected option
		if (levels[i] == selectedLevel) {
			levels[i]->setScale(1.25);
		}
		else {
			levels[i]->setScale(0.85);
		}
	}

	if (INPUTS->getKeyPress(KeyCode::KEY_W)) {//move seletor up
		if (selectedOptionNum >  0) {//number of selected option is greater than 0, first option in menu
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/LoadBullet.wav");
			selectedOptionNum--;//go up one option
			selectedLevel = levels[selectedOptionNum];//setting currently selected menu option
			selectionIndicator->setPosition(selectedLevel->getPosition() + Vec2(-550, -62));//setting position of seletion indicator

			if (scrollNum >=  1) {
				mainLayer->setPosition(mainLayer->getPosition() - Vec2(0, 185));
				scrollNum--;
			}
		}
	}
	else if (INPUTS->getKeyPress(KeyCode::KEY_S)) {//move selector down
		if (selectedOptionNum < levels.size() - 1) {//number of selected option is greater then last option in menu
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/LoadBullet.wav");
			selectedOptionNum++;//go down one option
			selectedLevel = levels[selectedOptionNum];//setting currently selected menu option
			selectionIndicator->setPosition(selectedLevel->getPosition() + Vec2(-550, -62));//setting position of seletion indicator

			if (selectedOptionNum >=  2) {
				mainLayer->setPosition(mainLayer->getPosition() + Vec2(0, 185));
				scrollNum++;
			}
		}
	}
	else if (INPUTS->getKeyPress(KeyCode::KEY_SPACE) || INPUTS->getKeyPress(KeyCode::KEY_ENTER)) {//select currently selected level
		if (selectedLevel->optionNumber == 0) {
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/boom.wav");
			director->replaceScene(Tutorial1::createScene());
		}
		else if (selectedLevel->optionNumber == 1) {
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/boom.wav");
			director->replaceScene(Stage1::createScene());
		}
		else if (selectedLevel->optionNumber == 2) {
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/boom.wav");
			director->replaceScene(Stage2::createScene());
		}
		else if (selectedLevel->optionNumber == 3) {
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/boom.wav");
			director->replaceScene(Stage3::createScene());
		}
	}
	else if (INPUTS->getKeyPress(KeyCode::KEY_BACKSPACE)) {//select currently selected level
		director->replaceScene(MenuScreen::createScene());
	}

	INPUTS->clearForNextFrame();
}
