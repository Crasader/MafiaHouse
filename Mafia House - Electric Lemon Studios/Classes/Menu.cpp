#include "Menu.h"

bool MainMenu::init() {
	if (!Scene::init()) { return false; }

	return true;
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
	mainLayer->addChild(selectionIndicator, 2);

	Vec2 optionPosition = Vec2(centre.x, origin.y + visibleSize.y - 600);//horizontal centre, at top of screen
	selectionIndicator->setPosition(optionPosition + Vec2(-400, 0));

	initMenu("menu/levels.txt");
	for (int i = 0; i < levels.size(); i++) {
		levels[i]->setPosition(optionPosition);
		mainLayer->addChild(levels[i], 1);

		optionPosition += Vec2(0, -175);//setting position for next option
	}

	selectedLevel = levels[0];
	selectedOptionNum = 0;

	schedule(schedule_selector(LevelSelectMenu::onStart));

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

	getScene()->getDefaultCamera()->removeFromParentAndCleanup(true);

	getDefaultCamera()->setPosition(centre);

	scheduleUpdate();
}

void LevelSelectMenu::update(float deltaTime) {
	selectedLevel = levels[selectedOptionNum];//setting currently selected menu option

	selectionIndicator->setPosition(selectedLevel->getPosition() + Vec2(-400, 0));//setting position of seletion indicator
	
	for (int i = 0; i < levels.size(); i++) {//increaseing size of selected option
		if (levels[i] == selectedLevel) {
			levels[i]->setScale(1.5);
		}
		else {
			levels[i]->setScale(1);
		}
	}

	if (selectionIndicator->getPositionY() < origin.y) {
		mainLayer->setPosition(getPosition() + Vec2(0, visibleSize.y));
	}
	else if (selectionIndicator->getPositionY() > origin.y + visibleSize.y) {
		mainLayer->setPosition(getPosition() - Vec2(0, visibleSize.y));
	}

	if (INPUTS->getKeyPress(KeyCode::KEY_W)) {//move seletor up
		if (selectedOptionNum >  0) {//number of selected option is greater than 0, first option in menu
			selectedOptionNum--;//go up one option
		}
	}
	else if (INPUTS->getKeyPress(KeyCode::KEY_S)) {//move selector down
		if (selectedOptionNum < levels.size() - 1) {//number of selected option is greater then last option in menu
			selectedOptionNum++;//go down one option
		}
	}
	else if (INPUTS->getKeyPress(KeyCode::KEY_SPACE)) {//select currently selected level
		if (selectedLevel->optionNumber == 1) {
			director->replaceScene(Stage1::createScene());
		}
		else if (selectedLevel->optionNumber == 2) {
			director->replaceScene(Stage2::createScene());
		}
		else if (selectedLevel->optionNumber == 3) {
			director->replaceScene(Stage3::createScene());
		}
	}

	INPUTS->clearForNextFrame();
}
