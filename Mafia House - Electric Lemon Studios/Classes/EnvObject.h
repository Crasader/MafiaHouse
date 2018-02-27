#pragma once
#include "GameObject.h"
class EnvObject : public GameObject
{
public:
	EnvObject();
	~EnvObject();

	static EnvObject* create(const std::string& filename = "pot.png");

	//void initObject(Vec2 startPos);

	//void initObject();

protected:
	//Node* pickUpRadius;//used to create invisible physics body to detect when player can pick up item
};
#pragma once
