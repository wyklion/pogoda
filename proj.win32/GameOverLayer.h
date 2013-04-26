#ifndef __GAME_OVER_H__
#define __GAME_OVER_H__   

#include "cocos2d.h" 
USING_NS_CC;
#include "PlayLayer.h"

class GameOverLayer : public CCLayerColor
{
public:
	~GameOverLayer();

	virtual bool initWithData(const GameProcess& data); //CCLayer�Ķ����Ƕ������������������virtual�ؼ���   
	
	static GameOverLayer* create(const GameProcess& data)
	{
		GameOverLayer* pRet = new GameOverLayer();
		if (pRet && pRet->initWithData(data))
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			delete pRet;
			pRet = NULL;
			return NULL;
		}
	}
public:
	void quit(CCObject* pSender);
	void menuLottery(CCObject* pSender);
	void callBackLottery(CCNode* node);
	void callBackRemove(CCNode* node);
};

#endif