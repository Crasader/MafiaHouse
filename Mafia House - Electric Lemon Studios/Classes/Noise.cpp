#include "Noise.h"

Noise::Noise() {
}

void Noise::initNoise(float volume, float duration, float time, Vec2 position, string type, Vec2 roomCoords) {
	setName("noise");
	name = type;
	this->duration = duration;
	startTime = time;
	radius = volume;
	room = roomCoords;
	setPosition(position);

	auto body = PhysicsBody::createCircle(radius);
	body->setDynamic(false);
	body->setCategoryBitmask(4);
	body->setCollisionBitmask(3);//player and enemies
	body->setContactTestBitmask(0xFFFFFFFF);
	body->setName("noise");
	setPhysicsBody(body);
}