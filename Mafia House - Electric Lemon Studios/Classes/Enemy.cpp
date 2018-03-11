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
	visionSprite = GameObject::create("visionCone.png");
	visionSprite->initObject();
	visionSprite->setName("vision_cone");
	visionSprite->setScale(0.75);
	visionSprite->setPositionNormalized(Vec2(1, 0.4));
	visionSprite->setGlobalZOrder(6);

	visionSprite->getPhysicsBody()->setCategoryBitmask(4);
	visionSprite->getPhysicsBody()->setCollisionBitmask(1);
	visionSprite->getPhysicsBody()->setEnabled(false);

	//this->addChild(visionSprite);
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


void Enemy::visionRays(vector<Vec2> *points, Vec2* start)
{
	bool* didRun = new bool;
	*didRun = false;
	PhysicsRayCastCallbackFunc func = [this, points, didRun](PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data)->bool
	{
		visionContactTag = info.shape->getBody()->getTag();
		visionContactName = info.shape->getBody()->getName();

		if (visionContactName == "enemy" || visionContactName == "item" || visionContactName == "stair" || visionContactName == "env_object" || visionContactName == "item_radius" || visionContactName == "door_radius") {//things to ingore collisions with
			return true;
		}
		else {
			points->push_back(info.contact);
			*didRun = true;
			return false;
		}
	};

	Vec2 startPoint;
	Vec2 endPoint;
	int direction;

	if (flippedX == false) {
		startPoint = this->getPosition() + Vec2(this->getContentSize().width - 5, 87);
		direction = 1;
	}
	else {
		startPoint = this->getPosition() + Vec2(5, 87);
		direction =  -1;
	}
	*start = startPoint;

	float angle = 0 - (visionDegrees / 2);
	for (int i = 0; i < visionDegrees; i++) {
		endPoint.x = startPoint.x + cosf((angle + i) * M_PI / 180) * visionRadius * direction;
		endPoint.y = startPoint.y + sinf((angle + i) * M_PI / 180) * visionRadius;
		director->getRunningScene()->getPhysicsWorld()->rayCast(func, startPoint, endPoint, nullptr);
		if (*didRun == false) {
			points->push_back(endPoint);
		}
		*didRun = false;
	}
}
