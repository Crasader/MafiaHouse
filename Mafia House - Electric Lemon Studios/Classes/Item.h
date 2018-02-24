#pragma once
#include "GameObject.h"
class Item : public GameObject
{
public:
	Item();
	~Item();

	static Item* create(const std::string& filename = "knife.png");

	void initObject(Vec2 startPos);

	void initObject();

protected:
	//Node* pickUpRadius;//used to create invisible physics body to detect when player can pick up item
};
