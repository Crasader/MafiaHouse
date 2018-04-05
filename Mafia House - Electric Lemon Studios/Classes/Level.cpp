#include "Level.h"
#include "Menu.h"

void Level::setup(){
	//loading sprite sheet into sprite frame cache
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("mafiahouse.plist");

	//initialize(preload) sound effects here 
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/equip.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/unequip.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/walk.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/openDoor.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/stairs.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/hide.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/gun_shoot.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/stab.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/swing.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/throw.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/levelcomplete.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/grabbody.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/dropbody.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/jump.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/gameover.wav");



	//node everything in level is attached to
	mainLayer = GameLayer::create();
	mainLayer->setName("mainLayer");
	addChild(mainLayer);

	//node all hud elements are attached
	hudLayer = Node::create();
	addChild(hudLayer);

	healthBar = Sprite::createWithSpriteFrameName("icons/healthBar.png");
	healthBar->getTexture()->setTexParameters(texParams);
	healthFill = Sprite::createWithSpriteFrameName("icons/healthFill.png");
	healthFill->getTexture()->setTexParameters(texParams);
	healthBar->setPosition(Vec2(-400, 250));
	healthFill->setPosition(Vec2(-400, 250));
	healthFill->setColor(ccc3(255, 0, 0));//red
	healthBar->setGlobalZOrder(20);
	healthFill->setGlobalZOrder(20);
	healthBar->setAnchorPoint(Vec2(0, 1));
	healthFill->setAnchorPoint(Vec2(0, 1));
	hudLayer->addChild(healthBar);
	hudLayer->addChild(healthFill);
	playerHead = Sprite::createWithSpriteFrameName("icons/playerHead.png");
	playerHead->getTexture()->setTexParameters(texParams);
	playerHead->setGlobalZOrder(20);
	playerHead->setAnchorPoint(Vec2(0, 1));
	playerHead->setPosition(Vec2(-50, 10) + Vec2(-400, 250));
	hudLayer->addChild(playerHead);
	itemBar = Sprite::createWithSpriteFrameName("icons/healthBar.png");
	itemBar->getTexture()->setTexParameters(texParams);
	itemFill = Sprite::createWithSpriteFrameName("icons/healthFill.png");
	itemFill->getTexture()->setTexParameters(texParams);
	itemFill->setColor(ccc3(0, 0, 255));//blue
	itemBar->setGlobalZOrder(20);
	itemBar->setScaleY(0.5);
	itemBar->setScaleX(0.85);
	itemFill->setGlobalZOrder(20);
	itemFill->setScaleY(0.5);
	itemFill->setScaleX(0.85);
	itemBar->setAnchorPoint(Vec2(0, 1));
	itemFill->setAnchorPoint(Vec2(0, 1));
	itemBar->setPosition(Vec2(30, -20) + Vec2(-400, 250));
	itemFill->setPosition(Vec2(30, -20) + Vec2(-400, 250));
	hudLayer->addChild(itemBar);
	hudLayer->addChild(itemFill);
	itemIcon = Sprite::create();
	itemIcon->getTexture()->setTexParameters(texParams);
	itemIcon->setGlobalZOrder(20);
	itemIcon->setAnchorPoint(Vec2(1, 1));
	itemIcon->setPosition(Vec2(2, -18) + Vec2(-400, 250));
	itemIcon->setRotation(-90);
	hudLayer->addChild(itemIcon);


	//initializing pause screen
	pauseLayer = Node::create();
	addChild(pauseLayer);
	pauseLayer->setVisible(false);
	auto pauseLabel = Label::createWithTTF("PAUSED", "fonts/pixelFJ8pt1__.ttf", 40);
	pauseLabel->getFontAtlas()->setAliasTexParameters();
	pauseLabel->setGlobalZOrder(20);
	pauseLabel->runAction(RepeatForever::create(Blink::create(2.5, 1)));
	pauseLayer->addChild(pauseLabel);
	pauseLabel = Label::createWithTTF("P: Unpause", "fonts/pixelFJ8pt1__.ttf", 18);
	pauseLabel->getFontAtlas()->setAliasTexParameters();
	pauseLabel->setGlobalZOrder(20);
	pauseLabel->setPosition(Vec2(0, -180));
	pauseLayer->addChild(pauseLabel);
	pauseLabel = Label::createWithTTF("SHIFT + R: Restart", "fonts/pixelFJ8pt1__.ttf", 18);
	pauseLabel->getFontAtlas()->setAliasTexParameters();
	pauseLabel->setGlobalZOrder(20);
	pauseLabel->setPosition(Vec2(-275, -239));
	pauseLayer->addChild(pauseLabel);
	pauseLabel = Label::createWithTTF("BACKSPACE: Quit", "fonts/pixelFJ8pt1__.ttf", 18);
	pauseLabel->getFontAtlas()->setAliasTexParameters();
	pauseLabel->setGlobalZOrder(20);
	pauseLabel->setPosition(Vec2(275, -239));
	pauseLayer->addChild(pauseLabel);

	darkenScreen = Sprite::create("backgrounds/black.png");
	darkenScreen->setContentSize(director->getVisibleSize());
	darkenScreen->setOpacity(155);
	darkenScreen->setGlobalZOrder(11);
	pauseLayer->addChild(darkenScreen);

	//Invisible Node for the camera to follow
	camPos = Node::create();
	mainLayer->addChild(camPos);

	srand(time(NULL));

	//necessary for collision detection
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Level::onContactBegin, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

	contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactPreSolve = CC_CALLBACK_2(Level::onContactPreSolve, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

	//for running the update function
	schedule(schedule_selector(Level::onStart));
}

void Level::onStart(float deltaTime){
	unschedule(schedule_selector(Level::onStart));

	//initializing exits
	for (int i = 0; i < doors.size(); i++) {
		if (doors[i]->checkExit() == true) {
			static_cast<Exit*>(doors[i])->initExit();
		}
	}

	//initializing enemy offhand items
	for (int i = 0; i < enemies.size(); i++) {
		enemies[i]->startAnimation(STAND, enemies[i]->stand);
		if (enemies[i]->offhandItem != NULL) {
			enemies[i]->offhandItem->removeFromParent();//removing from main layer
			enemies[i]->offhandItem->initPickedUpItem();
			enemies[i]->offhandItem->initOffhand();
			enemies[i]->addChild(enemies[i]->offhandItem);
			enemies[i]->inventory.push_back(enemies[i]->offhandItem);

			if (enemies[i]->getFlippedX() == true) {
				enemies[i]->offhandItem->knockback *= -1;
			}
			enemies[i]->offhandItem->holderFlipped = enemies[i]->getFlippedX();

			enemies[i]->offhandItem->enemyItem = true;

			if (enemies[i]->offhandItem->isKey == true) {
				enemies[i]->giveKey();//if they pickup a key, they have a key to open locked doors
			}
		}
		if (enemies[i]->itemToPickUp != NULL) {
			enemies[i]->itemToPickUp->removeFromParent();//removing from main layer
			enemies[i]->heldItem = enemies[i]->itemToPickUp;
			enemies[i]->heldItem->initPickedUpItem();
			if (enemies[i]->checkBoss() == true) {
				enemies[i]->heldItem->setPosition(Vec2(57, 36));
			}
			enemies[i]->addChild(enemies[i]->heldItem);
			enemies[i]->inventory.push_back(enemies[i]->heldItem);

			if (enemies[i]->getFlippedX() == true) {
				enemies[i]->heldItem->knockback *= -1;
			}
			enemies[i]->heldItem->holderFlipped = enemies[i]->getFlippedX();

			enemies[i]->heldItem->enemyItem = true;

			if (enemies[i]->heldItem->isKey == true) {
				enemies[i]->giveKey();//if they pickup a key, they have a key to open locked doors
			}
			enemies[i]->itemToPickUp = NULL;
		}
	}

	//initializing missing items
	for (int i = 0; i < mainLayer->items.size(); i++) {
		if (mainLayer->items[i]->getState() == Item::GROUND) {
			mainLayer->items[i]->initMissingItem();
			//mainLayer->items[i]->missingItem->setTag(mainLayer->items[i]->getTag());//giving missing item the same tag as the actual item
			mainLayer->addChild(mainLayer->items[i]->missingItem);
		}
	}

	player->startAnimation(STAND, player->stand);

	getScene()->getPhysicsWorld()->setGravity(Vec2(0, -240));

	getScene()->getPhysicsWorld()->setAutoStep(false);

	//physics debug drawing:
	//getScene()->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

	//deleting layer's default camera, or else there will be a double scene drawn
	getScene()->getDefaultCamera()->removeFromParentAndCleanup(true);
	//getting scene's default camera
	camera = getDefaultCamera();
	//setting 'zoom' level using Z positoin of camera
	camera->setPositionZ(459.42983 / camZoom);

	//scheduling update to start running after this
	scheduleUpdate();
}

void Level::getStats(float deltaTime) {
	unschedule(schedule_selector(Level::getStats));

	//check for achievements
	if (numKilled == 0 && mainLayer->numTimesDetected == 0) {//silent spectre
		silentSpectre = true;
	}
	if (numKilled == numEnemies && mainLayer->numTimesDetected == 0) {//full assassin
		fullAssassin = true;
	}

	//reading in level fime
	std::ifstream inFile;
	inFile.open("menu/levels.txt");

	vector<vector<string>> levels;

	string line;
	while (getline(inFile, line)) {//each interation gets a single line from the file; gets data for a single room
		if (line[0] == '#') { continue; }//ignoring comment lines, which begin with the '#' character

		//getting single pieces of data:
		vector<string> pieces;
		std::istringstream sstream(line);//putting line into a string stream
		string piece;
		while (getline(sstream, piece, ',')) {//each iteration gets a piece of the chunck, delmimited by the ',' character; gets a single piece of data for a component
			pieces.push_back(piece);
		}

		levels.push_back(pieces);
	}
	inFile.close();

	//setting level complete to true
	levels[levelNum][1] = "1";
	//checking if achievemnts were met
	if (fullAssassin == true) {
		levels[levelNum][2] = "1";
	}
	if (silentSpectre == true) {
		levels[levelNum][3] = "1";
	}
	//checking if best time was beaten
	if (gameTime < atof(levels[levelNum][4].c_str()) || atof(levels[levelNum][4].c_str()) <= 0) {
		gotBestTime = true;
		std::stringstream stream;
		stream << std::fixed << std::setprecision(3) << gameTime;
		levels[levelNum][4] = stream.str();
	}

	//writing to level file
	std::ofstream outFile;
	outFile.open("menu/levels.txt");

	for (int i = 0; i < levels.size(); i++) {
		for (int j = 0; j < levels[i].size(); j++) {
			outFile << levels[i][j];
			if (j < levels[i].size() - 1) {
				outFile << ',';
			}
			else {
				outFile << '\n';
			}
		}
	}
	outFile.close();

	completionScreen = Sprite::create("menu/completionLogo.png");
	completionScreen->getTexture()->setTexParameters(texParams);
	completionScreen->setGlobalZOrder(30);
	completionScreen->setPosition(Vec2(0, 100));
	completionScreen->runAction(MoveBy::create(2.0f,Vec2(0, 0)))->setTag(1);
	hudLayer->addChild(completionScreen);

	levelFinishTime = gameTime;
	std::stringstream stream;
	stream << std::fixed << std::setprecision(3) << levelFinishTime;

	completeTimeDisplay = Label::createWithTTF("COMPLETION TIME: " + stream.str(), "fonts/pixelFJ8pt1__.ttf", 20);
	completeTimeDisplay->getFontAtlas()->setAliasTexParameters();
	completeTimeDisplay->setGlobalZOrder(30);
	completeTimeDisplay->setPosition(Vec2(0, -90));
	completeTimeDisplay->setVisible(false);
	hudLayer->addChild(completeTimeDisplay);

	if (gotBestTime == true) {
		bestTime = Sprite::create("menu/bestTime.png");
		bestTime->getTexture()->setTexParameters(texParams);
		bestTime->setGlobalZOrder(30);
		bestTime->setPosition(Vec2(192, -70));
		bestTime->setVisible(false);
		hudLayer->addChild(bestTime);
	}

	numKilledDisplay = Label::createWithTTF("ENEMIES KILLED: " + std::to_string(numKilled), "fonts/pixelFJ8pt1__.ttf", 20);
	numKilledDisplay->getFontAtlas()->setAliasTexParameters();
	numKilledDisplay->setGlobalZOrder(30);
	numKilledDisplay->setPosition(Vec2(0, -140));
	numKilledDisplay->setVisible(false);
	hudLayer->addChild(numKilledDisplay);

	numTimesDetectedDisplay = Label::createWithTTF("TIMES DETECTED: " + std::to_string(mainLayer->numTimesDetected), "fonts/pixelFJ8pt1__.ttf", 20);
	numTimesDetectedDisplay->getFontAtlas()->setAliasTexParameters();
	numTimesDetectedDisplay->setGlobalZOrder(30);
	numTimesDetectedDisplay->setPosition(Vec2(0, -190));
	numTimesDetectedDisplay->setVisible(false);
	hudLayer->addChild(numTimesDetectedDisplay);

	fullAssassinLabel = Label::createWithTTF("KILL ALL ENEMIES\nWITHOUT BEING\nDETECTED: ", "fonts/pixelFJ8pt1__.ttf", 10);
	fullAssassinLabel->getFontAtlas()->setAliasTexParameters();
	fullAssassinLabel->setGlobalZOrder(30);
	fullAssassinLabel->setPosition(Vec2(-300, -100));
	fullAssassinLabel->setVisible(false);
	hudLayer->addChild(fullAssassinLabel);
	silentSpectreLabel = Label::createWithTTF("KILL NO ENEMIES\nWITHOUT BEING\nDETECTED: ", "fonts/pixelFJ8pt1__.ttf", 10);
	silentSpectreLabel->getFontAtlas()->setAliasTexParameters();
	silentSpectreLabel->setGlobalZOrder(30);
	silentSpectreLabel->setPosition(Vec2(300, -100));
	silentSpectreLabel->setVisible(false);
	hudLayer->addChild(silentSpectreLabel);

	if (fullAssassin == true) {
		achievement1 = Sprite::create("menu/achievement1.png");
		achievement1->getTexture()->setTexParameters(texParams);
		achievement1->setGlobalZOrder(30);
		achievement1->setPosition(Vec2(-300, -160));
		achievement1->setVisible(false);
		hudLayer->addChild(achievement1);
	}
	if (silentSpectre == true) {
		achievement2 = Sprite::create("menu/achievement2.png");
		achievement2->getTexture()->setTexParameters(texParams);
		achievement2->setGlobalZOrder(30);
		achievement2->setPosition(Vec2(300, -160));
		achievement2->setVisible(false);
		hudLayer->addChild(achievement2);
	}

	continueLabel = Label::createWithTTF("PRESS SPACE TO CONTINUE", "fonts/pixelFJ8pt1__.ttf", 20);
	continueLabel->getFontAtlas()->setAliasTexParameters();
	continueLabel->setGlobalZOrder(30);
	continueLabel->setPosition(Vec2(0, -240));
	continueLabel->setVisible(false);
	hudLayer->addChild(continueLabel);

	darkenScreen = Sprite::create("backgrounds/black.png");
	darkenScreen->setContentSize(director->getVisibleSize());
	darkenScreen->setOpacity(155);
	darkenScreen->setGlobalZOrder(11);
	hudLayer->addChild(darkenScreen);

	runAction(MoveBy::create(0.6f, Vec2(0, 0)))->setTag(0);
	runAction(MoveBy::create(1.2f,Vec2(0, 0)))->setTag(1);
	runAction(MoveBy::create(1.8f, Vec2(0, 0)))->setTag(2);
	runAction(MoveBy::create(2.4f, Vec2(0, 0)))->setTag(3);
	runAction(MoveBy::create(3.0f, Vec2(0, 0)))->setTag(4);
	runAction(MoveBy::create(2.4f, Vec2(0, 0)))->setTag(5);

	schedule(schedule_selector(Level::onEnd));
}

void Level::onEnd(float deltaTime) {
	gameTime += deltaTime;
	//Display Level Completion Screen

	if (getActionByTag(0) == NULL) {
		completeTimeDisplay->setVisible(true);
	}
	if (getActionByTag(1) == NULL) {
		numKilledDisplay->setVisible(true);
	}
	if (getActionByTag(2) == NULL) {
		numTimesDetectedDisplay->setVisible(true);
	}
	if (getActionByTag(3) == NULL) {
		fullAssassinLabel->setVisible(true);
		silentSpectreLabel->setVisible(true);
	}
	if (getActionByTag(4) == NULL) {
		if (gotBestTime == true) {
			bestTime->setVisible(true);
		}
		if (fullAssassin == true) {
			achievement1->setVisible(true);
		}
		if (silentSpectre == true) {
			achievement2->setVisible(true);
		}
	}
	if (getActionByTag(5) == NULL) {
		if (continueLabel->getActionByTag(10) == NULL) {
			continueLabel->runAction(RepeatForever::create(Blink::create(2.2, 1)))->setTag(10);
		}
	}
	if (INPUTS->getKey(KeyCode::KEY_SPACE)) {//skip stats showing up individually
		stopAllActions();
		levelFinishTime = -2;
	}

	if (gameTime - levelFinishTime >= completeScreenDisplayTime) {
		if (INPUTS->getKey(KeyCode::KEY_SPACE)) {//finish the level
			startNextLevel();
		}
		else if ((INPUTS->getKey(KeyCode::KEY_SHIFT)) && (INPUTS->getKey(KeyCode::KEY_R))) {//redo level
			resetLevel();
		}
		else if (INPUTS->getKey(KeyCode::KEY_BACKSPACE)) {//redo level
			director->replaceScene(LevelSelectMenu::createScene());
		}
	}
	INPUTS->clearForNextFrame();
}

void Level::pauseScreen(float deltaTime) {
	if (INPUTS->getKeyPress(KeyCode::KEY_P)) {//resume game
		unschedule(schedule_selector(Level::pauseScreen));
		getScene()->getPhysicsWorld()->setSpeed(1);
		scheduleUpdate();
		pauseLayer->setVisible(false);
		camera->resume();
		player->resume();
		for (int i = 0; i < mainLayer->items.size(); i++) {
			mainLayer->items[i]->resume();
		}
		for (int i = 0; i < enemies.size(); i++) {
			enemies[i]->resume();
		}
	}
	else if (INPUTS->getKey(KeyCode::KEY_SHIFT) && INPUTS->getKey(KeyCode::KEY_R)) {//restart the level
		resetLevel();
	}
	else if (INPUTS->getKey(KeyCode::KEY_BACKSPACE)) {//quit to level select
		director->replaceScene(LevelSelectMenu::createScene());
	}
	INPUTS->clearForNextFrame();
}

void Level::gameOver(float deltaTime) {
	if (initGameOver == false) {
		gameOverScreen = Sprite::create("menu/gameOver.png");
		gameOverScreen->getTexture()->setTexParameters(texParams);
		gameOverScreen->setGlobalZOrder(12);
		gameOverScreen->setOpacity(0);
		gameOverScreen->runAction(FadeIn::create(4.0f))->setTag(1);
		hudLayer->addChild(gameOverScreen);
		darkenScreen = Sprite::create("backgrounds/black.png");
		darkenScreen->setContentSize(director->getVisibleSize());
		darkenScreen->setOpacity(155);
		darkenScreen->setGlobalZOrder(11);
		hudLayer->addChild(darkenScreen);
		initGameOver = true;
	}
	if (gameOverScreen->getActionByTag(1) == NULL && initGameOverLabels == false) {
		auto pauseLabel = Label::createWithTTF("SPACE: Retry Level", "fonts/pixelFJ8pt1__.ttf", 22);
		pauseLabel->getFontAtlas()->setAliasTexParameters();
		pauseLabel->setGlobalZOrder(20);
		pauseLabel->setPosition(Vec2(0, -190));
		hudLayer->addChild(pauseLabel);
		pauseLabel = Label::createWithTTF("BACKSPACE: Quit", "fonts/pixelFJ8pt1__.ttf", 22);
		pauseLabel->getFontAtlas()->setAliasTexParameters();
		pauseLabel->setGlobalZOrder(20);
		pauseLabel->setPosition(Vec2(0, -240));
		hudLayer->addChild(pauseLabel);
		initGameOverLabels = true;
	}
	if (initGameOverLabels == true) {
		if (INPUTS->getKey(KeyCode::KEY_SPACE)) {//restart level
			resetLevel();
		}
		else if (INPUTS->getKey(KeyCode::KEY_BACKSPACE)) {//quit to main menu
			director->replaceScene(LevelSelectMenu::createScene());
		}
	}
	INPUTS->clearForNextFrame();
}

void Level::update(float deltaTime){
	//updating time
	gameTime += deltaTime;
	getScene()->getPhysicsWorld()->step(deltaTime);

	if (player->isReallyDead() == true) {//player died, gameover
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/gameover.wav");
		unscheduleUpdate();
		getScene()->getPhysicsWorld()->setSpeed(0);
		camera->pause();
		player->pause();
		for (int i = 0; i < mainLayer->items.size(); i++) {
			mainLayer->items[i]->pause();
		}
		for (int i = 0; i < enemies.size(); i++) {
			enemies[i]->pause();
		}
		schedule(schedule_selector(Level::gameOver));
	}

	if (levelComplete == true) {//finish level
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/levelcomplete.wav");
		unscheduleUpdate();
		getScene()->getPhysicsWorld()->setSpeed(0);
		schedule(schedule_selector(Level::pauseScreen));
		camera->pause();
		player->pause();
		for (int i = 0; i < mainLayer->items.size(); i++) {
			mainLayer->items[i]->pause();
		}
		for (int i = 0; i < enemies.size(); i++) {
			enemies[i]->pause();
		}
		schedule(schedule_selector(Level::getStats));
	}

	//camera zoom button
	if (INPUTS->getKeyPress(KeyCode::KEY_CTRL)) {
		camera->stopAllActions();
		auto zoomOut = MoveTo::create(1.0, Vec3(0, 0, 459.42983 / 0.4));
		camera->runAction(zoomOut);
	}
	else if (INPUTS->getKeyRelease(KeyCode::KEY_CTRL)) {
		camera->stopAllActions();
		auto zoomIn = MoveTo::create(1.0, Vec3(0, 0, 459.42983 / 1.0));
		camera->runAction(zoomIn);
	}

	//for drawing vision rays
	if (noiseCircles) {
		removeChild(noiseCircles, true);
	}
	noiseCircles = DrawNode::create();
	noiseCircles->setGlobalZOrder(9);
	//noise update
	for (int i = 0; i < mainLayer->noises.size(); i++) {
		if (gameTime - mainLayer->noises[i]->startTime >= mainLayer->noises[i]->duration) {//noise has expired
			mainLayer->noises[i]->removeFromParent();//removing noise from game world
			mainLayer->noises.erase(mainLayer->noises.begin() + i);//removing noise from vector of noises
			i--;
			continue;
		}
		float percentage = (gameTime - mainLayer->noises[i]->startTime) / mainLayer->noises[i]->duration;//percentage of duration finished
		float percentage1 = percentage * 1.4 >= 1 ? 1 : percentage * 1.4;
		float percentage2 = percentage * 1 >= 1 ? 1 : percentage * 1;
		float percentage3 = percentage * 0.7 >= 1 ? 1 : percentage * 0.7;
		noiseCircles->drawCircle(mainLayer->noises[i]->getPosition(), mainLayer->noises[i]->radius * percentage1, 0, 60, false, Color4F(1, 1, 1, abs(percentage1 - 1)));
		noiseCircles->drawCircle(mainLayer->noises[i]->getPosition(), mainLayer->noises[i]->radius * percentage2, 0, 60, false, Color4F(1, 1, 1, abs(percentage2 - 1)));
		noiseCircles->drawCircle(mainLayer->noises[i]->getPosition(), mainLayer->noises[i]->radius * percentage3, 0, 60, false, Color4F(1, 1, 1, abs(percentage3 - 1)));
	}
	addChild(noiseCircles);

	//hide object update
	for (int i = 0; i < hideObjects.size(); i++) {
		hideObjects[i]->playerInRange();
	}
	//stair update
	for (int i = 0; i < mainLayer->stairs.size(); i++) {
		mainLayer->stairs[i]->playerInRange();
	}
	//door update
	for (int i = 0; i < doors.size(); i++) {
		doors[i]->updateBroken();
		doors[i]->updateColour();
		doors[i]->playerInRange();
		if (doors[i]->noiseLevel != -1) {
			doors[i]->createNoise(doors[i]->noiseLevel, doors[i]->noiseLevel / 100, gameTime, doors[i]->getPosition() + doors[i]->getContentSize() / 2, doors[i]->roomHitFrom, "door_being_hit", &mainLayer->noises);
			doors[i]->noiseLevel = -1;
		}
		if (doors[i]->checkExit() == true) {
			if (numBosses <= 0) {//all bosses have been killed
				static_cast<Exit*>(doors[i])->canOpen = true;//activate level exits
				static_cast<Exit*>(doors[i])->unlock();
			}
		}
	}

	//for drawing gun shots
	if (gunShots) {
		removeChild(gunShots, true);
	}
	gunShots = DrawNode::create();
	gunShots->setGlobalZOrder(7);
	//items update
	for (int i = 0; i < mainLayer->items.size(); i++) {
		if (mainLayer->items[i]->getState() != Item::HELD) {
			if (mainLayer->items[i]->checkBroken() == true) {
				mainLayer->items.erase(mainLayer->items.begin() + i);
				i--;
				continue;
			}
		}
		if (mainLayer->items[i]->getState() != Item::HELD) {
			mainLayer->items[i]->updateFloor(mainLayer->floors);
			mainLayer->items[i]->updateRoom(mainLayer->floors[mainLayer->items[i]->currentFloor].rooms);
		}
		else if (mainLayer->items[i]->getState() == Item::HELD) {//updating room/foor for held items
			mainLayer->items[i]->updateHeldItemFloor(mainLayer->floors);
			mainLayer->items[i]->updateHeldItemRoom(mainLayer->floors[mainLayer->items[i]->currentFloor].rooms);
		}
		if (mainLayer->items[i]->makeNoise == true){
			if (mainLayer->items[i]->getState() != Item::HELD) {
				mainLayer->items[i]->createNoise(mainLayer->items[i]->noiseLevel * mainLayer->items[i]->getPhysicsBody()->getVelocity().getLength(), mainLayer->items[i]->noiseLevel, gameTime, mainLayer->items[i]->getPosition(), Vec2(mainLayer->items[i]->currentRoom, mainLayer->items[i]->currentFloor), "item_hitting_wall", &mainLayer->noises);
			}
			else {
				if (mainLayer->items[i]->holderFlipped == false) {
					mainLayer->items[i]->createNoise(mainLayer->items[i]->noiseLevel * 100, mainLayer->items[i]->noiseLevel, gameTime, mainLayer->items[i]->convertToWorldSpace(getPosition()) + mainLayer->items[i]->getContentSize(), Vec2(mainLayer->items[i]->currentRoom, mainLayer->items[i]->currentFloor), "item_hitting_wall", &mainLayer->noises);
				}
				else {
					mainLayer->items[i]->createNoise(mainLayer->items[i]->noiseLevel * 100, mainLayer->items[i]->noiseLevel, gameTime, mainLayer->items[i]->convertToWorldSpace(getPosition()) + Vec2(-mainLayer->items[i]->getContentSize().width, mainLayer->items[i]->getContentSize().height), Vec2(mainLayer->items[i]->currentRoom, mainLayer->items[i]->currentFloor), "item_hitting_wall", &mainLayer->noises);
				}
			}
			mainLayer->items[i]->makeNoise = false;
		}
		//for guns being shot
		if (mainLayer->items[i]->getAttackType() == Item::SHOOT && mainLayer->items[i]->wasShot == true) {
			mainLayer->items[i]->wasShot = false;
			mainLayer->items[i]->shotTime = gameTime;
			mainLayer->items[i]->createNoise(mainLayer->items[i]->noiseLevel * 900, (mainLayer->items[i]->noiseLevel * 5) - 1, gameTime, mainLayer->items[i]->startpoint, Vec2(mainLayer->items[i]->currentRoom, mainLayer->items[i]->currentFloor), "gunshot", &mainLayer->noises);
			mainLayer->items[i]->createNoise(mainLayer->items[i]->noiseLevel * 900, ((mainLayer->items[i]->noiseLevel * 5) - 1) / 2, gameTime, mainLayer->items[i]->startpoint, Vec2(mainLayer->items[i]->currentRoom, mainLayer->items[i]->currentFloor), "gunshot", &mainLayer->noises);
		}
		if (mainLayer->items[i]->shotTime != -1 && gameTime - mainLayer->items[i]->shotTime <= 0.5f) {//time to display gunshot for
			gunShots->drawSegment(mainLayer->items[i]->startpoint, mainLayer->items[i]->endpoint, 1, Color4F(1, 1, 1, 1));
		}
	}
	addChild(gunShots);

	//dead bodies update
	for (int i = 0; i < mainLayer->bodies.size(); i++) {
		mainLayer->bodies[i]->updateFloor(mainLayer->floors);
		mainLayer->bodies[i]->updateRoom(mainLayer->floors[mainLayer->bodies[i]->currentFloor].rooms);
		if (mainLayer->bodies[i]->makeNoise == true) {
			if (mainLayer->bodies[i]->getFlippedX() == false) {
				mainLayer->bodies[i]->createNoise(mainLayer->bodies[i]->noiseLevel * mainLayer->bodies[i]->getPhysicsBody()->getVelocity().getLength(), mainLayer->bodies[i]->noiseLevel, gameTime, mainLayer->bodies[i]->getPosition() + Vec2(mainLayer->bodies[i]->getContentSize().width, 0), Vec2(mainLayer->bodies[i]->currentRoom, mainLayer->bodies[i]->currentFloor), "body_hitting_wall", &mainLayer->noises);
			}
			else {
				mainLayer->bodies[i]->createNoise(mainLayer->bodies[i]->noiseLevel * mainLayer->bodies[i]->getPhysicsBody()->getVelocity().getLength(), mainLayer->bodies[i]->noiseLevel, gameTime, mainLayer->bodies[i]->getPosition() - Vec2(mainLayer->bodies[i]->getContentSize().width, 0), Vec2(mainLayer->bodies[i]->currentRoom, mainLayer->bodies[i]->currentFloor), "body_hitting_wall", &mainLayer->noises);
			}
			mainLayer->bodies[i]->makeNoise = false;
		}
	}

	//for drawing vision rays
	if (visionRays) {
		removeChild(visionRays, true);
	}
	visionRays = DrawNode::create();
	visionRays->setGlobalZOrder(8);
	//enemy update
	vector<Vec2> points;
	Vec2 start;
	//removing dead enemies
	for (int i = 0; i < enemies.size(); i++) {
		if (enemies.at(i)->isReallyDead() == true) {
			if (enemies[i]->checkBoss() == true) {
				numBosses--;//you killed a boss
				Label* bossKillLabel;
				if (numBosses <= 0) {
					bossKillLabel = Label::createWithTTF("MISSION COMPLETE. RETURN TO EXIT.", "fonts/pixelFJ8pt1__.ttf", 10);
				}
				else {
					bossKillLabel = Label::createWithTTF("BOSS KILLED. " + std::to_string(numBosses) + " LEFT TO KILL.", "fonts/pixelFJ8pt1__.ttf", 10);
				}
				bossKillLabel->getFontAtlas()->setAliasTexParameters();
				bossKillLabel->setGlobalZOrder(20);
				addChild(bossKillLabel);
				bossKillLabel->setOpacity(0);
				bossKillLabel->setPosition(player->getPosition() + Vec2(0, 90));
				auto fadeIn = FadeIn::create(0.5);
				auto moveUp = MoveBy::create(5.0, Vec2(0, 35));
				auto wait = MoveBy::create(2.5, Vec2(0, 0));
				auto fadeOut = FadeOut::create(2.0);
				auto callback = CallFunc::create([bossKillLabel]() {
					bossKillLabel->removeFromParent();
				});
				bossKillLabel->runAction(Sequence::create(fadeIn, wait, fadeOut, callback, nullptr));
				bossKillLabel->runAction(moveUp);
			}
			else {
				numKilled++;
			}
			enemies.at(i)->removeFromParentAndCleanup(true);
			enemies.erase(enemies.begin() + i);
			i--;
			continue;
		}
	}
	for (int i = 0; i < enemies.size(); i++) {
		enemies[i]->enemies = enemies;//giving each enemy the current list of enemies in level

		enemies[i]->update(mainLayer, gameTime);

		//enemy vision:
		enemies[i]->visionRays(&points, &start, gameTime);
		//drawing vision rays
		for (int j = 0; j < points.size(); j++) {
			//visionRays->drawDot(points[j], 1, Color4F::WHITE);
			visionRays->drawSegment(start, points[j], 2.2, Color4F(1, 0.9, 0.1, 0.2));
		}
		points.clear();
	}
	addChild(visionRays);

	//player update:
	player->update(mainLayer, gameTime);

	//check if player is going to interact with door/stairway, pick up item, hide behind object
	if (INPUTS->getKeyPress(KeyCode::KEY_E)) {
		if (player->isHidden() == false) {
			if ((player->itemToPickUp != NULL && player->heldItem == NULL) || player->bodyToPickUp != NULL) {
				player->handleInput(mainLayer, gameTime, PICKUP);
			}
			else {
				if (player->doorToUse != NULL) {
					player->handleInput(mainLayer, gameTime, USE_DOOR);
				}
				else if (player->stairToUse != NULL) {
					player->handleInput(mainLayer, gameTime, USE_STAIR);
				}
				else if (player->objectToHideBehind != NULL) {
					player->handleInput(mainLayer, gameTime, HIDE);
				}
			}
		}
		else {//player is hidden
			if (player->objectToHideBehind != NULL) {
				player->handleInput(mainLayer, gameTime, HIDE);
			}
			else if ((player->itemToPickUp != NULL && player->heldItem == NULL) || player->bodyToPickUp != NULL) {
				player->handleInput(mainLayer, gameTime, PICKUP);
			}
		}
	}
	//check if player is using item
	if (INPUTS->getKeyPress(KeyCode::KEY_SPACE)) {
		startPressTime = -1;
		if (player->heldItem != NULL) {
			player->handleInput(mainLayer, gameTime, USE_ITEM);
		}
	}
	if (INPUTS->getKeyRelease(KeyCode::KEY_SPACE)) {
		if (player->heldItem != NULL) {
			player->handleInput(mainLayer, gameTime, USE_RELEASE);
		}
	}
	//check if player is throwing item
	if (INPUTS->getKeyPress(KeyCode::KEY_SHIFT)) {
		if (startPressTime == -1) {
			startPressTime = gameTime;
		}
	}
	if (INPUTS->getKey(KeyCode::KEY_SHIFT)) {
		if (player->heldItem != NULL || player->heldBody != NULL) {
			if (startPressTime != -1 && gameTime - startPressTime >= holdTime) {//button has been held for a certain amount of time
				player->handleInput(mainLayer, gameTime, THROW_ITEM);
			}
		}
	}
	if (INPUTS->getKeyRelease(KeyCode::KEY_SHIFT)) {
		if (player->heldItem != NULL || player->heldBody != NULL) {
			if (gameTime - startPressTime >= holdTime) {//button was been held for a certain amount of time
				player->handleInput(mainLayer, gameTime, THROW_RELEASE);
			}
			else {
				player->handleInput(mainLayer, gameTime, DROP);//button was not held for minimum hold time
			}
		}
		startPressTime = -1;
	}

	//player movement input checking:
	if (INPUTS->getKey(KeyCode::KEY_D)) {
		player->handleInput(mainLayer, gameTime, MOVE_RIGHT);
	}
	if (INPUTS->getKey(KeyCode::KEY_A)) {
		player->handleInput(mainLayer, gameTime, MOVE_LEFT);
	}
	if (INPUTS->getKeyRelease(KeyCode::KEY_D) || INPUTS->getKeyRelease(KeyCode::KEY_A)) {
		player->handleInput(mainLayer, gameTime, STOP);
	}
	//player roll input checking:
	if (INPUTS->getKeyPress(KeyCode::KEY_D)) {
		if (gameTime - prevLeftPressTime <= doublePressTime) {//pressed key again with time limit
			player->handleInput(mainLayer, gameTime, ROLL);
		}
		prevLeftPressTime = gameTime;
	}
	if (INPUTS->getKeyPress(KeyCode::KEY_A)) {
		if (gameTime - prevRightPressTime <= doublePressTime) {//pressed key again with time limit
			player->handleInput(mainLayer, gameTime, ROLL);
		}
		prevRightPressTime = gameTime;
	}

	//player aiming input checking:
	if (INPUTS->getKey(KeyCode::KEY_W) && INPUTS->getKey(KeyCode::KEY_D)) {
		player->handleInput(mainLayer, gameTime, AIM_UP_RIGHT);
	}
	else if (INPUTS->getKey(KeyCode::KEY_W) && INPUTS->getKey(KeyCode::KEY_A)) {
		player->handleInput(mainLayer, gameTime, AIM_UP_LEFT);
	}
	else if (INPUTS->getKey(KeyCode::KEY_S) && INPUTS->getKey(KeyCode::KEY_D)) {
		player->handleInput(mainLayer, gameTime, AIM_DOWN_RIGHT);
	}
	else if (INPUTS->getKey(KeyCode::KEY_S) && INPUTS->getKey(KeyCode::KEY_A)) {
		player->handleInput(mainLayer, gameTime, AIM_DOWN_LEFT);
	}
	else if (INPUTS->getKey(KeyCode::KEY_D)) {
		player->handleInput(mainLayer, gameTime, AIM_RIGHT);
	}
	else if (INPUTS->getKey(KeyCode::KEY_A)) {
		player->handleInput(mainLayer, gameTime, AIM_LEFT);
	}
	else if (INPUTS->getKey(KeyCode::KEY_W)) {
		player->handleInput(mainLayer, gameTime, AIM_UP);
	}
	else if (INPUTS->getKey(KeyCode::KEY_S)) {
		player->handleInput(mainLayer, gameTime, AIM_DOWN);
	}

	//change between standing/crouching
	if (INPUTS->getKeyPress(KeyCode::KEY_W)) {
		player->handleInput(mainLayer, gameTime, MOVE_UP);
	}
	if (INPUTS->getKeyPress(KeyCode::KEY_S)) {
		player->handleInput(mainLayer, gameTime, MOVE_DOWN);
	}

	//for testing purposes only, do not abuse:
	if (INPUTS->getKeyPress(KeyCode::KEY_N)) {
		player->handleInput(mainLayer, gameTime, NO_CLIP);
	}
	//for flying only
	if (player->noclip == true) {
		if (INPUTS->getKey(KeyCode::KEY_W)) {
			player->handleInput(mainLayer, gameTime, MOVE_UP);
		}
		if (INPUTS->getKey(KeyCode::KEY_S)) {
			player->handleInput(mainLayer, gameTime, MOVE_DOWN);
		}
	}

	//must be called after checking all player actions
	player->resetCollisionChecks(gameTime);
	//reseting item collision check with hide radii
	for (int i = 0; i < mainLayer->items.size(); i++) {
		mainLayer->items[i]->isUnderObject = false;
		mainLayer->items[i]->didHitWall = false;
		if (mainLayer->items[i]->getState() == Item::GROUND) {
			mainLayer->items[i]->hasMoved();
			mainLayer->items[i]->playerInRange(player);
			mainLayer->items[i]->checkGroundSpeed();
		}
		else if (mainLayer->items[i]->getState() == Item::HELD) {
			mainLayer->items[i]->stealRange(player);
		}
		else if (mainLayer->items[i]->getState() == Item::THROWN) {
			mainLayer->items[i]->checkThrownSpeed();
			if (mainLayer->items[i]->getAttackType() == Item::SWING || mainLayer->items[i]->getAttackType() == Item::SHOOT) {
				mainLayer->items[i]->spin();
			}
		}
		else if (mainLayer->items[i]->getState() == Item::FALLING) {
			mainLayer->items[i]->checkFallingSpeed();
		}
	}
	//dead bodies update
	for (int i = 0; i < mainLayer->bodies.size(); i++) {
		if (mainLayer->bodies[i]->getState() == Item::GROUND) {
			mainLayer->bodies[i]->playerInRange(player);
			mainLayer->bodies[i]->checkGroundSpeed();
		}
		if (mainLayer->bodies[i]->getState() == Item::THROWN) {
			mainLayer->bodies[i]->checkThrownSpeed();
		}
	}

	//camOffset = Vec2(0, 150 / camZoom);//adjusting camera offset with zoom level?
	//having camera follow player
	followBox(camPos, player, camBoundingBox, camOffset);
	camera->setPosition(camPos->getPosition());
	hudLayer->setPosition(camera->getPosition());//make it so hud stays in same location on screen
	hudLayer->setPositionZ(camera->getPositionZ() - 459.42983);
	float percentage = player->getHP() / player->getMaxHP();
	healthFill->setScaleX(percentage);
	if (player->heldItem != NULL) {
		itemIcon->setSpriteFrame(player->heldItem->getSpriteFrame());
		itemBar->setVisible(true);
		itemFill->setVisible(true);
		itemIcon->setVisible(true);
		float percentage = player->heldItem->hp / player->heldItem->maxHP;
		itemFill->setScaleX(percentage * 0.85);
	}
	else {
		itemBar->setVisible(false);
		itemFill->setVisible(false);
		itemIcon->setVisible(false);
	}

	//pausing game
	if (INPUTS->getKeyPress(KeyCode::KEY_P)) {
		unscheduleUpdate();
		getScene()->getPhysicsWorld()->setSpeed(0);
		pauseLayer->setPositionZ(camera->getPositionZ() - 459.42983);
		pauseLayer->setPosition(camera->getPosition());
		pauseLayer->setVisible(true);
		camera->pause();
		player->pause();
		for (int i = 0; i < mainLayer->items.size(); i++) {
			mainLayer->items[i]->pause();
		}
		for (int i = 0; i < enemies.size(); i++) {
			enemies[i]->pause();
		}
		schedule(schedule_selector(Level::pauseScreen));
	}

	//update the keyboard each frame
	INPUTS->clearForNextFrame();
}

//check if 2 objects are touching, do something and set whether they collide or not after
bool Level::onContactPreSolve(PhysicsContact &contact, PhysicsContactPreSolve & solve) {
	Node *a = contact.getShapeA()->getBody()->getNode();
	Node *b = contact.getShapeB()->getBody()->getNode();

	if (a != NULL && b != NULL) {
		//player and exit hitbox
		if (a->getName() == "player" && b->getName() == "level_exit")
		{
			levelComplete = true;
			return false;
		}
		else if (a->getName() == "level_exit" && b->getName() == "player")
		{
			levelComplete = true;
			return false;
		}
		//player and exit door
		if (a->getName() == "player" && b->getName() == "exit_radius")
		{
			player->doorToUse = static_cast<Door*>(b->getParent());
			static_cast<Door*>(b->getParent())->playerRange = true;
			return false;
		}
		else if (a->getName() == "exit_radius" && b->getName() == "player")
		{
			player->doorToUse = static_cast<Door*>(a->getParent());
			static_cast<Door*>(a->getParent())->playerRange = true;
			return false;
		}
		//player and noises
		if (a->getName() == "player" && b->getName() == "noise")
		{
			return false;
		}
		else if (a->getName() == "noise" && b->getName() == "player")
		{
			return false;
		}

		//enemy and noises
		if ((a->getName() == "enemy" || a->getName() == "enemy_alert") && b->getName() == "noise")
		{
			return false;
		}
		else if (a->getName() == "noise" && (b->getName() == "enemy" || b->getName() == "enemy_alert"))
		{
			return false;
		}

		//player and physical object
		if (a->getName() == "player" && b->getName() == "phys_object")
		{
			if (player->getPositionY() >= (static_cast<PhysObject*>(b)->getPositionY() + static_cast<PhysObject*>(b)->surfaceHeight - 5)) {//only collide if player is above object
				player->touchingFloor = true;
				solve.setRestitution(0.0f);
				return true;
			}
			else {
				player->objectToClimb = static_cast<PhysObject*>(b);
				return false;
			}
		}
		else if (a->getName() == "phys_object" && b->getName() == "player")
		{
			if (player->getPositionY() >= (static_cast<PhysObject*>(a)->getPositionY() + static_cast<PhysObject*>(a)->surfaceHeight - 5)) {//only collide if player is above object
				player->touchingFloor = true;
				solve.setRestitution(0.0f);
				return true;
			}
			else {
				player->objectToClimb = static_cast<PhysObject*>(a);
				return false;
			}
		}

		// check if player is touching floor
		if ((a->getName() == "player" && b->getName() == "floor") || (a->getName() == "floor" && b->getName() == "player"))
		{
			player->touchingFloor = true;
			solve.setRestitution(0.0f);
			return true;
		}

		//check if player is hiding under a physical object
		if (a->getName() == "player" && b->getName() == "hide_radius")
		{
			if (player->isCrouched == true) {
				player->isHidingUnder = true;
				//static_cast<Item*>(b->getParent())->playerRange = true;
			}
			return false;
		}
		else if (a->getName() == "hide_radius" && b->getName() == "player")
		{
			if (player->isCrouched == true) {
				player->isHidingUnder = true;
				//static_cast<Item*>(b->getParent())->playerRange = true;
			}
			return false;
		}

		//item and hide radius
		if (a->getName() == "held_item" && b->getName() == "hide_radius")
		{
			if (static_cast<Item*>(a)->getAttackType() != Item::SHOOT) {
				static_cast<Item*>(a)->isUnderObject = true;
			}
			return false;
		}
		else if (a->getName() == "hide_radius" && b->getName() == "held_item")
		{
			if (static_cast<Item*>(b)->getAttackType() != Item::SHOOT) {
				static_cast<Item*>(b)->isUnderObject = true;
			}
			return false;
		}

		// check if item has collided with celing
		if (a->getName() == "held_item" && (b->getName() == "ceiling" || b->getName() == "vent"))
		{
			if (static_cast<Item*>(a)->getState() == Item::THROWN || static_cast<Item*>(a)->getState() == Item::FALLING) {
				//solve.setFriction(0);
				solve.setRestitution(0.45);
				return true;
			}
			else {
				static_cast<Item*>(a)->hitWall();
				return true;
			}
		}
		else if ((a->getName() == "ceiling" || a->getName() == "vent") && b->getName() == "held_item") {

			if (static_cast<Item*>(b)->getState() == Item::THROWN || static_cast<Item*>(a)->getState() == Item::FALLING) {
				//solve.setFriction(0);
				solve.setRestitution(0.45);
				return true;
			}
			else {
				static_cast<Item*>(b)->hitWall();
				return true;
			}
		}
		// check if item has collided with floor
		if (a->getName() == "held_item" && b->getName() == "floor")
		{
			if (static_cast<Item*>(a)->getState() == Item::THROWN || static_cast<Item*>(a)->getState() == Item::FALLING) {
				static_cast<Item*>(a)->stopY();
				//static_cast<Item*>(a)->moveNoLimit(Vec2(-200, 0));
				solve.setFriction(0.2f);
				solve.setRestitution(0);
				return true;
			}
			else {
				static_cast<Item*>(a)->hitWall();
				return true;
			}
		}
		else if (a->getName() == "floor" && b->getName() == "held_item") {
			
			if (static_cast<Item*>(b)->getState() == Item::THROWN || static_cast<Item*>(a)->getState() == Item::FALLING) {
				static_cast<Item*>(b)->stopY();
				//static_cast<Item*>(b)->moveNoLimit(Vec2(-200,0));
				solve.setFriction(0.2f);
				solve.setRestitution(0);
				return true;
			}
			else {
				static_cast<Item*>(b)->hitWall();
				return true;
			}
		}
		// check if item has collided with wall
		if (a->getName() == "held_item" && (b->getName() == "wall" || b->getName() == "door" || b->getName() == "exit_door"))
		{
			if (static_cast<Item*>(a)->getState() == Item::THROWN || static_cast<Item*>(a)->getState() == Item::FALLING) {
				//solve.setSurfaceVelocity(Vec2(-100, 0));
				//static_cast<Item*>(a)->move(Vec2(-150, 0));
				return true;
			}
			else {
				static_cast<Item*>(a)->hitWall();
				return true;
			}
		}
		else if ((a->getName() == "wall" || a->getName() == "door"  || a->getName() == "exit_door") && b->getName() == "held_item") {
			if (static_cast<Item*>(b)->getState() == Item::THROWN || static_cast<Item*>(a)->getState() == Item::FALLING) {
				//solve.setSurfaceVelocity(Vec2(-100, 0));
				//static_cast<Item*>(b)->move(Vec2(-150, 0));
				return true;
			}
			else {
				static_cast<Item*>(b)->hitWall();
				return true;
			}
		}

		// check if player has collided with a wall
		if (a->getName() == "player" && (b->getName() == "wall" || b->getName() == "door"))
		{
			static_cast<Player*>(a)->touchingWall = true;
			return true;
		}
		else if ((a->getName() == "wall" || a->getName() == "door") && b->getName() == "player") {
			static_cast<Player*>(b)->touchingWall = true;
			return true;
		}

		//alert enemy and wall
		if (a->getName() == "enemy_alert" && (b->getName() == "wall" || b->getName() == "door" || b->getName() == "exit_door"))
		{
			static_cast<Enemy*>(a)->touchingWall = true;
			return true;
		}
		else if ((a->getName() == "wall" || a->getName() == "door" || a->getName() == "exit_door") && b->getName() == "enemy_alert")
		{
			static_cast<Enemy*>(b)->touchingWall = true;
			return true;
		}
		//enemy and wall
		if (a->getName() == "enemy" && (b->getName() == "wall" || b->getName() == "door" || b->getName() == "exit_door"))
		{
			static_cast<Enemy*>(a)->touchingWall = true;
			return true;
		}
		else if ((a->getName() == "wall" || a->getName() == "door" || a->getName() == "exit_door") && b->getName() == "enemy")
		{
			static_cast<Enemy*>(b)->touchingWall = true;
			return true;
		}

		// check if player has collided with an enemy
		if (a->getName() == "player" && b->getName() == "enemy")
		{
			if (player->isHidden() != true) {
				//CCLOG("YOU TOUCHED AN ENEMY");
				static_cast<Enemy*>(b)->playerTouch();
				static_cast<Enemy*>(b)->detectedPlayer = player;
				if (player->getPositionY() < (static_cast<Enemy*>(b)->getPositionY() + static_cast<Enemy*>(b)->getSize().height / 2)) {//only collide if player is below top of enemy
					static_cast<Player*>(a)->stop();
					float displacement = a->getPositionX() - b->getPositionX();
					if (displacement <= 0) {
						static_cast<Enemy*>(b)->moveAbsolute(Vec2(20, 0));
						static_cast<Player*>(a)->moveAbsolute(Vec2(-200, 0));
					}
					else {
						static_cast<Enemy*>(b)->moveAbsolute(Vec2(-20, 0));
						static_cast<Player*>(a)->moveAbsolute(Vec2(200, 0));
					}
					//solve.setRestitution(5.0f);
					return true;
				}
				else {
					return false;
				}
			}
			else {
				//CCLOG("YOU AVOIDED DETECTION");
				return false;
			}
		}
		else if ((a->getName() == "enemy" && b->getName() == "player")) {
			if (player->isHidden() != true) {
				//CCLOG("YOU TOUCHED AN ENEMY");
				static_cast<Enemy*>(a)->playerTouch();
				static_cast<Enemy*>(a)->detectedPlayer = player;
				if (player->getPositionY() < (static_cast<Enemy*>(a)->getPositionY() + static_cast<Enemy*>(a)->getSize().height / 2)) {//only collide if player is below top of enemy
					static_cast<Player*>(b)->stop();
					float displacement = b->getPositionX() - a->getPositionX();
					if (displacement <= 0) {
						static_cast<Enemy*>(a)->moveAbsolute(Vec2(20, 0));
						static_cast<Player*>(b)->moveAbsolute(Vec2(-200, 0));
					}
					else {
						static_cast<Enemy*>(a)->moveAbsolute(Vec2(-20, 0));
						static_cast<Player*>(b)->moveAbsolute(Vec2(200, 0));
					}
					//solve.setRestitution(5.0f);
					return true;
				}
			}
			else {
				//CCLOG("YOU AVOIDED DETECTION");
				return false;
			}
		}

		// check if player has collided with an alerted enemy
		if ((a->getName() == "player" && b->getName() == "enemy_alert"))
		{
			if (player->isHidden() != true) {
				static_cast<Enemy*>(b)->playerTouch();
				return true;
			}
			else {
				return false;
			}
		}
		else if ((a->getName() == "enemy_alert" && b->getName() == "player")) {
			if (player->isHidden() != true) {
				static_cast<Enemy*>(a)->playerTouch();
				return true;
			}
			else {
				return false;
			}
		}

		//check if enemy has been hit by player's attack
		if ((a->getName() == "enemy" || a->getName() == "enemy_alert") && b->getName() == "held_item")
		{
			if (static_cast<Item*>(b) != static_cast<Enemy*>(a)->heldItem && static_cast<Item*>(b)->enemyItem == false) {//only get hit by non-enemy attacks
				return false;
			}
			else {//so enemies don't get hit by their own weapon
				if (static_cast<Item*>(b)->getState() == Item::THROWN) {
					return false;
				}
				return false;
			}
		}
		else if (a->getName() == "held_item" && (b->getName() == "enemy" || b->getName() ==  "enemy_alert"))
		{
			if (static_cast<Item*>(a) != static_cast<Enemy*>(b)->heldItem && static_cast<Item*>(a)->enemyItem == false) {//only get hit by non-enemy attacks
				return false;
			}
			else {//so enemies don't get hit by their own weapon
				if (static_cast<Item*>(a)->getState() == Item::THROWN) {
					return false;
				}
				return false;
			}
		}

		//player and held item
		if (a->getName() == "player" && b->getName() == "held_item")
		{
			if (static_cast<Item*>(b) != static_cast<Player*>(a)->heldItem) {
				if (static_cast<Item*>(b)->getState() != Item::HELD) {
					return true;
				}
				return false;
			}
			else {//so player doesn't get hit by their own weapon
				return false;
			}
		}
		else if (a->getName() == "held_item" && b->getName() == "player") {
			if (static_cast<Item*>(a) != static_cast<Player*>(b)->heldItem) {
				if (static_cast<Item*>(b)->getState() != Item::HELD) {
					return true;
				}
				return false;
			}
			else {//so player doesn't get hit by their own weapon
				return false;
			}
		}

		//enemy and dead body
		if (a->getName() == "enemy" && b->getName() == "dead_body")
		{
			if (static_cast<DeadBody*>(b)->isHidden == false) {
				static_cast<Enemy*>(a)->bodySeen = static_cast<DeadBody*>(b);
			}
			return false;
		}
		else if (a->getName() == "dead_body" && b->getName() == "enemy")
		{
			if (static_cast<DeadBody*>(a)->isHidden == false) {
				static_cast<Enemy*>(b)->bodySeen = static_cast<DeadBody*>(a);
			}
			return false;
		}

		//enemy and knocked out enemy
		if (a->getName() == "enemy" && b->getName() == "enemy")
		{
			if (static_cast<Enemy*>(b)->isKnockedOut() == true && static_cast<Enemy*>(b)->isKnockedOut() == true) {
				return true;
			}
			else if (static_cast<Enemy*>(b)->isKnockedOut()== true) {
				static_cast<Enemy*>(a)->bodySeen = static_cast<Enemy*>(b);
			}
			else if(static_cast<Enemy*>(a)->isKnockedOut() == true) {
				static_cast<Enemy*>(b)->bodySeen = static_cast<Enemy*>(a);
			}
			return false;
		}

		//check if player can pick up body
		if (a->getName() == "player_pickup" && b->getName() == "body_radius")
		{
			if ((player->getTag() >= 10 && static_cast<DeadBody*>(b->getParent())->isHidden == true) || (player->getTag() < 10 && static_cast<DeadBody*>(b->getParent())->isHidden == false)) {
				player->bodyToPickUp = static_cast<DeadBody*>(b->getParent());
				static_cast<DeadBody*>(b->getParent())->playerRange = true;
			}
			return false;
		}
		else if (a->getName() == "body_radius" && b->getName() == "player_pickup")
		{
			if ((player->getTag() >= 10 && static_cast<DeadBody*>(a->getParent())->isHidden == true) || (player->getTag() < 10 && static_cast<DeadBody*>(a->getParent())->isHidden == false)) {
				player->bodyToPickUp = static_cast<DeadBody*>(a->getParent());
				static_cast<DeadBody*>(a->getParent())->playerRange = true;
			}
			return false;
		}

		//check if player can pick up item
		if (a->getName() == "player_pickup" && b->getName() == "item_radius")
		{
			if (player->behindObject == false) {
				player->itemToPickUp = static_cast<Item*>(b->getParent());
				static_cast<Item*>(b->getParent())->playerRange = true;
			}
			return false;
		}
		else if (a->getName() == "item_radius" && b->getName() == "player_pickup")
		{
			if (player->behindObject == false) {
				player->itemToPickUp = static_cast<Item*>(a->getParent());
				static_cast<Item*>(a->getParent())->playerRange = true;
			}
			return false;
		}

		//player and item
		if ((a->getName() == "player" && b->getName() == "item") || (a->getName() == "item" && b->getName() == "player"))
		{
			return false;
		}

		//enemy and item
		if (((a->getName() == "enemy" || a->getName() == "enemy_alert") && b->getName() == "item") || (a->getName() == "item" && (b->getName() == "enemy" || b->getName() == "enemy_alert")))
		{
			return false;
		}


		//player and hide object
		if (a->getName() == "player" && b->getName() == "hide_object")
		{
			player->objectToHideBehind = static_cast<HideObject*>(b);
			static_cast<HideObject*>(b)->playerRange = true;
			return false;
		}
		else if (a->getName() == "hide_object" && b->getName() == "player")
		{
			player->objectToHideBehind = static_cast<HideObject*>(a);
			static_cast<HideObject*>(a)->playerRange = true;
			return false;
		}

		//player and stairway
		if (a->getName() == "player" && b->getName() == "stair")
		{
			player->stairToUse = static_cast<Stair*>(b);
			static_cast<Stair*>(b)->playerRange = true;
			return false;
		}
		else if (a->getName() == "stair" && b->getName() == "player")
		{
			player->stairToUse = static_cast<Stair*>(a);
			static_cast<Stair*>(a)->playerRange = true;
			return false;
		}

		//alert enemy and stairway
		if (a->getName() == "enemy_alert" && b->getName() == "stair")
		{
			if (static_cast<Enemy*>(a)->currentFloor == static_cast<Enemy*>(a)->detectedPlayer->currentFloor) {
				if (static_cast<Enemy*>(a)->runningAway == true) {
					static_cast<Enemy*>(a)->stairToUse = static_cast<Stair*>(b);
				}
			}
			return false;
		}
		else if (a->getName() == "stair" && b->getName() == "enemy_alert")
		{
			if (static_cast<Enemy*>(b)->currentFloor == static_cast<Enemy*>(b)->detectedPlayer->currentFloor) {
				if (static_cast<Enemy*>(b)->runningAway == true) {
					static_cast<Enemy*>(b)->stairToUse = static_cast<Stair*>(a);
				}
			}
			return false;
		}

		//alert enemy and hide object
		if ((a->getName() == "enemy_alert" && b->getName() == "hide_object") || (a->getName() == "hide_object" && b->getName() == "enemy_alert"))
		{
			return false;
		}

		//player and door
		if (a->getName() == "player" && (b->getName() == "door_radius" || b->getName() == "vent_radius"))
		{
			//CCLOG("CAN OPEN DOOR");
			player->doorToUse = static_cast<Door*>(b->getParent());
			static_cast<Door*>(b->getParent())->playerRange = true;
			return false;
		}
		else if ((a->getName() == "door_radius" || a->getName() == "vent_radius") && b->getName() == "player")
		{
			//CCLOG("CAN OPEN DOOR");
			player->doorToUse = static_cast<Door*>(a->getParent());
			static_cast<Door*>(a->getParent())->playerRange = true;
			return false;
		}

		//enemy and item radius
		if (a->getName() == "enemy" && b->getName() == "item_radius")
		{
			if (static_cast<Item*>(b->getParent())->enemyCanUse == true && static_cast<Item*>(b->getParent())->getPhysicsBody()->getVelocity().y < 0 && static_cast<Item*>(b->getParent())->enemyItem == false) {//item is moving downwards and has moved from original poaisiton
				if (static_cast<Enemy*>(a)->fallenItem == NULL) {//if they don't already have an item that's hit them
					static_cast<Enemy*>(a)->fallenItem = static_cast<Item*>(b->getParent());
				}
			}
			return false;
		}
		else if (a->getName() == "item_radius" && b->getName() == "enemy")
		{
			if (static_cast<Item*>(a->getParent())->enemyCanUse == true && static_cast<Item*>(a->getParent())->getPhysicsBody()->getVelocity().y < 0 && static_cast<Item*>(a->getParent())->enemyItem == false) {//item is moving downwards and has moved from original poaisiton
				if (static_cast<Enemy*>(b)->fallenItem == NULL) {//if they don't already have an item that's hit tiem
					static_cast<Enemy*>(b)->fallenItem = static_cast<Item*>(a->getParent());
				}
			}
			return false;
		}
		//alert enemy and item radius
		if (a->getName() == "enemy_alert" && b->getName() == "item_radius")
		{
			if (static_cast<Item*>(b->getParent())->enemyItem == false) {
				if (static_cast<Enemy*>(a)->itemToPickUp == NULL) {//if they don't already have an item to pick up
					static_cast<Enemy*>(a)->itemToPickUp = static_cast<Item*>(b->getParent());
				}
			}
			return false;
		}
		else if (a->getName() == "item_radius" && b->getName() == "enemy_alert")
		{
			if (static_cast<Item*>(a->getParent())->enemyItem == false) {
				if (static_cast<Enemy*>(b)->itemToPickUp == NULL) {//if they don't already have an item to pick up
					static_cast<Enemy*>(b)->itemToPickUp = static_cast<Item*>(a->getParent());
				}
			}
			return false;
		}

		//enemy and door radius
		if (a->getName() == "enemy" && (b->getName() == "door_radius" || b->getName() == "vent_radius"))
		{
			//if (static_cast<Door*>(b->getParent())->getName() == "door") {
			//	static_cast<Enemy*>(a)->doorToUse = static_cast<Door*>(b->getParent());
			//}
			return false;
		}
		else if ((a->getName() == "door_radius" || a->getName() == "vent_radius") && b->getName() == "enemy")
		{
			//if (static_cast<Door*>(a->getParent())->getName() == "door") {
			//	static_cast<Enemy*>(b)->doorToUse = static_cast<Door*>(a->getParent());
			//}
			return false;
		}
		//alert enemy and door radius
		if (a->getName() == "enemy_alert" && (b->getName() == "door_radius" || b->getName() == "vent_radius"))
		{
			return false;
		}
		else if ((a->getName() == "door_radius" || a->getName() == "vent_radius") && b->getName() == "enemy_alert")
		{
			return false;
		}

		//two players
		if (a->getName() == "player" && b->getName() == "player") {
			return false;
		}

	}
	return true;
}

//check if 2 objects collide and do something
bool Level::onContactBegin(cocos2d::PhysicsContact &contact){
	Node *a = contact.getShapeA()->getBody()->getNode();
	Node *b = contact.getShapeB()->getBody()->getNode();
	//player and noises
	if (a->getName() == "player" && b->getName() == "noise")
	{
		player->hearNoise(static_cast<Noise*>(b)->name);
		return false;
	}
	else if (a->getName() == "noise" && b->getName() == "player")
	{
		player->hearNoise(static_cast<Noise*>(a)->name);
		return false;
	}
	//item and hide radius
	if (a->getName() == "held_item" && b->getName() == "hide_radius")
	{
		if (static_cast<Item*>(a)->getAttackType() != Item::SHOOT) {
			static_cast<Item*>(a)->isUnderObject = true;
		}
		return false;
	}
	else if (a->getName() == "hide_radius" && b->getName() == "held_item")
	{
		if (static_cast<Item*>(b)->getAttackType() != Item::SHOOT) {
			static_cast<Item*>(b)->isUnderObject = true;
		}
		return false;
	}
	//enemy and noises
	if ((a->getName() == "enemy" || a->getName() == "enemy_alert") && b->getName() == "noise")
	{
		static_cast<Enemy*>(a)->noiseLocation = b->getPosition();
		static_cast<Enemy*>(a)->noiseRoom = static_cast<Noise*>(b)->room;
		return false;
	}
	else if (a->getName() == "noise" && (b->getName() == "enemy" || b->getName() == "enemy_alert"))
	{
		static_cast<Enemy*>(b)->noiseLocation = a->getPosition();
		static_cast<Enemy*>(b)->noiseRoom = static_cast<Noise*>(a)->room;
		return false;
	}
	//two held items
	if (a->getName() == "held_item" && b->getName() == "held_item") {
		if (static_cast<Item*>(a)->priority > static_cast<Item*>(b)->priority) {//if a's damage is higher than b's
			static_cast<Item*>(b)->hitWall();
		}
		else if (static_cast<Item*>(b)->priority > static_cast<Item*>(a)->priority) {//if b's is higher than a's
			static_cast<Item*>(a)->hitWall();
		}
		return false;
	}

	//check if enemy has been hit by player's attack
	if ((a->getName() == "enemy" || a->getName() == "enemy_alert") && b->getName() == "held_item")
	{
		if (static_cast<Item*>(b) != static_cast<Enemy*>(a)->heldItem && static_cast<Item*>(b)->enemyItem == false) {//only get hit by non-enemy attacks
			if (static_cast<Item*>(b)->getState() != Item::FALLING) {
				if (static_cast<Item*>(b)->isUnderObject == false) {
					static_cast<Enemy*>(a)->itemHitBy = static_cast<Item*>(b);
					if (static_cast<Enemy*>(a)->itemHitBy->getAttackType() == Item::SHOOT) {//guns lose health when thrown at enemies
						static_cast<Enemy*>(a)->itemHitBy->used();
					}
				}
			}
			else {
				static_cast<Enemy*>(a)->itemBumpedBy = static_cast<Item*>(b);
				static_cast<Enemy*>(a)->directionHitFrom = static_cast<Item*>(b)->getPhysicsBody()->getVelocity();
				static_cast<Item*>(b)->stop();
				static_cast<Item*>(b)->setRotation(0);
				static_cast<Item*>(b)->move(Vec2(-120,0));
			}
			if (static_cast<Item*>(b)->getState() == Item::THROWN && static_cast<Item*>(b) != static_cast<Enemy*>(a)->thrownItem) {
				static_cast<Item*>(b)->stop();
			}
			return false;
		}
		else {
			return false;
		}
	}
	else if (a->getName() == "held_item" && (b->getName() == "enemy" || b->getName() == "enemy_alert"))
	{
		if (static_cast<Item*>(a) != static_cast<Enemy*>(b)->heldItem && static_cast<Item*>(a)->enemyItem == false) {//only get hit by non-enemy attacks
			if (static_cast<Item*>(a)->getState() != Item::FALLING) {
				if (static_cast<Item*>(a)->isUnderObject == false) {
					static_cast<Enemy*>(b)->itemHitBy = static_cast<Item*>(a);
					if (static_cast<Enemy*>(b)->itemHitBy->getAttackType() == Item::SHOOT) {
						static_cast<Enemy*>(b)->itemHitBy->used();
					}
				}
			}
			else {
				static_cast<Enemy*>(b)->itemBumpedBy = static_cast<Item*>(a);
				static_cast<Enemy*>(b)->directionHitFrom = static_cast<Item*>(a)->getPhysicsBody()->getVelocity();
				static_cast<Item*>(a)->stop();
				static_cast<Item*>(a)->setRotation(0);
				static_cast<Item*>(a)->move(Vec2(-120, 0));
			}
			if (static_cast<Item*>(a)->getState() == Item::THROWN && static_cast<Item*>(a) != static_cast<Enemy*>(b)->thrownItem) {
				static_cast<Item*>(a)->stop();
			}
			return false;
		}
		else {
			return false;
		}
	}

	//player and held item
	if (a->getName() == "player" && b->getName() == "held_item")
	{
		if (static_cast<Item*>(b) != static_cast<Player*>(a)->heldItem) {//so player doesn't get hit by their own weapon
			if (static_cast<Item*>(b)->getState() != Item::FALLING) {
				static_cast<Player*>(a)->itemHitBy = (static_cast<Item*>(b));
			}
			else {
				static_cast<Player*>(a)->itemBumpedBy = static_cast<Item*>(a);
				static_cast<Player*>(a)->directionHitFrom = static_cast<Item*>(a)->getPhysicsBody()->getVelocity();
				static_cast<Item*>(b)->stop();
				static_cast<Item*>(b)->setRotation(0);
				static_cast<Item*>(b)->move(Vec2(-100, 0));
			}
			if (static_cast<Item*>(b)->getState() == Item::THROWN && static_cast<Item*>(b) != static_cast<Player*>(a)->thrownItem) {
				static_cast<Item*>(b)->stop();
			}
			return true;
		}
		else {
			return false;
		}
	}
	else if (a->getName() == "held_item" && b->getName() == "player") {
		if (static_cast<Item*>(a) != static_cast<Player*>(b)->heldItem) {//so player doesn't get hit by their own weapon
			if (static_cast<Item*>(a)->getState() != Item::FALLING) {
				static_cast<Player*>(b)->itemHitBy = (static_cast<Item*>(a));
			}
			else {
				static_cast<Player*>(b)->itemBumpedBy = static_cast<Item*>(a);
				static_cast<Player*>(b)->directionHitFrom = static_cast<Item*>(a)->getPhysicsBody()->getVelocity();
				static_cast<Item*>(a)->stop();
				static_cast<Item*>(a)->setRotation(0);
				static_cast<Item*>(a)->move(Vec2(-100, 0));
			}
			if (static_cast<Item*>(a)->getState() == Item::THROWN && static_cast<Item*>(a) != static_cast<Player*>(b)->thrownItem) {
				static_cast<Item*>(a)->stop();
			}
			return true;
		}
		else {
			return false;
		}
	}
	//enemy and door radius
	if (a->getName() == "enemy" && b->getName() == "door_radius")
	{
		static_cast<Enemy*>(a)->doorToUse = static_cast<Door*>(b->getParent());
		return false;
	}
	else if (a->getName() == "door_radius" && b->getName() == "enemy")
	{
		static_cast<Enemy*>(b)->doorToUse = static_cast<Door*>(a->getParent());
		return false;
	}
	//alert enemy and door radius
	if (a->getName() == "enemy_alert" && b->getName() == "door_radius")
	{
		static_cast<Enemy*>(a)->doorToUse = static_cast<Door*>(b->getParent());
		return false;
	}
	else if (a->getName() == "door_radius" && b->getName() == "enemy_alert")
	{
		static_cast<Enemy*>(b)->doorToUse = static_cast<Door*>(a->getParent());
		return false;
	}
	//alert enemy and door
	if (a->getName() == "enemy_alert" && b->getName() == "door")
	{
		static_cast<Enemy*>(a)->doorToUse = static_cast<Door*>(b);
		return false;
	}
	else if (a->getName() == "door" && b->getName() == "enemy_alert")
	{
		static_cast<Enemy*>(b)->doorToUse = static_cast<Door*>(a);
		return false;
	}
	//alert enemy and wall
	if (a->getName() == "enemy_alert" && (b->getName() == "wall" || b->getName() == "exit_door"))
	{
		static_cast<Enemy*>(a)->hitWall();
		return true;
	}
	else if ((a->getName() == "wall" || a->getName() == "exit_door") && b->getName() == "enemy_alert")
	{
		static_cast<Enemy*>(b)->hitWall();
		return true;
	}
	//enemy and wall
	if (a->getName() == "enemy" && (b->getName() == "wall" || b->getName() == "exit_door"))
	{
		static_cast<Enemy*>(a)->hitWall();
		return true;
	}
	else if ((a->getName() == "wall" || a->getName() == "exit_door") && b->getName() == "enemy")
	{
		static_cast<Enemy*>(b)->hitWall();
		return true;
	}

	//items and bodies
	if (a->getName() == "held_item" && b->getName() == "dead_body")
	{
		static_cast<DeadBody*>(b)->itemHit();
		return true;
	}
	else if (a->getName() == "dead_body"&& b->getName() == "held_item")
	{
		static_cast<DeadBody*>(a)->itemHit();
		return true;
	}

	//items and door
	if (a->getName() == "held_item" && (b->getName() == "door" || b->getName() == "vent"))
	{
		static_cast<Item*>(a)->hitWall();
		static_cast<Item*>(a)->makeNoise = true;
		static_cast<Door*>(b)->itemHit(static_cast<Item*>(a));
		return true;
	}
	else if ((a->getName() == "door" || a->getName() == "vent") && b->getName() == "held_item")
	{
		static_cast<Item*>(b)->hitWall();
		static_cast<Item*>(b)->makeNoise = true;
		static_cast<Door*>(a)->itemHit(static_cast<Item*>(b));
		return true;
	}
	//held/thrown items and walls
	if (a->getName() == "held_item" && (b->getName() == "wall" || b->getName() == "ceiling" || b->getName() == "floor" || b->getName() == "phys_object" || b->getName() == "exit_door"))
	{
		static_cast<Item*>(a)->makeNoise = true;
		return true;
	}
	else if ((a->getName() == "wall" || a->getName() == "ceiling" || a->getName() == "floor" || a->getName() == "phys_object" || a->getName() == "exit_door") && b->getName() == "held_item")
	{
		static_cast<Item*>(b)->makeNoise = true;
		return true;
	}
	//falling items and walls
	if (a->getName() == "item" && (b->getName() == "wall" || b->getName() == "ceiling" || b->getName() == "floor" || b->getName() == "phys_object" || b->getName() == "exit_door"))
	{
		if (static_cast<Item*>(a)->getState() == Item::FALLING) {
			static_cast<Item*>(a)->makeNoise = true;
		}
		return true;
	}
	else if ((a->getName() == "wall" || a->getName() == "ceiling" || a->getName() == "floor" || a->getName() == "phys_object" || a->getName() == "exit_door") && b->getName() == "item")
	{
		if (static_cast<Item*>(b)->getState() == Item::FALLING) {
			static_cast<Item*>(b)->makeNoise = true;
		}
		return true;
	}
	return true;
}

void Level::setBackground(string bgName, float scale) {
	backgroundScale = scale;
	//setting background image
	background = Sprite::create(bgName);
	background->setAnchorPoint(Vec2(0, 0));
	background->setPositionZ(0.0f);
	// position the sprite on the center of the screen
	auto visibleSize = director->getVisibleSize();
	background->setPosition(Vec2((visibleSize.width / 2) - (background->getContentSize().width / 2), 0));
	//set scale
	background->setScale(backgroundScale);
	//disabling anti-aliasing!!! (looks like blurry poop without this)
	Texture2D::TexParams texParams = { GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };
	background->getTexture()->setTexParameters(texParams);
	background->setGlobalZOrder(-1);
	mainLayer->addChild(background);

	//creating collision box on edge of game area
	/*auto border = PhysicsBody::createEdgeBox(background->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT, 1.0f, Vec2(0, 0));
	border->setDynamic(false);
	background->addComponent(border);*/
}

void Level::createFloor(Vec2 position, vector<RoomData*> *roomData, int height)
{
	Room* room;

	for (int i = 0; i < (*roomData).size(); i++) {
		(*roomData)[i]->num = i;//getting room's number
		(*roomData)[i]->left = position.x;//getting room's left side position
		(*roomData)[i]->right = position.x + (*roomData)[i]->width;//getting room's right side position

		room = Room::create();
		room->createRoom(&doors, &mainLayer->stairs, &hideObjects, &physObjects, &mainLayer->items, &enemies, &pathNodes, player, position, (*roomData)[i], height);

		position = position + Vec2((*roomData)[i]->width + room->fullThick, 0);//adding length of created room to set position for next room

		rooms.push_back(room);
	}
}

void Level::createLevel(Vec2 position, float levelWidth)
{
	Room r;

	//calculating width differences between floors and first floor
	int firstFloorWidth = 0;
	vector<int> floorOffsets;
	for (int i = 0; i < mainLayer->floors.size(); i++) {
		int totalWidth = 0;
		for (int j = 0; j < mainLayer->floors[i].rooms.size(); j++) {
			totalWidth += mainLayer->floors[i].rooms[j]->width;
			if (j != mainLayer->floors[i].rooms.size() - 1) {//not the last room on the floor
				totalWidth += 20;//add thickness of walls
			}
		}
		if (i == 0) {
			firstFloorWidth = totalWidth;
		}
		floorOffsets.push_back((totalWidth - firstFloorWidth) / 2);
	}

	position = position + Vec2((levelWidth / 2) - (firstFloorWidth / 2) + r.thick, r.thick);//setting position to be in centre of background

	//generating floors
	for (int i = 0; i < mainLayer->floors.size(); i++) {
		mainLayer->floors[i].num = i;//getting floor's number
		mainLayer->floors[i].bot = position.y;//getting floor's bottom
		mainLayer->floors[i].top = position.y + mainLayer->floors[i].height;//getting floor's top

		createFloor(position - Vec2(floorOffsets[i], 0), &mainLayer->floors[i].rooms, mainLayer->floors[i].height);

		position = position + Vec2(0, mainLayer->floors[i].height + r.fullThick);//adding height of created floor to set position for next floor
	}
}

bool Level::initLevel(string filename){
	//initializing player
	player = Player::createWithSpriteFrameName();
	player->initObject();

	//used to store data to pass into createLevel function
	FloorData floorData;

	std::ifstream file;
	file.open(filename);
	if (!file){
		CCLOG("LEVEL FILE NOT FOUND");
		return false;
	}

	//getting data from level file:
	int floorNum = 0;
	string line;
	int roomNum = -1;
	while (getline(file, line)) {//each interation gets a single line from the file; gets data for a single room
		if (line[0] == '#') { continue; }//ignoring comment lines, which begin with the '#' character

		RoomData* roomData = new RoomData;

		if (line == "FLOOR_END") {//indicates to move to next floor in floors vector
			mainLayer->floors.push_back(floorData);
			floorData.rooms.clear();
			floorNum++;
			roomNum = -1;
			continue;
		}

		if (roomNum == -1) {//first line is the height of the floor
			floorData.height = std::atoi(line.c_str());
			roomNum++;
			continue;
		}

		//getting data for a single component:
		std::istringstream l(line);//putting line into a string stream
		string chunk;
		int j = 0;
		while (getline(l, chunk, '|')) {//each iteration gets a chunk of the line, delmimited by the '|' character; gets data for a sinlge component of a room

			if (j == 0) {//first chunck is width of the room
				roomData->width = std::atoi(chunk.c_str());
				j++;
				continue;
			}

			//getting single pieces of data:
			vector<string> pieces;
			std::istringstream c(chunk);//putting chunck into a string stream
			string piece;
			while (getline(c, piece, ',')) {//each iteration gets a piece of the chunck, delmimited by the ',' character; gets a single piece of data for a component
				pieces.push_back(piece);
			}

			//setting data based on component type:
			if (pieces[0] == "bg") {
				roomData->bgName = pieces[1];
			}
			else if (pieces[0] == "door") {
				DoorData doorData;
				doorData.type = 1;
				doorData.floorNum = floorNum;
				//set door as locked
				if (pieces.size() > 2) {
					if (pieces[2] == "locked") {
						doorData.locked = true;
					}
				}
				if (pieces.size() > 3) {//set door's position on wall
					doorData.pos = atof(pieces[3].c_str());
				}
				//set what wall door is on
				if (pieces[1] == "right") {
					doorData.leftRoom = Vec2(roomNum, floorNum);
					doorData.rightRoom = Vec2(roomNum + 1, floorNum);
					roomData->rightDoors.push_back(doorData);
					roomData->hasRightDoor = true;
				}
				else if (pieces[1] == "left") {
					doorData.leftRoom = Vec2(roomNum - 1, floorNum);
					doorData.rightRoom = Vec2(roomNum, floorNum);
					roomData->leftDoors.push_back(doorData);
					roomData->hasLeftDoor = true;
				}
				else if (pieces[1] == "top") {
					roomData->ceilingDoors.push_back(doorData);
				}
				else if (pieces[1] == "bot") {
					roomData->bottomDoors.push_back(doorData);
				}
			}
			else if (pieces[0] == "exit") {
				DoorData doorData;
				doorData.type = 3;
				//set door as locked
				if (pieces.size() > 2) {
					if (pieces[2] == "locked") {
						doorData.locked = true;
					}
				}
				if (pieces.size() > 3) {//set door's position on wall
					doorData.pos = atof(pieces[3].c_str());
				}
				//set what wall door is on
				if (pieces[1] == "right") {
					doorData.leftRoom = Vec2(roomNum, floorNum);
					doorData.rightRoom = Vec2(roomNum + 1, floorNum);
					roomData->rightDoors.push_back(doorData);
					roomData->hasRightDoor = true;
				}
				else if (pieces[1] == "left") {
					doorData.leftRoom = Vec2(roomNum - 1, floorNum);
					doorData.rightRoom = Vec2(roomNum, floorNum);
					roomData->leftDoors.push_back(doorData);
					roomData->hasLeftDoor = true;
				}
				else if (pieces[1] == "top") {
					roomData->ceilingDoors.push_back(doorData);
				}
				else if (pieces[1] == "bot") {
					roomData->bottomDoors.push_back(doorData);
				}
			}
			else if (pieces[0] == "vent") {
				DoorData ventData;
				ventData.type = 2;
				//set vent as locked
				if (pieces.size() > 2) {
					if (pieces[2] == "locked") {
						ventData.locked = true;
					}
				}
				if (pieces.size() > 3) {//set vent's position on wall
					ventData.pos = atof(pieces[3].c_str());
				}
				//set what wall vent is on
				if (pieces[1] == "right") {
					roomData->rightDoors.push_back(ventData);
				}
				else if (pieces[1] == "left") {
					roomData->leftDoors.push_back(ventData);
				}
				else if (pieces[1] == "top") {
					roomData->ceilingDoors.push_back(ventData);
				}
				else if (pieces[1] == "bot") {
					roomData->bottomDoors.push_back(ventData);
				}
			}
			//Stairs
			else if (pieces[0] == "stair") {
				Stair* stair = Stair::createWithSpriteFrameName();
				stair->pairNum = atof(pieces[1].c_str());
				if (pieces[2] == "A") {
					stair->type = 1;
				}
				else if (pieces[2] == "B") {
					stair->type = 2;
				}
				stair->initObject();
				stair->roomStartPos = Vec2(atof(pieces[3].c_str()), atof(pieces[4].c_str()));
				stair->startRoom = Vec2(roomNum, floorNum);
				mainLayer->stairs.push_back(stair);
			}
			//Hide Objects
			else if (pieces[0] == "hide_object") {
				HideObject* hideObject;
				hideObject = HideObject::createWithSpriteFrameName("objects/" + pieces[1] + ".png");//all hide objects are exactly the same except for appearance
				hideObject->initHideObject(pieces[1]);
				hideObject->roomStartPos = Vec2(atof(pieces[2].c_str()), atof(pieces[3].c_str()));
				hideObject->startRoom = Vec2(roomNum, floorNum);
				hideObjects.push_back(hideObject);
			}
			//Physical Objects
			else if (pieces[0] == "phys_object") {
				PhysObject* physObject;
				if (pieces[1] == "table") {
					physObject = Table::createWithSpriteFrameName();
				}
				else if (pieces[1] == "kitch_table") {
					physObject = KitchenTable::createWithSpriteFrameName();
				}
				else if (pieces[1] == "fountain") {
					physObject = Fountain::createWithSpriteFrameName();
				}
				else if (pieces[1] == "wall_shelf") {
					physObject = WallShelf::createWithSpriteFrameName();
				}
				else if (pieces[1] == "vent_cover") {
					physObject = VentCover::createWithSpriteFrameName();
				}
				physObject->initObject();
				physObject->roomStartPos = Vec2(atof(pieces[2].c_str()), atof(pieces[3].c_str()));
				physObject->startRoom = Vec2(roomNum, floorNum);
				physObjects.push_back(physObject);
			}
			//Items
			else if (pieces[0] == "item") {
				Item* item;
				if (pieces[1] == "knife") {
					item = Knife::createWithSpriteFrameName();
				}
				else if (pieces[1] == "key") {
					item = Key::createWithSpriteFrameName();
				}
				else if (pieces[1] == "hammer") {
					item = Hammer::createWithSpriteFrameName();
				}
				else if (pieces[1] == "gun") {
					item = Gun::createWithSpriteFrameName();
				}
				else if (pieces[1] == "mug") {
					item = Mug::createWithSpriteFrameName();
				}
				else if (pieces[1] == "crowbar") {
					item = Crowbar::createWithSpriteFrameName();
				}
				else if (pieces[1] == "ironbar") {
					item = IronBar::createWithSpriteFrameName();
				}
				else if (pieces[1] == "screwdriver") {
					item = Screwdriver::createWithSpriteFrameName();
				}
				item->initObject();
				item->roomStartPos = Vec2(atof(pieces[2].c_str()), atof(pieces[3].c_str()));
				item->startRoom = Vec2(roomNum, floorNum);
				mainLayer->items.push_back(item);
			}
			//Enemies
			else if (pieces[0] == "enemy") {
				Enemy* enemy;
				if (pieces[1] == "thug") {
					enemy = Thug::createWithSpriteFrameName();//should be Guard subclass
					numEnemies++;
				}
				else if (pieces[1] == "guard") {
					enemy = Guard::createWithSpriteFrameName();//should be Guard subclass
					numEnemies++;
				}
				else if (pieces[1] == "boss") {
					enemy = Boss::createWithSpriteFrameName();//should be Guard subclass
					numBosses++;//add a boss to boss level counter
				}
				enemy->roomStartPos = Vec2(atof(pieces[2].c_str()), atof(pieces[3].c_str()));
				enemy->startRoom = Vec2(roomNum, floorNum);
				enemy->currentFloor = floorNum;
				if (pieces.size() > 4) {
					enemy->setPathTag(pieces[4]);//"STAND_LEFT", "STAND_RIGHT", "STAND_SWITCH",  indicate the enemy will stand still facing that direction, switch turns in both direcitons
				}
				if (pieces.size() > 5) {
					if (pieces[5] != "none") {
						Item* item;
						if (pieces[5] == "knife") {
							item = Knife::createWithSpriteFrameName();
						}
						else if (pieces[5] == "key") {
							item = Key::createWithSpriteFrameName();
						}
						else if (pieces[5] == "hammer") {
							item = Hammer::createWithSpriteFrameName();
						}
						else if (pieces[5] == "gun") {
							item = Gun::createWithSpriteFrameName();
						}
						else if (pieces[5] == "mug") {
							item = Mug::createWithSpriteFrameName();
						}
						else if (pieces[5] == "crowbar") {
							item = Crowbar::createWithSpriteFrameName();
						}
						else if (pieces[5] == "ironbar") {
							item = IronBar::createWithSpriteFrameName();
						}
						else if (pieces[5] == "screwdriver") {
							item = Screwdriver::createWithSpriteFrameName();
						}
						item->initObject();
						item->roomStartPos = Vec2(atof(pieces[2].c_str()), atof(pieces[3].c_str()));
						item->startRoom = Vec2(roomNum, floorNum);
						item->startHeld();
						mainLayer->items.push_back(item);
						enemy->itemToPickUp = item;
					}
				}
				if (pieces.size() > 6) {
					Item* item;
					if (pieces[6] == "knife") {
						item = Knife::createWithSpriteFrameName();
					}
					else if (pieces[6] == "key") {
						item = Key::createWithSpriteFrameName();
					}
					else if (pieces[6] == "hammer") {
						item = Hammer::createWithSpriteFrameName();
					}
					else if (pieces[6] == "gun") {
						item = Gun::createWithSpriteFrameName();
					}
					else if (pieces[6] == "mug") {
						item = Mug::createWithSpriteFrameName();
					}
					else if (pieces[6] == "crowbar") {
						item = Crowbar::createWithSpriteFrameName();
					}
					else if (pieces[6] == "ironbar") {
						item = IronBar::createWithSpriteFrameName();
					}
					else if (pieces[6] == "screwdriver") {
						item = Screwdriver::createWithSpriteFrameName();
					}
					item->initObject();
					item->roomStartPos = Vec2(atof(pieces[2].c_str()), atof(pieces[3].c_str()));
					item->startRoom = Vec2(roomNum, floorNum);
					mainLayer->items.push_back(item);
					enemy->offhandItem = item;
				}
				enemy->initObject();
				enemies.push_back(enemy);
			}
			//Path Nodes
			else if (pieces[0] == "node") {
				PathNode* pathNode = PathNode::create();
				pathNode->initNode(atoi(pieces[1].c_str()), atoi(pieces[2].c_str()), atof(pieces[3].c_str()), pieces[4]);
				pathNode->startRoom = Vec2(roomNum, floorNum);
				pathNode->roomStartPos = Vec2(atoi(pieces[1].c_str()), 0);
				pathNodes.push_back(pathNode);
			}
			//Player
			else if (pieces[0] == "player") {//set player position in room
				player->roomStartPos = Vec2(atof(pieces[1].c_str()), atof(pieces[2].c_str()));
				player->startRoom = Vec2(roomNum, floorNum);
				player->currentFloor = floorNum;
			}

			pieces.clear();
			j++;
		}

		roomData->room = Vec2(roomNum, floorNum);
		floorData.rooms.push_back(roomData);
		roomNum++;
	}
	file.close();

	//make the level
	createLevel(background->getPosition(), background->getContentSize().width * backgroundScale);
	//setting camera to player position
	camPos->setPosition(player->getPosition() + camOffset);

	//rooms
	for (int i = 0; i < rooms.size(); i++) {
		rooms[i]->setTag(rooms[i]->getTag() + i);//giving a unique tag to each room
		mainLayer->addChild(rooms[i]);
	}
	//doors
	for (int i = 0; i < doors.size(); i++) {
		for (int j = 0; j < mainLayer->floors.size(); j++) {
			for (int k = 0; k < mainLayer->floors[j].rooms.size(); k++) {
				if (doors[i]->leftRoomCoords == mainLayer->floors[j].rooms[k]->room) {
					doors[i]->leftRoom = mainLayer->floors[j].rooms[k];
				}
				if (doors[i]->rightRoomCoords == mainLayer->floors[j].rooms[k]->room) {
					doors[i]->rightRoom = mainLayer->floors[j].rooms[k];
				}
			}
		}
		if (doors[i]->defaultLocked == true) {
			doors[i]->lock();
		}
		doors[i]->setTag(doors[i]->getTag() + i);//giving a unique tag to each door
		mainLayer->addChild(doors[i]);
	}
	mainLayer->doors = doors;
	//stairways
	for (int i = 0; i < mainLayer->stairs.size(); i++) {
		mainLayer->addChild(mainLayer->stairs[i]);//Do Not give unique tag to each stairway, already done elsewhere
	}
	//hide objects
	for (int i = 0; i < hideObjects.size(); i++) {
		hideObjects[i]->setTag(hideObjects[i]->getTag() + i);//giving a unique tag to each object
		mainLayer->addChild(hideObjects[i]);
	}
	//physical objects
	for (int i = 0; i < physObjects.size(); i++) {
		physObjects[i]->setTag(physObjects[i]->getTag() + i);//giving a unique tag to each object
		mainLayer->addChild(physObjects[i]);
	}
	//items
	for (int i = 0; i < mainLayer->items.size(); i++) {
		mainLayer->items[i]->setTag(mainLayer->items[i]->getTag() + i);//giving a unique tag to each item
		mainLayer->addChild(mainLayer->items[i]);
	}
	//enemies
	for (int i = 0; i < enemies.size(); i++) {
		enemies[i]->setTag(enemies[i]->getTag() + i);//giving a unique tag to each enemy
		mainLayer->addChild(enemies[i]);
	}
	//path nodes
	for (int i = 0; i < pathNodes.size(); i++) {
		mainLayer->addChild(pathNodes[i]);
	}
	//player
	mainLayer->addChild(player);
	return true;
}
