#include "EnvObject.h"

HideObject::HideObject()
{
	//sprite properties
	zOrder = 3;
	scale = 1.0f;
	name = "hide_object";//used for collision detection
	//physics body properties
	tag = 20000;//each object will have a unique tag number
	dynamic = false;
	category = 16;
	collision = 1;
}
HideObject::~HideObject(){
}

void HideObject::initHideObject(string name) {
	GameObject::initObject();
	createOutline("objects/" + name + "_outline.png");
	outline->setColor(ccc3(255, 235, 50));//yellow
	outline->setVisible(false);
}

void HideObject::playerInRange() {
	if (hiding == false) {
		if (playerRange == true) {
			outline->setVisible(true);
			//outline->setColor(ccc3(255, 235, 50));//yellow
		}
		else {
			outline->setVisible(false);
			//outline->setColor(ccc3(155, 155, 155));//grey
		}
	}
	playerRange = false;
}

void HideObject::hide() {
	hiding = true;
	setOpacity(175);
	outline->setVisible(false);
}
void HideObject::unhide() {
	hiding = false;
	setOpacity(255);
	outline->setVisible(true);
}

//Physical Objects:
PhysObject::PhysObject(){
	//sprite properties
	zOrder = 6;
	scale = 1.0f;
	name = "phys_object";//used for collision detection
	//physics body properties
	tag = 50000;//each object will have a unique tag number
	dynamic = false;
	category = 0xFFFFFFFF;
	collision = 41;
}

void PhysObject::initObject(){
	initObjectNoPhysics();
	initBoxBody(surfaceSize);
	getPhysicsBody()->setPositionOffset(Vec2(0, 24));
}

//Table:
Table::Table(){
	PhysObject::PhysObject();
	surfaceSize = Size(140, 10);
}

void Table::initObject() {
	PhysObject::initObject();
}