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
	mainLayer->addChild(player, 2);

	//Invisible Node for the camera to follow
	camPos = Node::create();
	mainLayer->addChild(camPos);
	//makes "camera" follow player
	mainLayer->runAction(Follow::create(camPos, Rect::ZERO));

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
	player->pickUpItem();
	//player movement
	if (INPUTS->getKey(KeyCode::KEY_D)) {
		player->getPhysicsBody()->applyImpulse(Vec2(6000, 0));
		if (player->flipped == true) {
			player->flipped = false;
			player->flip();
			//player->setScaleX(player->getScaleX() * -1);
		}
	}
	if (INPUTS->getKey(KeyCode::KEY_A)) {
		player->getPhysicsBody()->applyImpulse(Vec2(-6000, 0));
		if (player->flipped == false) {
			player->flipped = true;
			player->flip();
			//player->setScaleX(player->getScaleX() * -1);
		}
	}
	if (INPUTS->getKeyRelease(KeyCode::KEY_D) || INPUTS->getKeyRelease(KeyCode::KEY_A)){
		player->getPhysicsBody()->setVelocity(Vec2(0, 0) );
	}

	//positioning camera node to be below player (adding it as a child of player doesn't work)
	//camPos->setPosition(player->getPosition() - Vec2(0, -400));
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

bool Level::onContactPreSolve(PhysicsContact &contact, PhysicsContactPreSolve & solve) {
	//solve.setRestitution(10.0);
	PhysicsBody *a = contact.getShapeA()->getBody();
	PhysicsBody *b = contact.getShapeB()->getBody();
	
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
		if (INPUTS->getKey(KeyCode::KEY_SPACE)) {
			player->itemToPickUp = b->getTag();
			b->getNode()->removeFromParent();
		}
		return false;
	}
	else if (b->getTag() == 1 && (a->getTag() >= 100 && a->getTag() <= 199))
	{
		CCLOG("CAN PICK UP ITEM");
		if (INPUTS->getKey(KeyCode::KEY_SPACE)) {
			player->itemToPickUp = a->getTag();
			a->getNode()->removeFromParent();
		}
		return false;
	}

	//player and held item
	if (a->getTag() == 1 && (b->getTag() >= 200 && b->getTag() <= 299))
	{
		return false;
	}
	else if (b->getTag() == 1 && (a->getTag() >= 200 && a->getTag() <= 299))
	{
		return false;
	}
	
	//CCLOG(" ");

	return true;
}

void Level::follow(Node* nodeA, Node* nodeB, float radius, Vec2 offset) {
	Vec2 displacement = nodeA->getPosition() - (nodeB->getPosition() + offset);
	float distance = displacement.getLength();
	if (distance > radius) {
		nodeA->setPosition((nodeB->getPosition() + offset) + (displacement.getNormalized() * radius));
	}
}
