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

void Stair::initObject() {
	setContentSize(stairSize);//won't be needed once we have a proper sprite
	
	GameObject::initObject();
	
	//use type to set tag of the stairway
	if (type == 2) {
		setTag(tag + pairNum + 1000);//will have tag of it's partner stairway plus 1000
	}
	else if (type == 1){
		setTag(tag + pairNum);
	}
}
