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
	Vec2 centre = Vec2(origin.x + visibleSize.x / 2, origin.y + visibleSize.y / 2);

	background = Sprite::create("menu/MainMenu");
	background->setScale(visibleSize.x / background->getContentSize().width, visibleSize.y / background->getContentSize().height);
	background->setPosition(centre.x, centre.y);
	addChild(background);

	selectionIndicator = Sprite::create("menu/MenuGun.png");
	addChild(selectionIndicator);

	initMenu("menu/levels.txt");

	Vec2 optionPosition = centre;

	for (int i = 0; i < levels.size(); i++) {
		levels[i]->setPosition(optionPosition);


		optionPosition += Vec2(0, -100);
	}

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
