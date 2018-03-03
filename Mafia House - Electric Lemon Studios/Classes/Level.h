#pragma once
#include "cocos2d.h"
#include "InputHandler.h"
#include <vector>
#include <string>
#include "Player.h"
#include "Enemy.h"
#include "Item.h"
#include "Wall.h"
#include "EnvObject.h"
USING_NS_CC;
using std::vector;

class Level: public Scene
{
public:
	Level();
	~Level();

	void setup();//call in init(), initializes stuff that is the same for each level

	void update(float deltaTime);

protected:

	Director* director = Director::getInstance();

	Node* hudLayer;//the layer for the HUD/UI, doesn't move with camera

	Node* mainLayer;//a node which everything in the scene is attached to, moved around to act as a camera following player

	Node* camPos;//an invisible node used to position the "camera"

	Sprite* background;//the background image of the level
	std::string backgroundName = "defaultbg.png";

	Player* player;//the player character
	
	vector<Enemy*> enemies;//the enemies in the level

	vector<Item*> items;//the items in the level

	vector<EnvObject*> objects;//the environmental objects in the level

	vector<Room*> rooms;//the rooms the level is made up of

	vector<Door*> doors;

	vector<Stair*> stairs;

	Vec2 camBoundingBox = Vec2(200, 100);
	Vec2 camOffset = Vec2(0, 300);//the offset for the camera, so player isn't in exact centre of screen

	bool onContactBegin(PhysicsContact &contact);

	bool onContactPreSolve(PhysicsContact &contact, PhysicsContactPreSolve & solve);//main function used for collision detection

	void followRadius(Node* nodeA, Node* nodeB, float radius = 0.0f, Vec2 offset = Vec2(0,0));//used to make one node follow another
	//nodeA will 'chase' nodeB; you can also use this to 'leash' nodeA to nodeB instead

	void followBox(Node* nodeA, Node* nodeB, Vec2 range, Vec2 offset = Vec2(0, 0));//like the followRadius function, but uses a rectangular bounding box instead of circular radius

	//for keyboard inputs, becuase getKeyPress doesn't work from within onConctactPresolve for some reason
	bool space_press = false;
	bool ctrl_press = false;

	//level generation functions; rooms, doors, stairs, objects, items, & enemies are the input parameters
	void createFloor(vector<Room*> *rooms, vector<Door*> *doors, vector<Stair*> *stairs, vector<EnvObject*> *objects, vector<Item*> *items, vector<Enemy*> *enemies, Vec2 position, vector<RoomData> roomData, int height);
	void createLevel(vector<Room*> *rooms, vector<Door*> *doors, vector<Stair*> *stairs, vector<EnvObject*> *objects, vector<Item*> *items, vector<Enemy*> *enemies, Vec2 position, float levelWidth, vector<FloorData> floorData);
};
