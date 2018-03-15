#include "Functions.h"

void followRadius(Node* nodeA, Node* nodeB, float radius, Vec2 offset) {
	Vec2 displacement = nodeA->getPosition() - (nodeB->getPosition() + offset);
	float distance = displacement.getLength();
	if (distance > radius) {
		nodeA->setPosition((nodeB->getPosition() + offset) + (displacement.getNormalized() * radius));
	}
}

void followBox(Node* nodeA, Node* nodeB, Vec2 range, Vec2 offset) {
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
