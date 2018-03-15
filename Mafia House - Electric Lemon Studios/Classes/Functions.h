//Additional Functions for the game
#pragma once
#include "cocos2d.h"
USING_NS_CC;

#define FRAMES * 1.0f/60.0f

//nodeA will 'chase' nodeB; you can also use this to 'leash' nodeA to nodeB instead
void followRadius(Node* nodeA, Node* nodeB, float radius, Vec2 offset);
 //like the followRadius function, but uses a rectangular bounding box instead of circular radius
void followBox(Node* nodeA, Node* nodeB, Vec2 range, Vec2 offset);

class GameLayer : public Node {
public:
	CREATE_FUNC(GameLayer);
	int itemUsed = -1;
	int enemyHit = -1;
	int enemyTouched = -1;
};
