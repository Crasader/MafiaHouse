#pragma once
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "InputHandler.h"
#include "Functions.h"
#include <vector>
#include <string>
#include<fstream>
#include "Player.h"
#include "Enemy.h"
#include "Item.h"
#include "Wall.h"
#include "EnvObject.h"
#include "Stair.h"
USING_NS_CC;
using std::vector;
using std::string;
using std::atoi;
using std::atof;

#define CREATE_SCENE_FUNC(_TYPE_) \
static Scene* createScene() \
{ \
	Scene* scene = Scene::createWithPhysics(); \
	_TYPE_* layer = _TYPE_::create(); \
	scene->addChild(layer); \
	return scene; \
}

#define RESET_LEVEL \
void resetLevel() \
{ \
	Scene* restartLevel = createScene(); \
	director->replaceScene(restartLevel); \
}

class Level: public Scene
{
public:
	virtual void resetLevel() = 0;

	void setup();//call in init(), initializes stuff that is the same for each level

	void setBackground(string bgName = "defaultbg.png", float scale = 1.0f);

	bool initLevel(string filename);//call in init(), creates everything in the level from a text file

	void update(float deltaTime);

	void onStart(float deltaTime);

	void resetCollisionChecks();//resets variables used for collision detection checking

protected:
	float gameTime = 0.0f;

	Director* director = Director::getInstance();

	Node* hudLayer;//the layer for the HUD/UI, doesn't move with camera

	Node* mainLayer;//a node which everything in the scene is attached to

	Sprite* background;//the background image of the level
	float backgroundScale = 1.0f;

	Player* player;//the player character
	
	vector<Enemy*> enemies;//the enemies in the level
	int enemyHit = -1;

	DrawNode* visionRays;//used for drawing vision cones

	vector<Item*> items;//the items in the level

	vector<EnvObject*> objects;//the environmental objects in the level

	vector<Room*> rooms;//the rooms the level is made up of

	vector<Door*> doors;

	vector<Stair*> stairs;

	Node* camPos;//an invisible node used to position the camera
	Camera* camera = NULL;
	Vec2 camBoundingBox = Vec2(200, 0);
	Vec2 camOffset = Vec2(0, 150);//the offset for the camera, so player isn't in exact centre of screen
	float camZoom = 1.0f;

	bool onContactBegin(PhysicsContact &contact);

	bool onContactPreSolve(PhysicsContact &contact, PhysicsContactPreSolve & solve);//main function used for collision detection

	unsigned walkingID; //store the id of walking audio

	//level generation functions; rooms, doors, stairs, objects, items, & enemies are the input parameters
	void createFloor(vector<Room*> *rooms, vector<Door*> *doors, vector<Stair*> *stairs, vector<EnvObject*> *objects, vector<Item*> *items, vector<Enemy*> *enemies, Player* player, Vec2 position, vector<RoomData> roomData, int height);
	void createLevel(vector<Room*> *rooms, vector<Door*> *doors, vector<Stair*> *stairs, vector<EnvObject*> *objects, vector<Item*> *items, vector<Enemy*> *enemies, Player* player, Vec2 position, float levelWidth, vector<FloorData> floorData);
};
