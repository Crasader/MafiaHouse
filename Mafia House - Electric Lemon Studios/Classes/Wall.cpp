#include "Wall.h"

Wall::Wall()
{
	name = "floor";
	tag = 0;
	//sprite properties
	zOrder = 6;
	scale = 1.0f;
	//physics body properties
	dynamic = false;
	category = 1;
	collision = 106;
	contactTest = 74;
}

Wall::~Wall(){
}

void Wall::initObject(Vec2 startPos, Size size) {
	setContentSize(size);//set the size of the wall
	GameObject::initObject(startPos);
}


//Room Class
Room::Room() {
}
Room::~Room() {
}

bool sortByPosition(DoorData a, DoorData b) {return a.pos < b.pos;}//function for sorting vector of DoorData

void Room::createWall(vector<Door*> *doors, int orientation, int type, Vec2 position, Size size, vector<DoorData> doorData, string name)
{
	Wall* w;
	if (doorData.size() > 0) {//has doors or vents

		int loops = (doorData.size() * 2);
		std::sort(doorData.begin(), doorData.end(), sortByPosition);//sorting doors in order from least to greatest

		//used purely for getting door and vent's proper size based on their orientation
		Door* door = Door::createWithSpriteFrameName();
		door->initObject(orientation);
		Vent* vent = Vent::createWithSpriteFrameName();
		vent->initObject(orientation);
		
		Door* d;
		float length;
		Vec2 newPos = position;
		int j = 0;
		if (orientation == 1) {//vertical
			for (int i = 0; i < loops; i++) {
				if (i % 2 == 0) {//on even number iterations, make a wall segment
					//length = (size.height * doorData[j].pos) - newPos.y + position.y;
					length =  doorData[j].pos - newPos.y + position.y;
					w = Wall::createWithSpriteFrameName();
					w->initObject(newPos, Size(size.width, length));
					w->setName(name);
					if (length > 0) {
						addChild(w);
					}
				}
				else {//on odd number iterations, make a door or vent
					if (doorData[j].type == 1) {
						d = Door::createWithSpriteFrameName();
						d->startRoom.y = doorData[j].floorNum;
						d->initObject(orientation, newPos);
						d->leftRoomCoords = doorData[j].leftRoom;
						d->rightRoomCoords = doorData[j].rightRoom;
					}
					else if (doorData[j].type == 2) {
						d = Vent::createWithSpriteFrameName();
						d->initObject(orientation, newPos);
					}
					else if (doorData[j].type == 3) {
						d = Exit::createWithSpriteFrameName();
						d->initObject(orientation, newPos);
					}
					length = d->size.height;
					if (d->checkExit() == false) {
						if (type != 2) {//only add doors or vents if right wall or ceiling
							if (doorData[j].locked == true) {
								d->defaultLocked = true;
							}
							doors->push_back(d);
						}
					}
					else {//door is an exit, add no matter what side
						if (type == 1) {//right exit
							static_cast<Exit*>(d)->side = 1;
						}
						else {
							static_cast<Exit*>(d)->side = 2;
						}
						doors->push_back(d);
					}
					j++;
				}
				newPos = newPos + Vec2(0, length);//setting position for next chunk of the wall
			}
			length = size.height - newPos.y + position.y;
			w = Wall::createWithSpriteFrameName();
			w->initObject(newPos, Size(size.width, length));
			w->setName(name);
			addChild(w);
		}
		else if (orientation == 2) {//horizontal
			for (int i = 0; i < loops; i++) {
				if (i % 2 == 0) {//on even number iterations, make a wall segment
					//length = (size.width * doorData[j].pos) - newPos.x + position.x;
					length = doorData[j].pos - newPos.x + position.x;
					w = Wall::createWithSpriteFrameName();
					w->initObject(newPos, Size(length, size.height));
					if (type == 2) {
						w->getPhysicsBody()->setCollisionBitmask(104);//floors don't actually collide with character because they get stuck on seems
						w->getPhysicsBody()->setContactTestBitmask(72);//floors check for contact with player and items
					}
					else {
						w->getPhysicsBody()->setContactTestBitmask(8);//ceilings only check with items
					}
					w->setName(name);
					if (length > 0) {
						addChild(w);
					}
				}
				else {//on odd number iterations, make a door or vent
					if (doorData[j].type == 1) {
						d = Door::createWithSpriteFrameName();
						d->startRoom.y = doorData[j].floorNum;
						d->initObject(orientation, newPos);
						d->leftRoomCoords = doorData[j].leftRoom;
						d->rightRoomCoords = doorData[j].rightRoom;
					}
					else if (doorData[j].type == 2) {
						d = Vent::createWithSpriteFrameName();
						d->initObject(orientation, newPos);
					}
					else if (doorData[j].type == 3) {
						d = Exit::createWithSpriteFrameName();
						d->initObject(orientation, newPos);
					}
					length = d->size.width;
					if (d->checkExit() == false) {
						if (type != 2) {//only add doors or vents if right wall or ceiling
							if (doorData[j].locked == true) {
								d->defaultLocked = true;
							}
							doors->push_back(d);
						}
					}
					else {//door is an exit, add no matter what side
						if (type == 1) {//right exit
							static_cast<Exit*>(d)->side = 1;
						}
						else {
							static_cast<Exit*>(d)->side = 2;
						}
						doors->push_back(d);
					}
					j++;
				}
				newPos = newPos + Vec2(length, 0);//setting position for next chunk of the wall
			}
			length = size.width - newPos.x + position.x;
			w = Wall::createWithSpriteFrameName();
			w->initObject(newPos, Size(length, size.height));
			if (type == 2) {
				w->getPhysicsBody()->setCollisionBitmask(104);//floors don't actually collide with character because they get stuck on seems
				w->getPhysicsBody()->setContactTestBitmask(72);//floors check for contact with player and items
			}
			else {
				w->getPhysicsBody()->setContactTestBitmask(8);//ceilings only check with items
			}
			w->setName(name);
			addChild(w);
		}
	}
	else {//no doors or vents
		w = Wall::createWithSpriteFrameName();
		if (orientation == 1) {
			w->initObject(position, Size(size.width, size.height));
		}
		else{
			w->initObject(position, Size(size.width, size.height));
			if (type == 2) {
				w->getPhysicsBody()->setCollisionBitmask(104);//floors don't actually collide with character because they get stuck on seems
				w->getPhysicsBody()->setContactTestBitmask(72);//floors check for contact with player and items
			}
			else {
				w->getPhysicsBody()->setContactTestBitmask(8);//ceilings only check with items
			}
		}
		w->setName(name);
		addChild(w);
	}
}

bool sortByNumber(PathNode* a, PathNode* b) { return (a->num) < (b->num); }//function for sorting vector of PathNodes

//creates a room, made of 4 walls, can have doors/vents and stairways
void Room::createRoom(vector<Door*> *doors, vector<Stair*> *stairs, vector<HideObject*> *hideObjects, vector<PhysObject*> *physObjects, vector<Item*> *items, vector<Enemy*> *enemies, vector<PathNode*> *pathNodes, Player* player, Vec2 position, RoomData* roomData, int height)
{	//setting size of room
	setContentSize(Size(roomData->width, height));
	setAnchorPoint(Vec2(0, 0));

	//setting background image for room
	background = Sprite::create("backgrounds/" + roomData->bgName);
	background->setContentSize(getContentSize() + Size(fullThick + 1, fullThick + 1));
	background->setGlobalZOrder(0);
	background->setAnchorPoint(Vec2(0, 0));
	background->setPosition(position - Vec2(thick, thick));
	addChild(background);

	//setting player position
	if (player->startRoom == roomData->room) {
		//player->setRoomPositionNormalized(position, Size(roomData.width, height), player->roomStartPos);
		player->setRoomPosition(position, player->roomStartPos);
	}
	//setting stairway positions
	if (stairs->size() > 0) {
		for (int i = 0; i < stairs->size(); i++) {
			if ((*stairs)[i]->startRoom == roomData->room) {
				//(*stairs)[i]->setRoomPositionNormalized(position, Size(roomData.width, height), (*stairs)[i]->roomStartPos);
				(*stairs)[i]->setRoomPosition(position, (*stairs)[i]->roomStartPos);
			}
		}
	}
	//setting hide object positions
	if (hideObjects->size() > 0) {
		for (int i = 0; i < hideObjects->size(); i++) {
			if ((*hideObjects)[i]->startRoom == roomData->room) {
				//(*hideObjects)[i]->setRoomPositionNormalized(position, Size(roomData.width, height), (*hideObjects)[i]->roomStartPos);
				(*hideObjects)[i]->setRoomPosition(position, (*hideObjects)[i]->roomStartPos);
			}
		}
	}
	//setting physical object positions
	if (physObjects->size() > 0) {
		for (int i = 0; i < physObjects->size(); i++) {
			if ((*physObjects)[i]->startRoom == roomData->room) {
				//(*physObjects)[i]->setRoomPositionNormalized(position, Size(roomData.width, height), (*physObjects)[i]->roomStartPos);
				(*physObjects)[i]->setRoomPosition(position, (*physObjects)[i]->roomStartPos);
			}
		}
	}
	//setting item positions
	if (items->size() > 0) {
		for (int i = 0; i < items->size(); i++) {
			if ((*items)[i]->startRoom == roomData->room) {
				//(*items)[i]->setRoomPositionNormalized(position, Size(roomData.width, height), (*items)[i]->roomStartPos);
				(*items)[i]->setRoomPosition(position, (*items)[i]->roomStartPos);
			}
		}
	}
	//setting path node positions
	if (pathNodes->size() > 0) {
		for (int i = 0; i < pathNodes->size(); i++) {
			//positioning path node
			if ((*pathNodes)[i]->startRoom == roomData->room) {
				(*pathNodes)[i]->setRoomPosition(position, (*pathNodes)[i]->roomStartPos);
				//assigning path node to correct enemy
				for (int j = 0; j < enemies->size(); j++) {
					if ((*enemies)[j]->getPathTag() == (*pathNodes)[i]->pathTag) {//if tags match
						(*enemies)[j]->pathNodes.push_back((*pathNodes)[i]);//add path node to enemies list of path nodes
					}
				}
			}
		}
	}
	//setting enemy positions
	if (enemies->size() > 0) {
		for (int i = 0; i < enemies->size(); i++) {
			if ((*enemies)[i]->startRoom == roomData->room) {
				//(*enemies)[i]->setRoomPositionNormalized(position, Size(roomData.width, height), (*enemies)[i]->roomStartPos);
				(*enemies)[i]->setRoomPosition(position, (*enemies)[i]->roomStartPos);
				if ((*enemies)[i]->pathNodes.size() > 0) {
					//adding starting position as a path node
					PathNode* startNode = new PathNode();
					startNode->initNode((*enemies)[i]->getPositionX(), 0, 3.0f, (*enemies)[i]->getPathTag());
					startNode->startRoom = (*enemies)[i]->startRoom;
					startNode->setPosition((*enemies)[i]->getPosition());
					pathNodes->push_back(startNode);
					(*enemies)[i]->pathNodes.push_back(startNode);
				}
				//sorting list of path nodes
				std::sort((*enemies)[i]->pathNodes.begin(), (*enemies)[i]->pathNodes.end(), sortByNumber);
			}
		}
	}

	//creating walls
	Vec2 wallPos = position - Vec2(thick, thick);//the position for generating the walls of the room

	//floor
	createWall(doors, 2, 2, wallPos, Size(roomData->width + fullThick, thick), roomData->bottomDoors, "floor");

	//ceiling
	createWall(doors, 2, 1, wallPos + Vec2(0, height + thick), Size(roomData->width + fullThick, thick), roomData->ceilingDoors, "ceiling");

	//left wall
	createWall(doors, 1, 2, wallPos + Vec2(0, thick), Size(thick, height), roomData->leftDoors, "wall");

	//right wall
	createWall(doors, 1, 1, wallPos + Vec2(roomData->width + thick, thick), Size(thick, height), roomData->rightDoors, "wall");
}
