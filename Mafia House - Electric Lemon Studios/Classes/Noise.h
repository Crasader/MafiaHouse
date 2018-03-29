#pragma once
#include "cocos2d.h"
#include <string>
USING_NS_CC;
using std::string;

class Noise :public Node {
public:
	CREATE_FUNC(Noise);
	Noise();
	~Noise() {}
	void initNoise(float volume, float duration, float time, Vec2 position, string type, Vec2 roomCoords);

	string name;
	float radius;
	float duration;
	float startTime;
	Vec2 room;
};