#ifndef __MAINMENU_SCENE_H__
#define __MAINMENU_SCENE_H__

#include "cocos2d.h"

class MainMenuScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(MainMenuScene);

private:
	void GoToGameScene(cocos2d::Ref *sender);
};

#endif // __MAINMENU_SCENE_H__
