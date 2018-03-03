#include "Level.h"



Level::Level()
{
}


Level::~Level()
{
}

void Level::setup()
{
	//node everything in level is attached to
	mainLayer = Node::create();
	this->addChild(mainLayer);
	//mainLayer->setScale(0.25);//uncomment to zoom out the entire level, will break the physics badly

	//setting background image
	background = Sprite::create(backgroundName);
	//background->setContentSize(Size(1920, 1080));
	//background->setScale(0.5);
	background->setAnchorPoint(Vec2(0, 0));
	background->setPosition(0, 0);
	mainLayer->addChild(background);

	//creating collision box on edge of game area
	auto border = PhysicsBody::createEdgeBox(background->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT, 1.0f, Vec2(0, 0));
	border->setDynamic(false);
	background->addComponent(border);

	//initializing player
	player = Player::create();
	player->initObject();
	mainLayer->addChild(player);

	//Invisible Node for the camera to follow
	camPos = Node::create();
	mainLayer->addChild(camPos);
	//makes "camera" follow player
	mainLayer->runAction(Follow::create(camPos));

	//necessary for collision detection
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Level::onContactBegin, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

	contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactPreSolve = CC_CALLBACK_2(Level::onContactPreSolve, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

	//for running the update function
	this->scheduleUpdate();
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
		player->move(Vec2(10.0f, 0));
		if (player->flipped == true) {
			player->flipped = false;
			player->flip();
		}
	}
	if (INPUTS->getKey(KeyCode::KEY_A)) {
		//player->getPhysicsBody()->applyImpulse(Vec2(-6000, 0));
		player->move(Vec2(-10.0f, 0));
		if (player->flipped == false) {
			player->flipped = true;
			player->flip();
		}
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

	if (player->hidden == true){
		player->setGlobalZOrder(2);
		if (player->heldItem != NULL) {
			player->heldItem->setGlobalZOrder(2);
		}
	}
	else {
		player->setGlobalZOrder(4);
		if (player->heldItem != NULL) {
			player->heldItem->setGlobalZOrder(4);
		}
	}

	//have player stay behind object they are hiding behind
	if (player->hidden == true) {
		auto hideObject = mainLayer->getChildByTag(player->objectHidingBehind);
		if (player->hideStart == true) {
			hideObject->setOpacity(180);
			player->hideStart = false;
		}
		followBox(player, hideObject, Vec2(hideObject->getContentSize().width / 2.0f, hideObject->getContentSize().height / 2.0f), Vec2(hideObject->getContentSize().width / 2.0f, hideObject->getContentSize().height / 2.0f));
	}
	else {
		if (player->objectHidingBehind != -1) {
			mainLayer->getChildByTag(player->objectHidingBehind)->setOpacity(225);
			player->objectHidingBehind = -1;
		}
	}

	//open and close doors
	for (int i = 0; i < doors.size(); i++) {
		if (doors[i]->getTag() == player->doorToUse) {
			doors[i]->use();
			player->doorToUse = -1;
		}
	}

	//having camera 'chase' player
	followBox(camPos, player, camBoundingBox, camOffset);

	//update the keyboard each frame
	INPUTS->clearForNextFrame();
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

//check if 2 objects are touching, do something and set whether they collide or not after
bool Level::onContactPreSolve(PhysicsContact &contact, PhysicsContactPreSolve & solve) {
	//solve.setRestitution(10.0);
	Node *a = contact.getShapeA()->getBody()->getNode();
	Node *b = contact.getShapeB()->getBody()->getNode();
	
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

void Level::createFloor(vector<Room*> *rooms, vector<Door*> *doors, vector<Stair*> *stairs, vector<EnvObject*> *objects, vector<Item*> *items, vector<Enemy*> *enemies, Vec2 position, vector<RoomData> roomData, int height)
{
	Room* room;

	for (int i = 0; i < roomData.size(); i++) {
		room = Room::create();

		room->createRoom(doors, stairs, objects, items, enemies, position, roomData[i].width, height, roomData[i].door, roomData[i].stairs);

		position = position + Vec2(roomData[i].width + room->fullThick, 0);//adding length of created room to set position for next room

		rooms->push_back(room);
	}
}

void Level::createLevel(vector<Room*> *rooms, vector<Door*> *doors, vector<Stair*> *stairs, vector<EnvObject*> *objects, vector<Item*> *items, vector<Enemy*> *enemies, Vec2 position, float levelWidth, vector<FloorData> floorData)
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

	position = position + Vec2((levelWidth / 2) - (firstFloorWidth / 2), 0);//setting position to be in centre of background

	//generating floors
	for (int i = 0; i < floorData.size(); i++) {
		createFloor(rooms, doors, stairs, objects, items, enemies, position - Vec2(floorOffsets[i], 0), floorData[i].rooms, floorData[i].height);

		position = position + Vec2(0, floorData[i].height + r.fullThick);//adding height of created floor to set position for next floor
	}
}