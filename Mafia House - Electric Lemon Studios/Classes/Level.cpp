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
		player->getPhysicsBody()->applyImpulse(Vec2(6000, 0));
		if (player->flipped == true) {
			player->flipped = false;
			player->flip();
		}
	}
	if (INPUTS->getKey(KeyCode::KEY_A)) {
		player->getPhysicsBody()->applyImpulse(Vec2(-6000, 0));
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

	//having camera 'chase' player
	follow(camPos, player, 200.0f, camOffset);

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
	if ((a->getTag() == 1 && b->getTag() == 2) || (b->getTag() == 1 && a->getTag() == 2))
	{
		CCLOG("YOU HAVE BEEN SPOTTED");
		return false;
	}

	//check if player can pick up item
	if (a->getTag() == 1 && (b->getTag() >= 100 && b->getTag() <= 199))
	{
		CCLOG("CAN PICK UP ITEM");
		if (space_press == true) {
			player->itemToPickUp = b->getTag();
			b->removeFromParent();
		}
		return false;
	}
	else if (b->getTag() == 1 && (a->getTag() >= 100 && a->getTag() <= 199))
	{
		CCLOG("CAN PICK UP ITEM");
		if (space_press == true) {
			player->itemToPickUp = a->getTag();
			a->removeFromParent();
		}
		return false;
	}

	//player and held item
	if ((a->getTag() == 1 && (b->getTag() >= 200 && b->getTag() <= 299)) || (b->getTag() == 1 && (a->getTag() >= 200 && a->getTag() <= 299)))
	{
		return false;
	}

	//player and env. object
	if (a->getTag() == 1 && (b->getTag() >= 300 && b->getTag() <= 399))
	{
		CCLOG("CAN HIDE BEHIND THING");
		if (ctrl_press == true) {
			if (player->hidden == false) {
				player->hidden = true;
			}
			else {
				player->hidden = false;
			}
		}
		return false;
	}
	else if (b->getTag() == 1 && (a->getTag() >= 300 && a->getTag() <= 399))
	{
		CCLOG("CAN HIDE BEHIND THING");

		if (ctrl_press == true) {
			if (player->hidden == false) {
				player->hidden = true;
			}
			else {
				player->hidden = false;
			}
		}
		return false;
	}

	return true;
}

void Level::follow(Node* nodeA, Node* nodeB, float radius, Vec2 offset) {
	Vec2 displacement = nodeA->getPosition() - (nodeB->getPosition() + offset);
	float distance = displacement.getLength();
	if (distance > radius) {
		nodeA->setPosition((nodeB->getPosition() + offset) + (displacement.getNormalized() * radius));
	}
}
