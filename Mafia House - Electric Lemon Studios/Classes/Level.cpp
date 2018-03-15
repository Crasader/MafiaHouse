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
	mainLayer = Node::create();
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

	//for running the update function
	schedule(schedule_selector(Level::onStart));
}

void Level::onStart(float deltaTime){
	unschedule(schedule_selector(Level::onStart));

	//physics debug drawing:
	getScene()->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

	//deleting layer's default camera, or else there will be a double scene drawn
	getScene()->getDefaultCamera()->removeFromParentAndCleanup(true);
	//getting scene's default camera
	camera = getDefaultCamera();
	//setting 'zoom' level using Z positoin of camera
	camera->setPositionZ(459.42983 / camZoom);

	//scheduling update to start running after this
	scheduleUpdate();
}

void Level::resetCollisionChecks() {
	enemyHit = -1;
}

void Level::update(float deltaTime){
	//updating time
	gameTime += deltaTime;

	if (player->caught == true){
		resetLevel();
	}

	//for drawing vision rays
	if (visionRays)
	{
		removeChild(visionRays, true);
	}
	visionRays = DrawNode::create();
	visionRays->setGlobalZOrder(10);

	//enemy update
	vector<Vec2> points;
	Vec2 start;
	for (int i = 0; i < enemies.size(); i++) {
		if (enemyHit != -1) {//temporary
			if (enemies[i]->getTag() == enemyHit) {
				//enemies[i]->kill();
				mainLayer->removeChild(enemies[i]);
				enemies.erase(enemies.begin() + i);
				i--;
				continue;
			}
		}
		//enemy vision:
		enemies[i]->visionRays(&points, &start);
		//drawing vision rays
		for (int j = 0; j < points.size(); j++) {
			//visionRays->drawDot(points[j], 1, Color4F::WHITE);
			visionRays->drawSegment(start, points[j], 2, Color4F(1,0.9,0.1,0.2));
		}
		points.clear();

		enemies[i]->update(mainLayer, gameTime, player);
	}
	addChild(visionRays);

	//player update:
	player->update(mainLayer, gameTime);

	//check if player is going to interact with door/stairway
	if (INPUTS->getKeyPress(KeyCode::KEY_Q)) {
		if (player->stairToUse != -1) {
			player->handleInput(mainLayer, gameTime, USE_STAIR);
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/stairs.wav");
			player->handleInput(USE_STAIR, mainLayer);
		}
		else if (player->doorToUse != -1) {
			player->handleInput(mainLayer, gameTime, USE_DOOR);
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/openDoor.wav");
			player->handleInput(USE_DOOR, mainLayer);
		}
	}
	//check if player is dropping item
	if (INPUTS->getKeyPress(KeyCode::KEY_F)) {
		if (player->heldItem != NULL) {
			player->handleInput(mainLayer, gameTime, DROP);
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/unequip.wav");
			player->handleInput(DROP, mainLayer);
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
		if (player->itemToPickUp != -1 && player->heldItem == NULL) {
			player->handleInput(mainLayer, gameTime, PICKUP);
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/equip.wav");
			player->handleInput(PICKUP, mainLayer);
		}
	}
	//check if player is going to hide behind object
	if (INPUTS->getKeyPress(KeyCode::KEY_CTRL)) {
		if (player->objectToHideBehind != -1) {
			player->handleInput(mainLayer, gameTime, HIDE);
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/hide.wav");
			player->handleInput(HIDE, mainLayer);
		}
	}

	//change between standing/crouching
	if (INPUTS->getKey(KeyCode::KEY_W)) {
		player->handleInput(mainLayer, gameTime, MOVE_UP);
	}
	if (INPUTS->getKey(KeyCode::KEY_S)) {
		player->handleInput(mainLayer, gameTime, MOVE_DOWN);
	}

	//player movement input checking for sounds effect
	if (INPUTS->getKeyPress(KeyCode::KEY_D) || INPUTS->getKeyPress(KeyCode::KEY_A))
	{
		walkingID = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/walk.wav", true);
	}
	if ((INPUTS->getKeyRelease(KeyCode::KEY_D) && !(INPUTS->getKey(KeyCode::KEY_A))) || (INPUTS->getKeyRelease(KeyCode::KEY_A) && !(INPUTS->getKey(KeyCode::KEY_D)))) 
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(walkingID);
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
	player->resetActionChecks();
	resetCollisionChecks();

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
		if ((a->getName() == "player" && b->getName() == "enemy") || (a->getName() == "enemy" && b->getName() == "player"))
		{
			if (player->hidden != true) {
				CCLOG("YOU TOUCHED AN ENEMY");
				//return true;
				//solve.setRestitution(0.0f);
				return false;
			}
			else {
				CCLOG("YOU AVOIDED DETECTION");
				return false;
			}
		}
		// check if player has collided with an enemy
		if ((a->getName() == "player" && b->getName() == "enemy_alert") || (a->getName() == "enemy_alert" && b->getName() == "player"))
		{
			if (player->hidden != true) {
				CCLOG("YOU TOUCHED AN ENEMY");
				player->caught = true;
				solve.setRestitution(0.0f);
				return true;
			}
			else {
				CCLOG("YOU AVOIDED DETECTION");
				return false;
			}
		}

		//check if enemy has been hit by player's attack
		if (a->getName() == "enemy" && b->getName() == "held_item")
		{
			CCLOG("ENEMY HIT");
			enemyHit = a->getTag();
			return true;
		}
		else if (a->getName() == "held_item" && b->getName() == "enemy")
		{
			CCLOG("ENEMY HIT");
			enemyHit = b->getTag();
			return true;
		}

		//check if player can pick up item
		if (a->getName() == "player" && b->getName() == "item_radius")
		{
			CCLOG("CAN PICK UP ITEM");
			player->itemToPickUp = b->getParent()->getTag();
			return false;
		}
		else if (a->getName() == "item_radius" && b->getName() == "player")
		{
			CCLOG("CAN PICK UP ITEM");
			player->itemToPickUp = a->getParent()->getTag();
			return false;
		}

		//player and item
		if ((a->getName() == "player" && b->getName() == "item") || (a->getName() == "item" && b->getName() == "player"))
		{
			return false;
		}

		//player and held item
		if ((a->getName() == "player" && b->getName() == "held_item") || (a->getName() == "held_item" && b->getName() == "player"))
		{
			return false;
		}

		//player and env. object
		if (a->getName() == "player" && b->getName() == "env_object")
		{
			CCLOG("CAN HIDE BEHIND THING");
			player->objectToHideBehind = b->getTag();
			return false;
		}
		else if (a->getName() == "env_object" && b->getName() == "player")
		{
			CCLOG("CAN HIDE BEHIND THING");
			player->objectToHideBehind = a->getTag();
			return false;
		}

		//player and stairway
		if (a->getName() == "player" && b->getName() == "stair")
		{
			CCLOG("CAN GO THROUGH STAIRWAY");
			player->stairToUse = b->getTag();
			return false;
		}
		else if (a->getName() == "stair" && b->getName() == "player")
		{
			CCLOG("CAN GO THROUGH STAIRWAY");
			player->stairToUse = a->getTag();
			return false;
		}

		//player and door
		if (a->getName() == "player" && b->getName() == "door_radius")
		{
			CCLOG("CAN OPEN DOOR");
			player->doorToUse = b->getParent()->getTag();
			return false;
		}
		else if (a->getName() == "door_radius" && b->getName() == "player")
		{
			CCLOG("CAN OPEN DOOR");
			player->doorToUse = a->getParent()->getTag();
			return false;
		}

	}
	return true;
}

//check if 2 objects collide and do something
bool Level::onContactBegin(cocos2d::PhysicsContact &contact){
	/*PhysicsBody *a = contact.getShapeA()->getBody();
	PhysicsBody *b = contact.getShapeB()->getBody();

	// check if player has collided with a vision cone
	if ((a->getTag() == 1 && b->getTag() == 2) || (b->getTag() == 1 && a->getTag() == 2))
	{
	CCLOG("BEGIN COLLISION HAS OCCURED");
	}*/

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

void Level::createFloor(vector<Room*> *rooms, vector<Door*> *doors, vector<Stair*> *stairs, vector<EnvObject*> *objects, vector<Item*> *items, vector<Enemy*> *enemies, Player* player, Vec2 position, vector<RoomData> roomData, int height)
{
	Room* room;

	for (int i = 0; i < roomData.size(); i++) {
		room = Room::create();

		room->createRoom(doors, stairs, objects, items, enemies, player, position, roomData[i], height);

		position = position + Vec2(roomData[i].width + room->fullThick, 0);//adding length of created room to set position for next room

		rooms->push_back(room);
	}
}

void Level::createLevel(vector<Room*> *rooms, vector<Door*> *doors, vector<Stair*> *stairs, vector<EnvObject*> *objects, vector<Item*> *items, vector<Enemy*> *enemies, Player* player, Vec2 position, float levelWidth, vector<FloorData> floorData)
{
	Room r;

	//calculating width differences between floors and first floor
	int firstFloorWidth = 0;
	vector<int> floorOffsets;
	for (int i = 0; i < floorData.size(); i++) {
		int totalWidth = 0;
		for (int j = 0; j < floorData[i].rooms.size(); j++) {
			totalWidth += floorData[i].rooms[j].width;
		}
		if (i == 0) {
			firstFloorWidth = totalWidth;
		}
		floorOffsets.push_back((totalWidth - firstFloorWidth) / 2);
	}

	position = position + Vec2((levelWidth / 2) - (firstFloorWidth / 2) + r.thick, r.thick);//setting position to be in centre of background

	//generating floors
	for (int i = 0; i < floorData.size(); i++) {
		createFloor(rooms, doors, stairs, objects, items, enemies, player, position - Vec2(floorOffsets[i], 0), floorData[i].rooms, floorData[i].height);

		position = position + Vec2(0, floorData[i].height + r.fullThick);//adding height of created floor to set position for next floor
	}
}

bool Level::initLevel(string filename){
	//used to store data to pass into createLevel function
	vector<FloorData> floors;
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
		RoomData roomData;

		if (line[0] == '#') { continue; }//ignoring comment lines, which begin with the '#' character

		if (line == "FLOOR_END") {//indicates to move to next floor in floors vector
			floors.push_back(floorData);
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
				if (pieces.size() > 2) {//set door's position on wall
					doorData.pos = atof(pieces[2].c_str());
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
				if (pieces.size() > 2) {//set vent's position on wall
					ventData.pos = atof(pieces[2].c_str());
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
			else if (pieces[0] == "stair") {
				Stair* stair = Stair::create();
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
				stairs.push_back(stair);
			}
			else if (pieces[0] == "object") {
				EnvObject* object;
				if (pieces[1] == "plant") {
					object = EnvObject::create();//should be Plant subclass
				}
				object->initObject();
				object->roomStartPos = Vec2(atof(pieces[2].c_str()), atof(pieces[3].c_str()));
				object->startRoom = Vec2(roomNum, floorNum);
				objects.push_back(object);
			}
			else if (pieces[0] == "item") {
				Item* item;
				if (pieces[1] == "knife") {
					item = Knife::create();
				}
				item->initObject();
				item->roomStartPos = Vec2(atof(pieces[2].c_str()), atof(pieces[3].c_str()));
				item->startRoom = Vec2(roomNum, floorNum);
				items.push_back(item);
			}
			else if (pieces[0] == "enemy") {
				Enemy* enemy;
				if (pieces[1] == "guard") {
					enemy = Enemy::create();//should be Guard subclass
				}
				enemy->initObject();
				enemy->roomStartPos = Vec2(atof(pieces[2].c_str()), atof(pieces[3].c_str()));
				enemy->startRoom = Vec2(roomNum, floorNum);
				enemies.push_back(enemy);
			}
			else if (pieces[0] == "player") {//set player position in room
				player->roomStartPos = Vec2(atof(pieces[1].c_str()), atof(pieces[2].c_str()));
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
	createLevel(&rooms, &doors, &stairs, &objects, &items, &enemies, player, background->getPosition(), background->getContentSize().width * backgroundScale, floors);
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
	for (int i = 0; i < stairs.size(); i++) {
		mainLayer->addChild(stairs[i]);//Do Not give unique tag to each stairway, already done elsewhere
	}
	//objects
	for (int i = 0; i < objects.size(); i++) {
		objects[i]->setTag(objects[i]->getTag() + i);//giving a unique tag to each object
		mainLayer->addChild(objects[i]);
	}
	//items
	for (int i = 0; i < items.size(); i++) {
		items[i]->setTag(items[i]->getTag() + i);//giving a unique tag to each item
		mainLayer->addChild(items[i]);
	}
	//enemies
	for (int i = 0; i < enemies.size(); i++) {
		enemies[i]->setTag(enemies[i]->getTag() + i);//giving a unique tag to each enemy
		mainLayer->addChild(enemies[i]);
	}

	return true;
}
