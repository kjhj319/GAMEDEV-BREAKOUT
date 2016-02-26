#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"

class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
	void move(float);
	void menuCloseCallback(cocos2d::Ref* pSender);
	void tick(float);

	void setPhyWorld(cocos2d::PhysicsWorld *world);
	// Touch event
	void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);

	void setDirection(double directionX, double directionY);
	void setOppoDirection(double directionX, double directionY);
	bool checkPaddleCollision();
	void checkCollision();
	double getReflection(double hit);

    virtual bool init();

private:
	cocos2d::Label *scoreLabel;
	std::string scoreText;

	cocos2d::Sprite *ball;
	cocos2d::Sprite *paddle;
	std::vector<cocos2d::Sprite*> woods;
	float dirx, diry;
	int score;

	cocos2d::PhysicsWorld *m_world;

    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
};

#endif // __GAME_SCENE_H__
