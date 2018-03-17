#pragma once
#include "cocos2d.h"
#include <vector>
#include "Stair.h"
USING_NS_CC;
using std::vector;

class GameLayer : public Node {
public:
	CREATE_FUNC(GameLayer);

	vector<Stair*> stairs;
	vector<Floor> floors;

	Stair* getPartnerStair(Stair* stair);//returns a stair's partner, the one it leads to

	int itemUsed = -1;
	int enemyHit = -1;
	int enemyTouched = -1;
	int playerStairUsed = -1;
};