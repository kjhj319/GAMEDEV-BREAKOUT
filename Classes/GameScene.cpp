#include "GameScene.h"
#include "Box2D/Box2D.h"
#include <math.h>
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;

}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto backgroundSprite = Sprite::create("Background.png");
	backgroundSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(backgroundSprite);

	ball = Sprite::create("ball1.png");
	ball->setPosition(visibleSize.width / 2 - 20, 100);
	this->addChild(ball);

	// Similar with the ball
	paddle = Sprite::create("bar.jpg");
	paddle->setPosition(visibleSize.width / 2, 50);
	this->addChild(paddle);

	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 60; j += 20)
		{
			cocos2d::Sprite *wood = Sprite::create("wood.png");
			wood->setPosition(i * 40 + 20, 500 + j);
			this->addChild(wood);
			woods.push_back(wood);
		}
	}

	score = 0;
	scoreText = ("Score: 0");
	scoreLabel = Label::createWithTTF(scoreText, "fonts/Marker Felt.ttf", visibleSize.height * 0.05);
	scoreLabel->setColor(Color3B::BLACK);
	scoreLabel->setPosition(Point(visibleSize.width * 0.9 + origin.x, visibleSize.height * 0.95 + origin.y));
	this->addChild(scoreLabel);

	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(GameScene::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));

	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);

	setDirection(0, 1);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	this->schedule(schedule_selector(GameScene::tick), 0.004);
	//0.002
    return true;
}

void GameScene::onTouchMoved(Touch *touch, Event *event)
{
	Point touchLocation = this->convertToWorldSpace(this->convertTouchToNodeSpace(touch));

	paddle->setPositionX(touchLocation.x);
}
void GameScene::onTouchEnded(Touch *touch, Event *event)
{

}

bool GameScene::onTouchBegan(Touch *touch, Event *event)
{
	return true;
}

void GameScene::tick(float dt)
{
	checkCollision();
	ball->setPosition(ball->getPositionX() + dirx, ball->getPositionY() + diry);
}

void GameScene::setDirection(double directionX, double directionY)
{
	double length = sqrt(directionX * directionX + directionY * directionY);
	dirx = (directionX / length);
	diry = (directionY / length);
}

void GameScene::setOppoDirection(double directionX, double directionY)
{
	dirx *= directionX;
	diry *= directionY;
}

void GameScene::checkCollision()
{
	if (ball->getPositionX() < 0) {
		ball->setPosition(0, ball->getPositionY());
		setOppoDirection(-1, 1);
	}
	else if (ball->getPositionX() + ball->getContentSize().width > 800) {
		ball->setPosition(800 - ball->getContentSize().width, ball->getPositionY());
		setOppoDirection(-1, 1);
	}

	else if (ball->getPositionY() + ball->getContentSize().height > 600){
		ball->setPosition(ball->getPositionX(), 600 - ball->getContentSize().height);
		setOppoDirection(1, -1);
	}

	else if (ball->getPositionY() < 0) {
		init();
	}

	CCRect box1Rect = ball->boundingBox();
	CCRect box2Rect = paddle->boundingBox();
	double ballCenter = ball->getPositionX() + ball->getContentSize().width / 2;
	if (box1Rect.intersectsRect(box2Rect)) {
		ball->setPosition(ball->getPositionX(), paddle->getPositionY() + paddle->getContentSize().height);
		setDirection(getReflection(ballCenter - paddle->getPositionX()), 1);
	}

	for (auto itr = woods.begin(); itr != woods.end();)
	{
		CCRect box1Rect = ball->boundingBox();
		CCRect box2Rect = (*itr)->boundingBox();

		auto left = ball->getPositionX();
		auto top = ball->getPositionY() + ball->getContentSize().height;
		auto width = ball->getContentSize().width;
		auto height = ball->getContentSize().height;
		Vec2 leftPoint(left, top + (height / 2.f));                 
		Vec2 rightPoint(left + width, top - (height / 2.f));   
		Vec2 topPoint(left + (width / 2.f), top);                   
		Vec2 bottomPoint(left + (width / 2.f), ball->getPositionY());

		if (box1Rect.intersectsRect(box2Rect))
		{
			if (box2Rect.containsPoint(leftPoint) || box2Rect.containsPoint(rightPoint))
			{
				setOppoDirection(1, -1);
			}
			else if (box2Rect.containsPoint(topPoint) || box2Rect.containsPoint(bottomPoint))
			{
				setOppoDirection(-1, 1);
			}

			else
			{
				setOppoDirection(-1, 1);
			}
			this->removeChild(*itr);
			itr = woods.erase(itr);
			score++;
			scoreText = ("Score: " + std::to_string(score));
			scoreLabel->setString(scoreText);
			auto eatingAudio = CocosDenshion::SimpleAudioEngine::getInstance();
			eatingAudio->playEffect("Eating.wav");
		}
		else
		{
			++itr;
		}
	}

	if (woods.size() == 0)
	{
		init();
	}
}

double GameScene::getReflection(double hit)
{
	if (hit < 0) {
		hit = 0;
	}
	else if (hit > paddle->getContentSize().height) {
		hit = paddle->getContentSize().height;
	}

	hit -= paddle->getContentSize().height / 2.0f;

	return 2.0f * (hit / (paddle->getContentSize().height / 2.0f));
}

void GameScene::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}