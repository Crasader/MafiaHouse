#include "Character.h"

void Character::initObject(Vec2 startPos) {
	GameObject::initObjectNoPhysics(startPos);
	initBoxBody(bodySize);
}

/*AnimationTag Character::stringToTag(string name) {
	AnimationTag animTag;
	if (name == "stand") {
		animTag = STAND;
	}
	else if (name == "walking") {
		animTag = WALK;
	}
	else if (name == "moonwalk") {
		animTag = MOONWALK;
	}
	else if (name == "stab") {
		animTag = STAB;
	}
	else if (name == "swing") {
		animTag = SWING;
	}
	else if (name == "throw") {
		animTag = THROW;
	}
	else if (name == "climb") {
		animTag = CLIMB;
	}
	else if (name == "hide") {
		animTag = HIDDEN;
	}
	else if (name == "fall") {
		animTag = FALL;
	}
	else if (name == "death") {
		animTag = DEATH;
	}
	else if (name == "knockout") {
		animTag = KNOCKOUT;
	}
	else if (name == "sleep") {
		animTag = SLEEP;
	}
	return animTag;
}*/

void Character::startAnimation(AnimationTag tag, GameAnimation animation) {
	if (getActionByTag(tag) == NULL) {
		runAction(animation.action);
	}
}

void Character::stopAnimation(AnimationTag tag) {
	stopActionByTag(tag);
}

void Character::pickUpItem(GameLayer* mainLayer) {
	if (itemToPickUp != NULL) {
		heldItem = itemToPickUp;
		mainLayer->removeChild(itemToPickUp, true);

		removeChild(heldItem, true);
		addChild(heldItem);
		heldItem->initHeldItem();
		inventory.push_back(heldItem);

		itemToPickUp = NULL;
	}
}

void Character::dropItem(GameLayer* mainLayer) {
	if (heldItem != NULL) {
		//removing dropped item from inventory
		for (int i = 0; i < inventory.size(); i++) {
			if (inventory[i] == heldItem) {
				inventory.erase(inventory.begin() + i);
				break;
			}
		}
		heldItem->initDroppedItem(convertToWorldSpace(heldItem->getPosition()), flippedX);

		removeChild(heldItem, true);
		mainLayer->addChild(heldItem);
		heldItem = NULL;
	}
}

void Character::breakItem(GameLayer* mainLayer) {
	if (heldItem != NULL) {
		//removing item from inventory
		for (int i = 0; i < inventory.size(); i++) {
			if (inventory[i] == heldItem) {
				inventory.erase(inventory.begin() + i);
				break;
			}
		}
		//removing item from the main Layer's list of items
		for (int i = 0; i < mainLayer->items.size(); i++) {
			if (mainLayer->items[i] == heldItem) {
				mainLayer->items.erase(mainLayer->items.begin() + i);
				break;
			}
		}
		heldItem->breakItem();
		heldItem = NULL;
	}
}

void Character::beginUseItem() {
	if (heldItem != NULL) {
		if (heldItem->getAttackType() == Item::STAB) {
			heldItem->beginStab();
			setSpriteFrame(stab.animation->getFrames().at(0)->getSpriteFrame());//setting player sprite to first frame of stab animation
		}
		else if (heldItem->getAttackType() == Item::SWING) {
			heldItem->beginSwing();
			setSpriteFrame(swing.animation->getFrames().at(0)->getSpriteFrame());//first frame of the swing animation
		}
	}
}

void Character::useItem() {
	if (heldItem != NULL) {
		heldItem->getPhysicsBody()->setEnabled(true);
		if (heldItem->getAttackType() == Item::STAB) {
			heldItem->stabSequence();
			setSpriteFrame(stab.animation->getFrames().at(1)->getSpriteFrame());
		}
		else if (heldItem->getAttackType() == Item::SWING) {
			heldItem->swingSequence();
			setSpriteFrame(swing.animation->getFrames().at(1)->getSpriteFrame());//run animation here rather than setting frame if there's more than 2 frames for swinging
		}
	}
}

void Character::useDoor() {
	if (doorToUse != NULL) {
		doorToUse->use();
	}
}

void Character::useStair(GameLayer* mainLayer) {
	if (stairToUse != NULL) {
		stairToUse->use(this, mainLayer);
	}
}
