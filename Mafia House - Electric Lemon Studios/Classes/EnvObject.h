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

	float surfaceHeight;

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
	CREATE_WITH_FRAME_NAME(Table, "objects/fancy_table.png");
};

class WallShelf : public PhysObject {
public:
	WallShelf();
	~WallShelf() {}
	CREATE_WITH_FRAME(WallShelf);
	CREATE_WITH_FRAME_NAME(WallShelf, "objects/wall_shelf.png");
};


class VentCover : public PhysObject {
public:
	VentCover();
	~VentCover() {}
	CREATE_WITH_FRAME(VentCover);
	CREATE_WITH_FRAME_NAME(VentCover, "objects/vent_cover.png");
};