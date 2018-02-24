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

	//Invisible Node for the camera to follow
	camPos = Node::create();
	mainLayer->addChild(camPos);
	//makes "camera" follow player
	mainLayer->runAction(Follow::create(camPos, Rect::ZERO));

	//setting background image
	background = Sprite::create(backgroundName);
	//background->setContentSize(Size(1920, 1080));
	background->setAnchorPoint(Vec2(0, 0));
	background->setPosition(0, 0);
	background->setScale(0.5);
	mainLayer->addChild(background);

	//creating collision box on edge of game area
	auto border = PhysicsBody::createEdgeBox(background->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT, 1.0f, Vec2(0, 0));
	border->setDynamic(false);
	background->addComponent(border);

	//initializing player
	player = Player::create();
	player->initObject();
	mainLayer->addChild(player, 2);

	//necessary for collision detection
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Level::onContactBegin, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

	//for running the update function
	this->scheduleUpdate();
}

void Level::update(float deltaTime)
{
	//player movement
	if (INPUTS->getKey(KeyCode::KEY_D)) {
		player->getPhysicsBody()->setVelocity(Vec2(100, 0));
		if (player->isFlippedX() == true) {
			player->setFlippedX(false);
		}
	}
	else if (INPUTS->getKey(KeyCode::KEY_A)) {
		player->getPhysicsBody()->setVelocity(Vec2(-100, 0));
		if (player->isFlippedX() == false) {
			player->setFlippedX(true);
		}
	}
	else {
		player->getPhysicsBody()->setVelocity(Vec2(0, 0));
	}

	//positioning camera node to be below player (adding it as a child of player doesn't work)
	camPos->setPosition(player->getPosition() - Vec2(0, -400));

	//update the keyboard each frame
	INPUTS->clearForNextFrame();
}

//check if 2 objects collide and do something
bool Level::onContactBegin(cocos2d::PhysicsContact &contact)
{
	PhysicsBody *a = contact.getShapeA()->getBody();
	PhysicsBody *b = contact.getShapeB()->getBody();

	// check if player has collided with a vision cone
	if ((a->getTag() == 1 && b->getTag() == 2) || (b->getTag() == 1 && a->getTag() == 2))
	{
		CCLOG("COLLISION HAS OCCURED");
	}

	return true;
}