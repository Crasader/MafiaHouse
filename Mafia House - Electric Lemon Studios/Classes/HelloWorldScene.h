#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
using namespace cocos2d;

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

	// update function
	void update(float deltaTime);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	//setup to help initialize the scene
	void setup();

	void Step(float dt);

private:
	//--- Cocos Engine References ---//
	Director* director;
	Vec2 windowSize;
	float gameTime = 0.0f;

	//character
	Sprite* sprite;
	Sprite* character;
	Sprite* background;
	Vec2 characterPosition = Vec2(79, 200);
	float characterSpeed = 10.0f;


	int a = 0;
};

#endif // __HELLOWORLD_SCENE_H__
