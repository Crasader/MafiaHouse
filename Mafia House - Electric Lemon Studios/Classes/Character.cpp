#include "Character.h"

void Character::initObject(Vec2 startPos) {
	GameObject::initObjectNoPhysics(startPos);
	initBoxBody(bodySize);
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
			setSpriteFrame(stabbing.animation->getFrames().at(0)->getSpriteFrame());//setting player sprite to first frame of stab animation
		}
		else if (heldItem->getAttackType() == Item::SWING) {
			heldItem->beginSwing();
			//setSpriteFrame(swingAnimation->getFrames[0]);//setting player sprite to first frame of stab animation
		}
	}
}

void Character::useItem() {
	if (heldItem != NULL) {
		heldItem->getPhysicsBody()->setEnabled(true);
		if (heldItem->getAttackType() == Item::STAB) {
			heldItem->stabSequence();
			setSpriteFrame(stabbing.animation->getFrames().at(1)->getSpriteFrame());
		}
		else if (heldItem->getAttackType() == Item::SWING) {
			heldItem->swingSequence();
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

void Character::hit(Item* item) {
	if (item->didHitWall == false) {
		item->used();
		isHit = true;
	}
}
