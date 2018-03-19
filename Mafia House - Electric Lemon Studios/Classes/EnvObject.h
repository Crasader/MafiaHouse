#pragma once
#include "GameObject.h"
class EnvObject : public GameObject
{
public:
	EnvObject();
	~EnvObject();
	CREATE_SPRITE_FUNC(EnvObject, "pot.png");
	CREATE_WITH_FRAME(EnvObject);
	CREATE_WITH_FRAME_NAME(EnvObject, "objects/pot.png");
};
