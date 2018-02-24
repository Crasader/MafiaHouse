#pragma once
#include "GameObject.h"
class Item : public GameObject
{
public:
	Item();
	~Item();

	static Item* create(const std::string& filename = "knife.png");
};
