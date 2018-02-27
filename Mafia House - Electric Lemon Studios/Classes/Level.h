#pragma once
#include "cocos2d.h"
#include "InputHandler.h"
#include <vector>
#include <string>
#include "Player.h"
#include "Enemy.h"
#include "Item.h"
#include "Wall.h"
USING_NS_CC;

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
	
	std::vector<Enemy*> enemies;//the enemies in the level

	std::vector<Item*> items;//the items in the level

	std::vector<Wall*> walls;//the physical boundaries of the level

	Vec2 camOffset = Vec2(0, 300);//the offset for the camera, so player isn't in exact centre of screen

	bool onContactBegin(PhysicsContact &contact);

	bool onContactPreSolve(PhysicsContact &contact, PhysicsContactPreSolve & solve);//main function used for collision detection

	void follow(Node* nodeA, Node* nodeB, float radius = 0.0f, Vec2 offset = Vec2(0,0));//used to make one node follow another
};
