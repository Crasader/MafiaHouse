#include "Level.h"



Level::Level()
{

}


Level::~Level()
{
}

void Level::onStart(float dt)
{
	this->unschedule(schedule_selector(Level::onStart));
	camera = this->getScene()->getDefaultCamera();
	camera->setPositionZ(500 / camZoom);

}

void Level::update(float deltaTime)
{
	//clearing 'inputs'
	ctrl_press = false;
	space_press = false;

	//checking to see if player is picking up an item
	player->pickUpItem();

	//player movement input checking
	if (INPUTS->getKey(KeyCode::KEY_D)) {
		//player->getPhysicsBody()->applyImpulse(Vec2(6000, 0));
		player->move(Vec2(25.0f, 0));
		if (player->flipped == true) {
			player->flipped = false;
			player->flip();
		}
	}
	if (INPUTS->getKey(KeyCode::KEY_A)) {
		//player->getPhysicsBody()->applyImpulse(Vec2(-6000, 0));
		player->move(Vec2(-25.0f, 0));
		if (player->flipped == false) {
			player->flipped = true;
			player->flip();
		}
	}
	if (INPUTS->getKey(KeyCode::KEY_W)) {
		player->move(Vec2(0, 25.0f));
	}
	if (INPUTS->getKey(KeyCode::KEY_S)) {
		player->move(Vec2(0, -25.0f));
	}
	if (INPUTS->getKeyRelease(KeyCode::KEY_D) || INPUTS->getKeyRelease(KeyCode::KEY_A)){
		player->getPhysicsBody()->setVelocity(Vec2(0, 0) );
	}

	//picking up items
	if (INPUTS->getKeyPress(KeyCode::KEY_SPACE)) {
		space_press = true;
	}
	//hiding
	if (INPUTS->getKeyPress(KeyCode::KEY_CTRL)) {
		ctrl_press = true;
	}

	//have player stay behind object they are hiding behind
	if (player->hidden == true) {
		auto hideObject = mainLayer->getChildByTag(player->objectHidingBehind);
		if (player->hideStart == true) {
			player->setGlobalZOrder(player->getGlobalZOrder() - 3);
			if (player->heldItem != NULL) {
				player->heldItem->setGlobalZOrder(player->heldItem->getGlobalZOrder() - 3);
			}
			hideObject->setOpacity(180);
			player->hideStart = false;
		}
		followBox(player, hideObject, Vec2((hideObject->getContentSize().width / 2.0f) - (player->getContentSize().width / 2.0f), hideObject->getContentSize().height / 2.0f), Vec2((hideObject->getContentSize().width / 2.0f) - (player->getContentSize().width / 2.0f), hideObject->getContentSize().height / 2.0f));
	}
	else {
		if (player->objectHidingBehind != -1) {
			mainLayer->getChildByTag(player->objectHidingBehind)->setOpacity(225);
			player->objectHidingBehind = -1;
			player->setGlobalZOrder(player->getGlobalZOrder() + 3);
			if (player->heldItem != NULL) {
				player->heldItem->setGlobalZOrder(player->heldItem->getGlobalZOrder() + 3);
			}
		}
	}

	//open and close doors
	for (int i = 0; i < doors.size(); i++) {
		if (doors[i]->getTag() == player->doorToUse) {
			doors[i]->use();
			player->doorToUse = -1;
		}
	}

	//use stairs
	for (int i = 0; i < stairs.size(); i++) {
		if (stairs[i]->getTag() == player->stairEntered) {
			if (stairs[i]->type == 1) {
				player->setPosition(mainLayer->getChildByTag(stairs[i]->getTag() + 1000)->getPosition());
			}
			else if (stairs[i]->type == 2) {
				player->setPosition(mainLayer->getChildByTag(stairs[i]->getTag() - 1000)->getPosition());
			}
			player->stairEntered = -1;
		}
	}

	//camOffset = Vec2(0, 150 / camZoom);//adjusting camera offset with zoom level?
	//having camera 'chase' player
	followBox(camPos, player, camBoundingBox, camOffset);
	if (camera != NULL) {
		camera->setPosition(camPos->getPosition());
	}

	//update the keyboard each frame
	INPUTS->clearForNextFrame();
}

//check if 2 objects are touching, do something and set whether they collide or not after
bool Level::onContactPreSolve(PhysicsContact &contact, PhysicsContactPreSolve & solve) {
	//solve.setRestitution(10.0);
	Node *a = contact.getShapeA()->getBody()->getNode();
	Node *b = contact.getShapeB()->getBody()->getNode();

	if (a != NULL && b != NULL) {

		// check if player has collided with a vision cone
		if ((a->getName() == "player" && b->getName() == "vision_cone") || (a->getName() == "vision_cone" && b->getName() == "player"))
		{
			if (player->hidden != true) {
				CCLOG("YOU HAVE BEEN SPOTTED");
			}
			else {
				CCLOG("YOU AVOIDED DETECTION");
			}
			return false;
		}

		// check if player has collided with an enemy
		if ((a->getName() == "player" && b->getName() == "enemy") || (a->getName() == "enemy" && b->getName() == "player"))
		{
			if (player->hidden != true) {
				CCLOG("YOU TOUCHED AN ENEMY");
				//return true;
				return false;
			}
			else {
				CCLOG("YOU AVOIDED DETECTION");
				return false;
			}
		}

		//check if player can pick up item
		if (a->getName() == "player" && b->getName() == "item_radius")
		{
			CCLOG("CAN PICK UP ITEM");
			if (space_press == true) {
				player->itemToPickUp = b->getParent()->getTag();
				b->getParent()->removeFromParent();
			}
			return false;
		}
		else if (a->getName() == "item_radius" && b->getName() == "player")
		{
			CCLOG("CAN PICK UP ITEM");
			if (space_press == true) {
				player->itemToPickUp = a->getParent()->getTag();
				a->getParent()->removeFromParent();
			}
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
			if (ctrl_press == true) {
				if (player->hidden == false) {
					player->hideStart = true;
					player->hidden = true;
					player->objectHidingBehind = b->getTag();
				}
				else {
					player->hidden = false;
				}
			}
			return false;
		}
		else if (a->getName() == "env_object" && b->getName() == "player")
		{
			CCLOG("CAN HIDE BEHIND THING");

			if (ctrl_press == true) {
				if (player->hidden == false) {
					player->hideStart = true;
					player->hidden = true;
					player->objectHidingBehind = a->getTag();
				}
				else {
					player->hidden = false;
				}
			}
			return false;
		}

		//player and stairway
		if (a->getName() == "player" && b->getName() == "stair")
		{
			CCLOG("CAN GO THROUGH STAIRWAY");
			if (ctrl_press == true) {
				player->stairEntered = b->getTag();
			}
			return false;
		}
		else if (a->getName() == "stair" && b->getName() == "player")
		{
			CCLOG("CAN GO THROUGH STAIRWAY");
			if (ctrl_press == true) {
				player->stairEntered = a->getTag();
			}
			return false;
		}

		//player and door
		if (a->getName() == "player" && b->getName() == "door_radius")
		{
			CCLOG("CAN OPEN DOOR");
			if (ctrl_press == true) {
				player->doorToUse = b->getParent()->getTag();
			}
			return false;
		}
		else if (a->getName() == "door_radius" && b->getName() == "player")
		{
			CCLOG("CAN OPEN DOOR");
			if (ctrl_press == true) {
				player->doorToUse = a->getParent()->getTag();
			}
			return false;
		}

	}
	return true;
}

//check if 2 objects collide and do something
bool Level::onContactBegin(cocos2d::PhysicsContact &contact)
{
	/*PhysicsBody *a = contact.getShapeA()->getBody();
	PhysicsBody *b = contact.getShapeB()->getBody();

	// check if player has collided with a vision cone
	if ((a->getTag() == 1 && b->getTag() == 2) || (b->getTag() == 1 && a->getTag() == 2))
	{
	CCLOG("BEGIN COLLISION HAS OCCURED");
	}*/

	return true;
}

void Level::followRadius(Node* nodeA, Node* nodeB, float radius, Vec2 offset) {
	Vec2 displacement = nodeA->getPosition() - (nodeB->getPosition() + offset);
	float distance = displacement.getLength();
	if (distance > radius) {
		nodeA->setPosition((nodeB->getPosition() + offset) + (displacement.getNormalized() * radius));
	}
}

void Level::followBox(Node* nodeA, Node* nodeB, Vec2 range, Vec2 offset) {
	Vec2 displacement = nodeA->getPosition() - (nodeB->getPosition() + offset);
	if (displacement.x > range.x) {
		nodeA->setPositionX((nodeB->getPositionX() + offset.x) + range.x);
	}
	else if (displacement.x < -range.x) {
		nodeA->setPositionX((nodeB->getPositionX() + offset.x) - range.x);
	}
	if (displacement.y > range.y) {
		nodeA->setPositionY((nodeB->getPositionY() + offset.y) + range.y);
	}
	else if (displacement.y < -range.y) {
		nodeA->setPositionY((nodeB->getPositionY() + offset.y) - range.y);
	}
}

void Level::createFloor(vector<Room*> *rooms, vector<Door*> *doors, vector<Stair*> *stairs, vector<EnvObject*> *objects, vector<Item*> *items, vector<Enemy*> *enemies, Player* player, Vec2 position, vector<RoomData> roomData, int height)
{
	Room* room;

	for (int i = 0; i < roomData.size(); i++) {
		room = Room::create();

		room->createRoom(doors, stairs, objects, items, enemies, player, position, roomData[i].width, height, roomData[i].door, roomData[i].room);

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

void Level::setup()
{
	//node everything in level is attached to
	mainLayer = Node::create();
	this->addChild(mainLayer);

	//initializing player
	player = Player::create();
	player->initObject();
	mainLayer->addChild(player);

	//Invisible Node for the camera to follow
	camPos = Node::create();
	mainLayer->addChild(camPos);

	//necessary for collision detection
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Level::onContactBegin, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

	contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactPreSolve = CC_CALLBACK_2(Level::onContactPreSolve, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

	//for running the update function
	this->schedule(schedule_selector(Level::onStart));

	this->scheduleUpdate();
}

void Level::setBackground(string bgName, float scale) {
	backgroundScale = scale;
	//setting background image
	background = Sprite::create(bgName);
	background->setAnchorPoint(Vec2(0, 0));
	// position the sprite on the center of the screen
	auto visibleSize = director->getVisibleSize();
	background->setPosition(Vec2((visibleSize.width / 2) - (background->getContentSize().width / 2), 0));
	//background->setPosition(Vec2(0, 0));
	background->setScale(backgroundScale);
	mainLayer->addChild(background);

	//creating collision box on edge of game area
	auto border = PhysicsBody::createEdgeBox(background->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT, 1.0f, Vec2(0, 0));
	border->setDynamic(false);
	background->addComponent(border);
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
			if (pieces[0] == "door") {
				if (pieces[1] == "right") {
					roomData.door = 1;
				}
				else if (pieces[1] == "both") {
					roomData.door = 2;
				}
				else if (pieces[1] == "left") {
					roomData.door = 3;
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
		//guard moves automatically, put this into Enemy class
		auto movement = MoveBy::create(5, Vec2(400, 0));
		auto turn = ScaleBy::create(0.0f, -1.0f, 1.0f);
		auto wait = MoveBy::create(0.5, Vec2(0, 0));
		auto moveback = MoveBy::create(5, Vec2(-400, 0));
		auto sequence = Sequence::create(movement, wait, turn, moveback, wait, turn, NULL);
		enemies[i]->runAction(RepeatForever::create(sequence));
	}

	return true;
}