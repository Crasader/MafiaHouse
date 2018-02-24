#include "Wall.h"



Wall::Wall()
{
	//sprite properties
	//spriteName = "default.png";
	scale = 1.0f;
	//physics body properties
	tag = 0;
	dynamic = false;
}


Wall::~Wall()
{
}

Wall* Wall::create(const std::string& filename)
{
	Wall *sprite = new (std::nothrow) Wall();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

void Wall::initObject(Vec2 startPos, Size size) {
	this->setContentSize(size);//set the size of the wall
	GameObject::initObject(startPos);
}
