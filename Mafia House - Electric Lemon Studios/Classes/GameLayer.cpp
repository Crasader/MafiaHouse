#include "GameLayer.h"
Stair* GameLayer::getPartnerStair(Stair* stair) {
	if (stair->type == 1) {
		return static_cast<Stair*>(getChildByTag(stair->getTag() + 1000));
	}
	else if (stair->type == 2) {
		return static_cast<Stair*>(getChildByTag(stair->getTag() - 1000));
	}
}