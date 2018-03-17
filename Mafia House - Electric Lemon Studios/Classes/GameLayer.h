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
};