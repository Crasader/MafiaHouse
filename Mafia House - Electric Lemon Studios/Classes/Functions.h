//Additional Functions for the game
#pragma once
#include "cocos2d.h"
USING_NS_CC;

//nodeA will 'chase' nodeB; you can also use this to 'leash' nodeA to nodeB instead
void followRadius(Node* nodeA, Node* nodeB, float radius, Vec2 offset);
 //like the followRadius function, but uses a rectangular bounding box instead of circular radius
void followBox(Node* nodeA, Node* nodeB, Vec2 range, Vec2 offset);