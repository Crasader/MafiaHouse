//Additional Functions for the game
#pragma once
#include "cocos2d.h"
#include <vector>
#include <string>
using std::vector;
using std::string;
USING_NS_CC;

//convert time value into frames (for 60fps):
#define FRAMES * 1.0f/60.0f

//convert number of frames into seconds:
#define SECONDS * 60

//nodeA will 'chase' nodeB; you can also use this to 'leash' nodeA to nodeB instead
void followRadius(Node* nodeA, Node* nodeB, float radius, Vec2 offset);
 //like the followRadius function, but uses a rectangular bounding box instead of circular radius
void followBox(Node* nodeA, Node* nodeB, Vec2 range, Vec2 offset);
//for genrating random numbers
int randNum(int min, int max);

class DoorData {
public:
	DoorData() {}
	~DoorData() {}

	float pos = 0.0f;

	int type = 1;//1 = door, 2 = vent

	bool locked = false;//true, starts locked, enemies will lock it automatically
};

class RoomData {
public:
	RoomData() {}
	~RoomData() {}

	int num;//room number
	int width;//length of the room
	int left;//left of the room
	int right;//right of the room

	string bgName = "guestRoom.png";

	bool hasLeftDoor = false;
	bool hasRightDoor = false;

	vector<DoorData> rightDoors;//the locations of the vents and doors on the right wall
	vector<DoorData> leftDoors;//the locations of the vents and doors on the left wall
	vector<DoorData> ceilingDoors;//the locations of the vents and doors on the ceiling
	vector<DoorData> bottomDoors;//the locations of the vents and doors on the ground

	Vec2 room = Vec2(0, 0);//the room's coordinates
};

class FloorData {
public:
	FloorData() {}
	~FloorData() {}

	int num;//floor number
	int height;//height of the floor
	int top;//top of the floor
	int bot;//bottomg of the floor
	vector<RoomData> rooms;//rooms the floor contains
};