#pragma once
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include <string>
#include <vector>
#include <algorithm>
#include "Functions.h"
#include "Noise.h"
USING_NS_CC;
using std::string;
using std::vector;

#define CREATE_EMPTY_SPRITE_FUNC(__TYPE__) \
static __TYPE__* create() \
{ \
	 __TYPE__ *sprite = new (std::nothrow) __TYPE__(); \
	if (sprite && sprite->init()) \
	{ \
	sprite->autorelease(); \
	return sprite; \
	} \
	CC_SAFE_DELETE(sprite); \
	return nullptr; \
}
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
//for making a sprite from a sprite frame is the sprite frame cache:
#define CREATE_WITH_FRAME(__TYPE__) \
static __TYPE__* createWithSpriteFrame(SpriteFrame *spriteFrame) \
{ \
    __TYPE__ *sprite = new (std::nothrow) __TYPE__(); \
    if (sprite && spriteFrame && sprite->initWithSpriteFrame(spriteFrame)) \
    { \
        sprite->autorelease(); \
        return sprite; \
    } \
    CC_SAFE_DELETE(sprite); \
    return nullptr; \
}
#define CREATE_WITH_FRAME_NAME(__TYPE__, _FRAMENAME_) \
static __TYPE__* createWithSpriteFrameName(const std::string& spriteFrameName = _FRAMENAME_) \
{ \
    SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName); \
    return createWithSpriteFrame(frame); \
}
//#if COCOS2D_DEBUG > 0 \
//    char msg[256] = {0}; \
//    sprintf(msg, "Invalid spriteFrameName: %s", spriteFrameName.c_str()); \
//    CCASSERT(frame != nullptr, msg); \
//#endif

enum AnimationTag {
	STAND,
	WALK,
	MOONWALK,
	CROUCH,
	STANDUP,
	CROUCHWALK,
	STAB,
	FALLATK,
	SWING,
	THROW,
	CLIMB,
	JUMP,
	ROLLING,
	HIDDEN,
	FALL,
	ROLL,
	KNOCKOUT,
	DEATH,
	SLEEP,
	ZZZs,
	OBJECT
};

Vector<cocos2d::SpriteFrame*> getAnimation(const char *format, int count);//gets animation from sprite sheet

class GameAnimation {
public:
	GameAnimation() {}
	GameAnimation(int tag, char* path, int numFrames, float frameTime, bool loop) {
		auto frames = getAnimation(path, numFrames);//change number of frames to correct number
		animation = Animation::createWithSpriteFrames(frames, frameTime);//change number to correct speed for animation
		animation->retain();
		if (loop == true) {
			action = Speed::create(RepeatForever::create(Animate::create(animation)), 1.0f);
		}
		else {
			action = Speed::create(Animate::create(animation), 1.0f);
		}
		action->retain();
		action->setTag(tag);
	}
	Speed* action;
	Animation* animation;
};

class GameObject: public Sprite
{
public:
	GameObject();
	~GameObject();
	CREATE_EMPTY_SPRITE_FUNC(GameObject);//must overload create function of Sprite to derive class properly
	CREATE_WITH_FRAME(GameObject);//this is just used by the below function
	CREATE_WITH_FRAME_NAME(GameObject, "default.png");//use this function to create a sprite when using sprite sheet

	virtual void initObject();
	virtual void initObjectNoPhysics();
	virtual void initAutoBody();
	virtual void initBoxBody(Size size);
	virtual void initObjectNoPhysics(Vec2 startPos);
	virtual void initObject(Vec2 startPos);

	void startAnimation(AnimationTag tag, GameAnimation animation);
	void stopAnimation(AnimationTag tag);

	virtual void createOutline(string name);
	virtual void createOutline2(string name);

	void setRoomPositionNormalized(Vec2 roomPos, Size roomSize, Vec2 position);//set the objects nomalized position relative to the room it is generated inside
	void setRoomPosition(Vec2 roomPos, Vec2 position);//set the objects position relative to the room it is generated inside

	virtual void updateFloor(vector<FloorData> floors);
	virtual void updateRoom(vector<RoomData*> rooms);
	void updateHeldItemFloor(vector<FloorData> floors);
	void updateHeldItemRoom(vector<RoomData*> rooms);

	//movement functions
	void stopY();
	void stopX();
	void stop();
	void slowStop();
	void slowStopX();

	void moveNoLimit(Vec2 velocity);//doesn't check for object's speed limit
	void moveAbsoluteNoLimit(Vec2 velocity);//doesn't check for object's speed limit
	void move(Vec2 velocity);//moves relative to direction object is facing
	void moveAbsolute(Vec2 velocity);//moves in absolute direction, positive is right, up

	void setSpeed(float speed);

	virtual void flipX();//flips object on X-axis

	bool getFlippedX() { return flippedX; }

	void createNoise(float radius, float duration, float time, Vec2 position, Vec2 roomCoords, string type, vector<Noise*> *noises);

	Vec2 roomStartPos = Vec2(0, 0);//position object starts in relative to room position
	//startRoom.y == floor number
	//startRoom.x == room nuumber
	Vec2 startRoom = Vec2(0,0);//NOT POSITION COORDINATES, the room the object starts in, see above comments
	
	//for pathfinding:
	int currentFloor;
	int currentRoom;

	bool playerRange = false;//for interactable items, check if player is in range to use them

	Sprite* outline;
	Sprite* outline2;

protected:
	PhysicsBody * mainBody;

	string outlineName;
	string outline2Name;

	Vec2 initialPos;

	bool flippedX = false;//false = facing right
	bool flippedY = false;//false = facing upright

	float baseSpeed = 100.0f;
	float maxSpeed = 100.0f;
	float maxSpeedY = 100.0f;

	std::string name = "name";//can set name to identify sprite type, used for collision detection
	float zOrder = 0;//determines what is drawn over top of what
	float scale = 1.0;//scale factor for sprite
	bool reversedX = false;//is the object's sprite 'flipped' from the start or not
	bool reversedY = false;

	int tag = 0;//tag for the physics body, used for collision detection
	bool dynamic = false;//set whether bosy will be dynamic or not
	bool rotate = false;
	int category = 0;//category group bitmask for collisions
	int contactTest = 0;//category group bitmask for collision testing
	int collision = 0;//collision group bitmask for collision physics

	Director* director = Director::getInstance();
	SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();
};
