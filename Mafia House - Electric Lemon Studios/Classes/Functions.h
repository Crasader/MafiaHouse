//Additional Functions for the game
#pragma once
#include "cocos2d.h"
#include <vector>
using std::vector;
USING_NS_CC;

//convert time value into frames (for 60fps):
#define FRAMES * 1.0f/60.0f

//convert number of frames into seconds:
#define SECONDS * 60

//nodeA will 'chase' nodeB; you can also use this to 'leash' nodeA to nodeB instead
void followRadius(Node* nodeA, Node* nodeB, float radius, Vec2 offset);
 //like the followRadius function, but uses a rectangular bounding box instead of circular radius
void followBox(Node* nodeA, Node* nodeB, Vec2 range, Vec2 offset);

class Floor {//just for keeping track of what floor characters are on
public:
	Floor(int n, int h, int t, int b);
	~Floor();

	int num;
	int height;
	int top;
	int bot;
};
