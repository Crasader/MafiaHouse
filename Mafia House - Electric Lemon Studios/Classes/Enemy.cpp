#include "Enemy.h"

Enemy::Enemy()
{
	name = "enemy";
	tag = 30000;//enemies will be 30000 - 39999
	//sprite properties
	zOrder = 4;
	scale = 1.0f;
	//physics body properties
	dynamic = true;
	category = 4;
	collision = 3;

	maxSpeed = 50;
}
Enemy::~Enemy(){
}

void Enemy::initObject(Vec2 startPos)
{
	GameObject::initObject(startPos);
	//initializing vision cone
	vision = GameObject::create("visionCone.png");
	vision->initObject();
	vision->setName("vision_cone");
	vision->setScale(0.75);
	vision->setPositionNormalized(Vec2(1, 0.4));
	vision->setGlobalZOrder(6);

	vision->getPhysicsBody()->setCategoryBitmask(4);
	vision->getPhysicsBody()->setCollisionBitmask(1);
	vision->getPhysicsBody()->setEnabled(false);

	this->addChild(vision);
}

void Enemy::walk(float time) {
	if (previousTurnTime == -1) {
		previousTurnTime = time;
	}

	if (time - previousTurnTime >= walkTime) {
		previousTurnTime = time;
		stop();
		flipX();
	}
	else {
		move(Vec2(4, 0));
	}
}


void Enemy::visionRays(Vec2* point, Vec2* start, Vec2* end)
{
	PhysicsRayCastCallbackFunc func = [this, point](PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data)->bool
	{
		visionContactTag = info.shape->getBody()->getTag();
		visionContactName = info.shape->getBody()->getName();

		*point = info.contact;

		if (visionContactName == "item" || visionContactName == "stair" || visionContactName == "env_object") {
			return true;
		}
		return false;
	};

	Vec2 startPoint;
	Vec2 endPoint;

	if (flippedX == false) {
		startPoint = this->getPosition() + Vec2(this->getContentSize().width + 1, 80);
		endPoint = startPoint + (Vec2(1, 0) * 200);
	}
	else {
		startPoint = this->getPosition() + Vec2(-1, 80);
		endPoint = startPoint + (Vec2(-1, 0) * 200);
	}

	director->getRunningScene()->getPhysicsWorld()->rayCast(func, startPoint, endPoint, nullptr);

	*start = startPoint;
	*end = endPoint;
}
