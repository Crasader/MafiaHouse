#pragma once
#include "cocos2d.h"
#include <string>
#include <vector>
#include <algorithm>
USING_NS_CC;
using std::string;
using std::vector;

#define CREATE_SPRITE_FUNC(__TYPE__, _FILENAME_) \
static __TYPE__* create(const std::string& filename = _FILENAME_) \
{ \
	__TYPE__ *sprite = new(std::nothrow) __TYPE__(); \
	if (sprite && sprite->initWithFile(filename)) \
	{ \
		sprite->autorelease(); \
		return sprite; \
	} \
	CC_SAFE_DELETE(sprite); \
	return nullptr; \
}

class GameObject: public Sprite
{
public:
	GameObject();
	~GameObject();
	CREATE_SPRITE_FUNC(GameObject, "default.png");//must overload create function of Sprite to derive class properly

	virtual void initObject();
	virtual void initObject(Vec2 startPos);

	void initAnimations();
	Vector<cocos2d::SpriteFrame*> getAnimation(const char *format, int count);//gets animation from sprite sheet

	void setRoomPositionNormalized(Vec2 roomPos, Size roomSize, Vec2 position);//set the objects nomalized position relative to the room it is generated inside
	void setRoomPosition(Vec2 roomPos, Vec2 position);//set the objects position relative to the room it is generated inside

	//movement functions
	void stop();

	void move(Vec2 velocity);

	void flipX();//flips object on X-axis

	Vec2 roomStartPos = Vec2(0, 0);//position object starts in relative to room position
	Vec2 startRoom = Vec2(0,0);//coordinates of room the object starts in

protected:
	bool flippedX = false;//false = facing right
	bool flippedY = false;//false = facing upright

	float maxSpeed = 100.0f;

	std::string name = "name";//can set name to identify sprite type, used for collision detection
	float zOrder = 0;//determines what is drawn over top of what
	float scale = 1.0;//scale factor for sprite
	bool startFlippedX = false;
	bool startFlippedY = false;

	int tag = 0;//tag for the physics body, used for collision detection
	bool dynamic = false;//set whether bosy will be dynamic or not
	int category = 1;//category group bitmask for collisions
	int collision = 1;//collision group bitmask for collisions

	Director* director = Director::getInstance();
};
