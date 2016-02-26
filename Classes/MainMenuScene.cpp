#include "MainMenuScene.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* MainMenuScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainMenuScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainMenuScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto backgroundSprite = Sprite::create( "Background.png" );
	backgroundSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	this->addChild( backgroundSprite );

	cocos2d::Label *snakeLabel = Label::createWithTTF("Break Out", "fonts/Marker Felt.ttf", visibleSize.height * 0.15);
	snakeLabel->setColor(Color3B::BLACK);
	snakeLabel->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.8 + origin.y));
	this->addChild(snakeLabel);

	auto playButton = MenuItemImage::create("PlayButton.png", "PlayButton.png", CC_CALLBACK_1(
											MainMenuScene::GoToGameScene, this));
	playButton->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	auto menu = Menu::create(playButton, NULL);
	menu->setPosition(Point::ZERO);

	this->addChild(menu);

    return true;
}

void MainMenuScene::GoToGameScene(cocos2d::Ref *sender)
{
	auto scene = GameScene::createScene();

	Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
}