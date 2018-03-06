#pragma once
#include "GameObject.h"
class EnvObject : public GameObject
{
public:
	EnvObject();
	~EnvObject();
	CREATE_SPRITE_FUNC(EnvObject, "pot.png");
};
