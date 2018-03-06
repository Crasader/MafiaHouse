#include "EnvObject.h"

EnvObject::EnvObject()
{
	//sprite properties
	zOrder = 3;
	scale = 1.0f;
	name = "env_object";//used for collision detection
	//physics body properties
	tag = 20000;//each object will have a unique tag number
	dynamic = true;
	category = 16;
	collision = 3;
}
EnvObject::~EnvObject(){
}
