#include "Level.h"

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

	//node everything in level is attached to
	mainLayer = GameLayer::create();
	addChild(mainLayer);

	//initializing player
	player = Player::createWithSpriteFrameName();
	player->initObject();
	mainLayer->addChild(player);
	//player->noclip();

	//Invisible Node for the camera to follow
	camPos = Node::create();
	mainLayer->addChild(camPos);

	//necessary for collision detection
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Level::onContactBegin, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

	contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactPreSolve = CC_CALLBACK_2(Level::onContactPreSolve, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

	auto label1 = Label::createWithTTF("Space for Pick Up", "fonts/Pixel-Noir Skinny Short.ttf", 24);
	label1->setPosition(player->getPosition());
	label1->setGlobalZOrder(10);
	mainLayer->addChild(label1, 3000);

	//for running the update function
	schedule(schedule_selector(Level::onStart));
}

void Level::onStart(float deltaTime){
	unschedule(schedule_selector(Level::onStart));

	//initializing enemy joints
	//for (int i = 0; i < enemies.size(); i++) {
	//	enemies[i]->initJoints();
	//}

	getScene()->getPhysicsWorld()->setGravity(Vec2(0, -200));

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

void Level::update(float deltaTime){
	//updating time
	gameTime += deltaTime;

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
		doors[i]->updateColour();
		doors[i]->playerInRange();
	}
	//items update
	for (int i = 0; i < mainLayer->items.size(); i++) {
		if (mainLayer->items[i]->getState() == Item::GROUND) {
			mainLayer->items[i]->hasMoved();
			mainLayer->items[i]->playerInRange(player);
		}
		else if (mainLayer->items[i]->getState() == Item::HELD) {

		}
		else if (mainLayer->items[i]->getState() == Item::THROWN) {
			mainLayer->items[i]->checkSpeed();
		}
	}

	//for drawing vision rays
	if (visionRays){
		removeChild(visionRays, true);
	}
	visionRays = DrawNode::create();
	visionRays->setGlobalZOrder(10);
	//enemy update
	vector<Vec2> points;
	Vec2 start;
	for (int i = 0; i < enemies.size(); i++) {
		//remove dead enemies from scene, will be replaced with a dead body
		//this will be done in Death State exit function
		if (enemies.at(i)->isReallyDead() == true) {
			enemies.at(i)->removeFromParentAndCleanup(true);
			enemies.erase(enemies.begin() + i);
			i--;
			continue;
		}
		//enemy vision:
		enemies[i]->visionRays(&points, &start);
		//drawing vision rays
		for (int j = 0; j < points.size(); j++) {
			//visionRays->drawDot(points[j], 1, Color4F::WHITE);
			visionRays->drawSegment(start, points[j], 2, Color4F(1,0.9,0.1,0.2));
		}
		points.clear();

		enemies[i]->update(mainLayer, gameTime);
	}
	addChild(visionRays);

	//player update:
	player->update(mainLayer, gameTime);

	//check if player is going to interact with door/stairway
	if (INPUTS->getKeyPress(KeyCode::KEY_Q)) {
		if (player->stairToUse != NULL) {
			player->handleInput(mainLayer, gameTime, USE_STAIR);
		}
		else if (player->doorToUse != NULL) {
			player->handleInput(mainLayer, gameTime, USE_DOOR);
		}
	}
	//check if player is dropping item
	if (INPUTS->getKeyPress(KeyCode::KEY_F)) {
		if (player->heldItem != NULL) {
			player->handleInput(mainLayer, gameTime, DROP);
		}
	}
	//check if player is using item
	if (INPUTS->getKeyPress(KeyCode::KEY_SPACE)) {
		if (player->heldItem != NULL) {
			player->handleInput(mainLayer, gameTime, USE_ITEM);
		}
	}
	if (INPUTS->getKeyRelease(KeyCode::KEY_SPACE)) {
		if (player->heldItem != NULL) {
			player->handleInput(mainLayer, gameTime, USE_RELEASE);
		}
	}
	//checking to see if player is picking up an item
	if (INPUTS->getKeyPress(KeyCode::KEY_E)) {
		if (player->itemToPickUp != NULL && player->heldItem == NULL) {
			player->handleInput(mainLayer, gameTime, PICKUP);
		}
	}
	//check if player is going to hide behind object
	if (INPUTS->getKeyPress(KeyCode::KEY_CTRL)) {
		if (player->objectToHideBehind != NULL) {
			player->handleInput(mainLayer, gameTime, HIDE);
		}
	}

	//change between standing/crouching
	if (INPUTS->getKey(KeyCode::KEY_W)) {
		player->handleInput(mainLayer, gameTime, MOVE_UP);
	}
	if (INPUTS->getKey(KeyCode::KEY_S)) {
		player->handleInput(mainLayer, gameTime, MOVE_DOWN);
	}

	//player movement input checking
	if (INPUTS->getKey(KeyCode::KEY_D)) {
		player->handleInput(mainLayer, gameTime, MOVE_RIGHT);
	}
	if (INPUTS->getKey(KeyCode::KEY_A)) {
		player->handleInput(mainLayer, gameTime, MOVE_LEFT);
	}
	if (INPUTS->getKeyRelease(KeyCode::KEY_D) || INPUTS->getKeyRelease(KeyCode::KEY_A)) {
		player->handleInput(mainLayer, gameTime, STOP);
	}
	if (INPUTS->getKeyPress(KeyCode::KEY_N)) {//for testing purposes only, do not abuse
		player->handleInput(mainLayer, gameTime, NO_CLIP);
	}

	//must be called after checking all player actions
	player->resetCollisionChecks();

	//camOffset = Vec2(0, 150 / camZoom);//adjusting camera offset with zoom level?
	//having camera follow player
	followBox(camPos, player, camBoundingBox, camOffset);
	camera->setPosition(camPos->getPosition());

	//update the keyboard each frame
	INPUTS->clearForNextFrame();
}

//check if 2 objects are touching, do something and set whether they collide or not after
bool Level::onContactPreSolve(PhysicsContact &contact, PhysicsContactPreSolve & solve) {
	Node *a = contact.getShapeA()->getBody()->getNode();
	Node *b = contact.getShapeB()->getBody()->getNode();

	if (a != NULL && b != NULL) {
		// check if player has collided with an enemy
		if ((a->getName() == "player" && b->getName() == "enemy"))
		{
			if (player->isHidden() != true) {
				//CCLOG("YOU TOUCHED AN ENEMY");
				static_cast<Enemy*>(b)->playerTouch();
				static_cast<Enemy*>(b)->detectedPlayer = player;
				solve.setRestitution(20.0f);
				return true;
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
				solve.setRestitution(20.0f);
				return true;
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
				return true;
			}
			else {//so enemies don't get hit by their own weapon
				return false;
			}
		}
		else if (a->getName() == "held_item" && (b->getName() == "enemy" || b->getName() ==  "enemy_alert"))
		{
			if (static_cast<Item*>(a) != static_cast<Enemy*>(b)->heldItem && static_cast<Item*>(a)->enemyItem == false) {//only get hit by non-enemy attacks
				return true;
			}
			else {//so enemies don't get hit by their own weapon
				return false;
			}
		}

		//player and held item
		if (a->getName() == "player" && b->getName() == "held_item")
		{
			if (static_cast<Item*>(b) != static_cast<Player*>(a)->heldItem) {
				return true;
			}
			else {//so player doesn't get hit by their own weapon
				return false;
			}
		}
		else if (a->getName() == "held_item" && b->getName() == "player") {
			if (static_cast<Item*>(a) != static_cast<Player*>(b)->heldItem) {
				return true;
			}
			else {//so player doesn't get hit by their own weapon
				return false;
			}
		}

		//check if player can pick up item
		if (a->getName() == "player" && b->getName() == "item_radius")
		{
			//CCLOG("CAN PICK UP ITEM");
			player->itemToPickUp = static_cast<Item*>(b->getParent());
			static_cast<Item*>(b->getParent())->playerRange = true;
			return false;
		}
		else if (a->getName() == "item_radius" && b->getName() == "player")
		{
			//CCLOG("CAN PICK UP ITEM");
			player->itemToPickUp = static_cast<Item*>(a->getParent());
			static_cast<Item*>(a->getParent())->playerRange = true;
			return false;
		}

		//player and item
		if ((a->getName() == "player" && b->getName() == "item") || (a->getName() == "item" && b->getName() == "player"))
		{
			return false;
		}

		//player and hide object
		if (a->getName() == "player" && b->getName() == "env_object")
		{
			player->objectToHideBehind = static_cast<HideObject*>(b);
			static_cast<HideObject*>(b)->playerRange = true;
			return false;
		}
		else if (a->getName() == "env_object" && b->getName() == "player")
		{
			player->objectToHideBehind = static_cast<HideObject*>(a);
			static_cast<HideObject*>(a)->playerRange = true;
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

		//player and door
		if (a->getName() == "player" && b->getName() == "door_radius")
		{
			//CCLOG("CAN OPEN DOOR");
			player->doorToUse = static_cast<Door*>(b->getParent());
			static_cast<Door*>(b->getParent())->playerRange = true;
			return false;
		}
		else if (a->getName() == "door_radius" && b->getName() == "player")
		{
			//CCLOG("CAN OPEN DOOR");
			player->doorToUse = static_cast<Door*>(a->getParent());
			static_cast<Door*>(a->getParent())->playerRange = true;
			return false;
		}

		//enemy and item radius
		if ((a->getName() == "enemy" && b->getName() == "item_radius") || (a->getName() == "item_radius" && b->getName() == "enemy"))
		{
			return false;
		}
		//alert enemy and item radius
		if (a->getName() == "enemy_alert" && b->getName() == "item_radius")
		{
			static_cast<Enemy*>(a)->itemToPickUp = static_cast<Item*>(b->getParent());
			return false;
		}
		else if (a->getName() == "item_radius" && b->getName() == "enemy_alert")
		{
			static_cast<Enemy*>(b)->itemToPickUp = static_cast<Item*>(a->getParent());
			return false;
		}

		//enemy and door radius
		if (a->getName() == "enemy" && b->getName() == "door_radius")
		{
			return false;
		}
		else if (a->getName() == "door_radius" && b->getName() == "enemy")
		{
			return false;
		}
		//alert enemy and door radius
		if (a->getName() == "enemy_alert" && b->getName() == "door_radius")
		{
			return false;
		}
		else if (a->getName() == "door_radius" && b->getName() == "enemy_alert")
		{
			return false;
		}

	}
	return true;
}

//check if 2 objects collide and do something
bool Level::onContactBegin(cocos2d::PhysicsContact &contact){
	Node *a = contact.getShapeA()->getBody()->getNode();
	Node *b = contact.getShapeB()->getBody()->getNode();

	//check if enemy has been hit by player's attack
	if ((a->getName() == "enemy" || a->getName() == "enemy_alert") && b->getName() == "held_item")
	{
		if (static_cast<Item*>(b) != static_cast<Enemy*>(a)->heldItem && static_cast<Item*>(b)->enemyItem == false) {//only get hit by non-enemy attacks
			static_cast<Enemy*>(a)->itemHitBy = static_cast<Item*>(b);
			return true;
		}
		else {
			return false;
		}
	}
	else if (a->getName() == "held_item" && (b->getName() == "enemy" || b->getName() == "enemy_alert"))
	{
		if (static_cast<Item*>(a) != static_cast<Enemy*>(b)->heldItem && static_cast<Item*>(a)->enemyItem == false) {//only get hit by non-enemy attacks
			static_cast<Enemy*>(b)->itemHitBy = static_cast<Item*>(a);
			return true;
		}
		else {
			return false;
		}
	}

	//player and held item
	if (a->getName() == "player" && b->getName() == "held_item")
	{
		if (static_cast<Item*>(b) != static_cast<Player*>(a)->heldItem) {//so player doesn't get hit by their own weapon
																		 //get hit
			static_cast<Player*>(a)->itemHitBy = (static_cast<Item*>(b));
			return true;
		}
		else {
			return false;
		}
	}
	else if (a->getName() == "held_item" && b->getName() == "player") {
		if (static_cast<Item*>(a) != static_cast<Player*>(b)->heldItem) {//so player doesn't get hit by their own weapon
																		 //get hit
			static_cast<Player*>(b)->itemHitBy = (static_cast<Item*>(a));
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
	//enemy and wall
	if (a->getName() == "enemy_alert" && b->getName() == "wall")
	{
		static_cast<Enemy*>(a)->hitWall();
		return true;
	}
	else if (a->getName() == "wall" && b->getName() == "enemy_alert")
	{
		static_cast<Enemy*>(b)->hitWall();
		return true;
	}
	//enemy and wall
	if (a->getName() == "enemy" && b->getName() == "wall")
	{
		static_cast<Enemy*>(a)->hitWall();
		return true;
	}
	else if (a->getName() == "wall" && b->getName() == "enemy")
	{
		static_cast<Enemy*>(b)->hitWall();
		return true;
	}

	//items and door
	if (a->getName() == "held_item" && b->getName() == "door")
	{
		static_cast<Item*>(a)->hitWall();
		static_cast<Door*>(b)->itemHit(static_cast<Item*>(a));
		return true;
	}
	else if (a->getName() == "door" && b->getName() == "held_item")
	{
		static_cast<Item*>(b)->hitWall();
		static_cast<Door*>(a)->itemHit(static_cast<Item*>(b));
		return true;
	}
	//items and walls
	if (a->getName() == "held_item" && b->getName() == "wall")
	{
		static_cast<Item*>(a)->hitWall();
		return true;
	}
	else if (a->getName() == "wall" && b->getName() == "held_item")
	{
		static_cast<Item*>(b)->hitWall();
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

void Level::createFloor(Vec2 position, vector<RoomData> *roomData, int height)
{
	Room* room;

	for (int i = 0; i < (*roomData).size(); i++) {
		(*roomData)[i].num = i;//getting room's number
		(*roomData)[i].left = position.x;//getting room's left side position
		(*roomData)[i].right = position.x + (*roomData)[i].width;//getting room's right side position

		room = Room::create();
		room->createRoom(&doors, &mainLayer->stairs, &hideObjects, &mainLayer->items, &enemies, &pathNodes, player, position, (*roomData)[i], height);

		position = position + Vec2((*roomData)[i].width + room->fullThick, 0);//adding length of created room to set position for next room

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
			totalWidth += mainLayer->floors[i].rooms[j].width;
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

		RoomData roomData;

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
				roomData.width = std::atoi(chunk.c_str());
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
				roomData.bgName = pieces[1];
			}
			else if (pieces[0] == "door") {
				DoorData doorData;
				doorData.type = 1;
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
					roomData.rightDoors.push_back(doorData);
				}
				else if (pieces[1] == "left") {
					roomData.leftDoors.push_back(doorData);
				}
				else if (pieces[1] == "top") {
					roomData.ceilingDoors.push_back(doorData);
				}
				else if (pieces[1] == "bot") {
					roomData.bottomDoors.push_back(doorData);
				}
			}
			else if (pieces[0] == "vent") {
				DoorData ventData;
				ventData.type = 2;
				if (pieces.size() >= 2) {//set vent's position on wall
					ventData.pos = atof(pieces[2].c_str());
				}
				//set vent as locked
				if (pieces.size() >= 3) {
					if (pieces[3] == "locked") {
						ventData.locked = true;
					}
				}
				//set what wall vent is on
				if (pieces[1] == "right") {
					roomData.rightDoors.push_back(ventData);
				}
				else if (pieces[1] == "left") {
					roomData.leftDoors.push_back(ventData);
				}
				else if (pieces[1] == "top") {
					roomData.ceilingDoors.push_back(ventData);
				}
				else if (pieces[1] == "bot") {
					roomData.bottomDoors.push_back(ventData);
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
			//Items
			else if (pieces[0] == "item") {
				Item* item;
				if (pieces[1] == "knife") {
					item = Knife::createWithSpriteFrameName();
					item->retain();
				}
				else if (pieces[1] == "key") {
					item = Key::createWithSpriteFrameName();
					item->retain();
				}
				else if (pieces[1] == "hammer") {
					item = Hammer::createWithSpriteFrameName();
					item->retain();
				}
				item->initObject();
				item->roomStartPos = Vec2(atof(pieces[2].c_str()), atof(pieces[3].c_str()));
				item->startRoom = Vec2(roomNum, floorNum);
				mainLayer->items.push_back(item);
			}
			//Enemies
			else if (pieces[0] == "enemy") {
				Enemy* enemy;
				if (pieces[1] == "guard") {
					enemy = Enemy::createWithSpriteFrameName();//should be Guard subclass
				}
				enemy->roomStartPos = Vec2(atof(pieces[2].c_str()), atof(pieces[3].c_str()));
				enemy->startRoom = Vec2(roomNum, floorNum);
				enemy->currentFloor = floorNum;
				if (pieces.size() > 4) {
					enemy->setPathTag(pieces[4]);//"STAND_LEFT", "STAND_RIGHT", "STAND_SWITCH",  indicate the enemy will stand still facing that direction, switch turns in both direcitons
				}
				if (pieces.size() > 5) {
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
					item->initObject();
					item->roomStartPos = Vec2(atof(pieces[2].c_str()), atof(pieces[3].c_str()));
					item->startRoom = Vec2(roomNum, floorNum);
					mainLayer->items.push_back(item);
					enemy->itemToPickUp = item;
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

		roomData.room = Vec2(roomNum, floorNum);
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
		doors[i]->setTag(doors[i]->getTag() + i);//giving a unique tag to each door
		mainLayer->addChild(doors[i]);
	}
	//stairways
	for (int i = 0; i < mainLayer->stairs.size(); i++) {
		mainLayer->addChild(mainLayer->stairs[i]);//Do Not give unique tag to each stairway, already done elsewhere
	}
	//hide objects
	for (int i = 0; i < hideObjects.size(); i++) {
		hideObjects[i]->setTag(hideObjects[i]->getTag() + i);//giving a unique tag to each object
		mainLayer->addChild(hideObjects[i]);
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

	return true;
}
