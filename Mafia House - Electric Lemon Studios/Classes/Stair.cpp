#include "Stair.h"

//Stair Class
Stair::Stair() {
	name = "stair";
	tag = 40000;
	//sprite properties
	zOrder = 2;
	scale = 1.0f;
	//physics body properties
	dynamic = false;
	category = 4;
	collision = 0xFFFFFFFF;
}

Stair::~Stair() {

}

Stair* Stair::create(const std::string& filename)
{
	Stair *sprite = new (std::nothrow) Stair();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

void Stair::initObject() {
	this->setContentSize(stairSize);//won't be needed once we have a proper sprite
	
	GameObject::initObject();
	
	//use type to set tag of the stairway
	if (type == 2) {
		this->setTag(tag + pairNum + 1000);//will have tag of it's partner stairway plus 1000
	}
	else if (type == 1){
		this->setTag(tag + pairNum);
	}
}
