#include "Character.h"

void Character::initObject(Vec2 startPos) {
	GameObject::initObjectNoPhysics(startPos);
	initBoxBody(bodySize);
}

float Character::getPositionX() {
	return Node::getPositionX() + FRAME_OFFSET;
}
Vec2 Character::getPosition() {
	return Node::getPosition() + Vec2(FRAME_OFFSET, 0);
}

void Character::setPosition(Vec2 pos) {
	Node::setPosition(pos + Vec2(FRAME_OFFSET, 0));
}
void Character::setPositionX(float posX) {
	Node::setPositionX(posX + FRAME_OFFSET);
}

void Character::flipX() {
	GameObject::flipX();
	if (heldItem != NULL) {
		heldItem->knockback *= -1;
	}
}

void Character::updateRoom(vector<RoomData*> rooms) {
	for (int i = 0; i < rooms.size(); i++) {
		if ((getPositionX() > rooms[i]->left) && (getPositionX() + getSize().width < rooms[i]->right)) {//player in on the floor, inbetween top and bottom
			currentRoom = i;
			break;
		}
	}
}

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
		removeChild(heldItem, true);

		heldItem = itemToPickUp;
		mainLayer->removeChild(itemToPickUp, true);
		
		addChild(heldItem);
		heldItem->initHeldItem();
		inventory.push_back(heldItem);

		itemToPickUp = NULL;

		if (flippedX == true) {
			heldItem->knockback *= -1;
		}
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

bool Character::checkDead() {
	if (hp <= 0) {
		return true;
	}
	return false;
}