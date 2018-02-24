#pragma once
#include "cocos2d.h"
#include <string>
USING_NS_CC;

class GameObject: public Sprite
{
public:
	GameObject();
	~GameObject();
	//must overload create function of Sprite to derive class properly
	static GameObject* create(const std::string& filename);

	virtual void initObject(Vec2 startPos = Vec2(0,0));

	void initAnimations();
	Vector<cocos2d::SpriteFrame*> getAnimation(const char *format, int count);//gets animation from sprite sheet

	GameObject* object;
	//static std::string spriteName;//file name of the sprite
	float scale;//scale factor for sprite

	int tag;//tag for the physics body, used for collision detection
	bool dynamic;//set whether bosy will be dynamic or not
	int category = 1;//category group bitmask for collisions
	int collision = 1;//collision group bitmask for collisions
};

