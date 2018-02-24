#include "Item.h"



Item::Item()
{
	//sprite properties
	//spriteName = "knife.png";
	scale = 0.75f;
	//physics body properties
	tag = 3;
	dynamic = false;
	category = 2;
	collision = 2;
}


Item::~Item()
{
}

Item* Item::create(const std::string& filename)
{
	Item *sprite = new (std::nothrow) Item();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}
