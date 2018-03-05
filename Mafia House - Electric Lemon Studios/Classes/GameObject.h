#pragma once
#include "cocos2d.h"
#include <string>
#include <vector>
#include <algorithm>
USING_NS_CC;

class GameObject: public Sprite
{
public:
	GameObject();
	~GameObject();
	//must overload create function of Sprite to derive class properly
	static GameObject* create(const std::string& filename);

	virtual void initObject();
	virtual void initObject(Vec2 startPos);

	void initAnimations();
	Vector<cocos2d::SpriteFrame*> getAnimation(const char *format, int count);//gets animation from sprite sheet

	void setRoomPositionNormalized(Vec2 roomPos, Size roomSize, Vec2 position);//set the objects nomalized position relative to the room it is generated inside
	void setRoomPosition(Vec2 roomPos, Vec2 position);//set the objects position relative to the room it is generated inside

	void move(Vec2 velocity);

	void flip();//flips game object on X-axis

	bool flipped = false;//false = facing right

	Vec2 roomStartPos = Vec2(0, 0);//position object starts in relative to room position
	Vec2 startRoom = Vec2(0,0);//coordinates of room the object starts in

protected:
	std::string name = "name";//can set name to identify sprite type, used for collision detection
	float zOrder = 0;//determines what is drawn over top of what
	float scale = 1.0;//scale factor for sprite
	bool flippedX = false;

	int tag = 0;//tag for the physics body, used for collision detection
	bool dynamic = false;//set whether bosy will be dynamic or not
	int category = 1;//category group bitmask for collisions
	int collision = 1;//collision group bitmask for collisions
};

