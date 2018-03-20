#pragma once
#include "GameObject.h"
class HideObject : public GameObject
{
public:
	HideObject();
	~HideObject();
	CREATE_SPRITE_FUNC(HideObject, "pot.png");
	CREATE_WITH_FRAME(HideObject);
	CREATE_WITH_FRAME_NAME(HideObject, "objects/pot.png");

	void initHideObject(string name);

	void playerInRange();

	void hide();
	void unhide();

	bool hiding = false;
};
