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
	Node::setPosition(pos - Vec2(FRAME_OFFSET, 0));
}
void Character::setPositionX(float posX) {
	Node::setPositionX(posX - FRAME_OFFSET);
}

void Character::setRoomPosition(Vec2 roomPos, Vec2 position) {
	setPosition(roomPos + position);
	initialPos = getPosition();
}

void Character::flipX() {
	GameObject::flipX();
	if (heldItem != NULL) {
		heldItem->knockback *= -1;
		heldItem->holderFlipped = flippedX;
	}
}

void Character::updateRoom(vector<RoomData*> rooms) {
	for (int i = 0; i < rooms.size(); i++) {
		if ((getPositionX() > rooms[i]->left - 10) && (getPositionX() + getSize().width < rooms[i]->right + 10)) {//player in on the floor, inbetween top and bottom
			currentRoom = i;
			break;
		}
	}
}

void Character::pickUpItem(GameLayer* mainLayer) {
	if (itemToPickUp != NULL) {
		removeChild(heldItem, true);

		heldItem = itemToPickUp;
		itemToPickUp->removeFromParent();
		
		addChild(heldItem);
		heldItem->initPickedUpItem();
		inventory.push_back(heldItem);

		itemToPickUp = NULL;

		if (flippedX == true) {
			heldItem->knockback *= -1;
		}
		heldItem->holderFlipped = flippedX;
	}
}

void Character::dropItem(GameLayer* mainLayer, float time) {
	if (heldItem != NULL) {
		heldItem->setVisible(true);
		//removing dropped item from inventory
		for (int i = 0; i < inventory.size(); i++) {
			if (inventory[i] == heldItem) {
				inventory.erase(inventory.begin() + i);
				break;
			}
		}
		if (thrownItem == NULL) {
			thrownItem = heldItem;
			thrownItem->thrownTime = time;
		}
		heldItem->initDroppedItem(convertToWorldSpace(heldItem->getPosition()), flippedX);

		removeChild(heldItem, true);
		mainLayer->addChild(heldItem);
		heldItem = NULL;
	}
}

void Character::throwItem(GameLayer* mainLayer, float time) {
	if (heldItem != NULL) {
		//removing thrown item from inventory
		for (int i = 0; i < inventory.size(); i++) {
			if (inventory[i] == heldItem) {
				inventory.erase(inventory.begin() + i);
				break;
			}
		}
		heldItem->thrownTime = time;
		heldItem->throwItem(aimAngle, convertToWorldSpace(heldItem->getPosition()), flippedX);
		thrownItem = heldItem;
		removeChild(heldItem, true);
		mainLayer->addChild(heldItem);
		heldItem = NULL;
	}
}

void Character::beginThrowItem() {
	if (heldItem != NULL) {
		heldItem->prepareThrow(aimAngle);

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

void Character::beginUseItem(float angle) {
	if (heldItem != NULL) {
		if (heldItem->getAttackType() == Item::STAB) {
			heldItem->prepareStab(angle);
			setSpriteFrame(stab.animation->getFrames().at(0)->getSpriteFrame());//setting player sprite to first frame of stab animation
		}
		else if (heldItem->getAttackType() == Item::SWING) {
			heldItem->prepareSwing(angle);
			setSpriteFrame(swing.animation->getFrames().at(0)->getSpriteFrame());//first frame of the swing animation
		}
	}
}

void Character::useItem(float angle) {
	if (heldItem != NULL) {
		if (heldItem->getAttackType() != Item::SHOOT) {
			heldItem->getPhysicsBody()->setEnabled(true);
		}
		if (heldItem->getAttackType() == Item::STAB) {
			heldItem->stabSequence(angle, flippedX);
			setSpriteFrame(stab.animation->getFrames().at(1)->getSpriteFrame());
		}
		else if (heldItem->getAttackType() == Item::SWING) {
			heldItem->swingSequence(angle, flippedX);
			//startAnimation(SWING, swing);
			setSpriteFrame(swing.animation->getFrames().at(1)->getSpriteFrame());//first frame of the swing animation
		}
	}
}

void Character::finishUseItem() {
	if (heldItem->getAttackType() == Item::STAB) {
		setSpriteFrame(stab.animation->getFrames().at(0)->getSpriteFrame());
	}
	else if (heldItem->getAttackType() == Item::SWING) {
		setSpriteFrame(swing.animation->getFrames().at(0)->getSpriteFrame());//first frame of the swing animation
	}
	else if (heldItem->getAttackType() == Item::SHOOT) {
		setSpriteFrame(stab.animation->getFrames().at(0)->getSpriteFrame());//first frame of the swing animation
	}
	heldItem->getPhysicsBody()->setEnabled(false);
}

void Character::useDoor() {
	if (doorToUse != NULL) {
		doorToUse->use();
	}
}

void Character::useStair(GameLayer* mainLayer) {
	if (stairToUse != NULL) {
		usedStair = stairToUse;
		stop();
		auto callback1 = CallFunc::create([this, mainLayer]() {
			if (usedStair != NULL) {
				usedStair->setSpriteFrame(frameCache->getSpriteFrameByName("objects/stairdoor_open.png"));
				usedStair->numLabel->setVisible(false);
			}
		});
		auto callback2 = CallFunc::create([this, mainLayer]() {
			if (usedStair != NULL) {
				usedStair->use(this, mainLayer);
				usedStair = NULL;
			}
		});
		auto wait = MoveBy::create(0.2f, Vec2(0, 0));
		auto sequence = Sequence::create(callback1, wait, callback2, nullptr);//runs the stair use animation and then has character take the stairs
		if (getActionByTag(99) == NULL) {
			runAction(sequence)->setTag(99);
		}
	}

	/*if (stairToUse != NULL) {
		stairToUse->use(this, mainLayer);
	}*/
}

bool Character::checkDead() {
	if (hp <= 0) {
		return true;
	}
	return false;
}