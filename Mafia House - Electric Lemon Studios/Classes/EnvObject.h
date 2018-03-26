#pragma once
#include "GameObject.h"
class HideObject : public GameObject{
public:
	HideObject();
	~HideObject();
	CREATE_WITH_FRAME(HideObject);
	CREATE_WITH_FRAME_NAME(HideObject, "objects/pot.png");

	void initHideObject(string name);

	void playerInRange();

	void hide();
	void unhide();

	bool hiding = false;
};

class PhysObject : public GameObject{
public:
	PhysObject();
	~PhysObject() {}

	virtual void initObject();

	void initHideBox();

protected:
	Size surfaceSize;
	Vec2 surfaceOffset;
	Size hideBoxSize;
	Vec2 hideBoxPosition;
	Node* hideBox;
};

class Table : public PhysObject{
public:
	Table();
	~Table() {}
	CREATE_WITH_FRAME(Table);
	CREATE_WITH_FRAME_NAME(Table, "objects/table.png");

	void initObject();
};

class VentCover : public PhysObject {
public:
	VentCover();
	~VentCover() {}
	CREATE_WITH_FRAME(VentCover);
	CREATE_WITH_FRAME_NAME(VentCover, "objects/vent_cover.png");

	void initObject();
};