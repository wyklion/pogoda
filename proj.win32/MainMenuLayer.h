#ifndef __MAIN_MENU_LAYER_H__
#define __MAIN_MENU_LAYER_H__

#include "cocos2d.h"
USING_NS_CC;
#include "cocos-ext.h"
USING_NS_CC_EXT;

class MainMenuScene : public CCScene
{
public:
	MainMenuScene(void);
	~MainMenuScene(void);

	virtual bool init();
	CREATE_FUNC(MainMenuScene);
	
};


class MainMenuLayer : public CCLayer
{
public:
	MainMenuLayer() {};
	virtual ~MainMenuLayer();
	virtual bool init();
	CREATE_FUNC(MainMenuLayer);

public:
	void callbackAdmin(CCObject *pSender);
	void menuCallbackPlay(CCObject* sender);
	void menuCallbackSkill(CCObject* sender);
	void menuCallbackRecording(CCObject* sender);
	void music(CCObject *pSender);
	void jumpForever(CCNode* obj);
};

#endif