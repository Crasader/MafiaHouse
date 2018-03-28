#pragma once
#include "Item.h"
class DeadBody : public Item
{
public:
	DeadBody();
	~DeadBody() {}
	CREATE_WITH_FRAME(DeadBody);
	CREATE_WITH_FRAME_NAME(DeadBody, "enemy/thug/dead.png");

	void initObject(Vec2 startPos = Vec2(0, 0));
	void initRadius();

	void playerInRange(Node* player);

	void initPickedUpBody();
	void initHeldBody();
	void initCrouchPickedUpBody();
	void initCrouchHeldBody();
	void initDroppedBody(Vec2 pos, bool flip);
	void throwItem(float angle, Vec2 pos, bool flip);
	void initThrownItem();
	void initGroundItem();

	void checkSpeed();

	void prepareThrow();
	void prepareCrouchThrow();

	bool isHidden = false;
};