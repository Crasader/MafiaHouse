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
	void initCrouchPickedUpBody();
	void initDroppedBody(Vec2 pos, bool flip);

	bool isHidden = false;
};