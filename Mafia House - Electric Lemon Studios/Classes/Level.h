#pragma once
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "InputHandler.h"
#include "Functions.h"
#include <vector>
#include <string>
#include <iomanip>
#include<fstream>
#include "Player.h"
#include "Enemy.h"
#include "Item.h"
#include "Wall.h"
#include "EnvObject.h"
#include "Stair.h"
#include "GameLayer.h"
#include "Noise.h"
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

	void pauseScreen(float deltaTime);

	void onStart(float deltaTime);

	void getStats(float deltaTime);

	void onEnd(float deltaTime);

	void gameOver(float deltaTime);

protected:
	Texture2D::TexParams texParams = { GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };

	int levelNum;

	float levelFinishTime = -1;
	float completeScreenDisplayTime = 6.0f;//minimum time the completion screen will display for

	bool gotBestTime = false;
	bool fullAssassin = false;//kill all enemies without being detected
	bool silentSpectre = false;//kill no enemies without being detected
	bool levelComplete = false;
	int numBosses = 0;
	int numEnemies = 0;
	int numKilled = 0;
	//mainLayer->numTimesDetected;

	Sprite* completionScreen;
	Label* numKilledDisplay;
	Label* numTimesDetectedDisplay;
	Label* completeTimeDisplay;
	Label* fullAssassinLabel;
	Label* silentSpectreLabel;
	Label* continueLabel;

	Sprite* achievement1;
	Sprite* achievement2;
	Sprite* bestTime;

	Sprite* gameOverScreen;
	bool initGameOver = false;
	bool initGameOverLabels = false;

	Sprite* darkenScreen;

	float gameTime = 0.0f;

	Director* director = Director::getInstance();

	Node* hudLayer;//the layer for the HUD/UI, doesn't move with camera
	Node* pauseLayer;//layer for pause screen
	Sprite* healthBar;
	Sprite* healthFill;
	Sprite* playerHead;
	Sprite* itemBar;
	Sprite* itemFill;
	Sprite* itemIcon;

	GameLayer* mainLayer;//a node which everything in the scene is attached to

	Sprite* background;//the background image of the level
	float backgroundScale = 1.0f;

	DrawNode* gunShots;

	DrawNode* noiseCircles;//used for drawing circles to visualize noises

	Player* player;//the player character
	
	vector<Enemy*> enemies;//the enemies in the level
	vector<PathNode*> pathNodes;//the path nodes for the enemies

	DrawNode* visionRays;//used for drawing vision cones

	vector<HideObject*> hideObjects;//the hide objects in the level

	vector<PhysObject*> physObjects;//the physical objects in the level

	vector<Room*> rooms;//the rooms the level is made up of

	vector<Door*> doors;

	Node* camPos;//an invisible node used to position the camera
	Camera* camera = NULL;
	Vec2 camBoundingBox = Vec2(200, 50);
	Vec2 camOffset = Vec2(0, 150);//the offset for the camera, so player isn't in exact centre of screen
	float camZoom = 1.0f;

	//for doulbe tap inputs:
	float prevLeftPressTime = -1;
	float prevRightPressTime = -1;
	float doublePressTime = 12 FRAMES;//amount of time for a doulbe press to register

	bool onContactBegin(PhysicsContact &contact);

	bool onContactPreSolve(PhysicsContact &contact, PhysicsContactPreSolve & solve);//main function used for collision detection

	//level generation functions; rooms, doors, stairs, objects, items, & enemies are the input parameters
	void createFloor(Vec2 position, vector<RoomData*> *roomData, int height);
	void createLevel(Vec2 position, float levelWidth);
};
