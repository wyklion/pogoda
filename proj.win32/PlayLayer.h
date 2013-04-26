#ifndef __PLAY_LAYER_H__
#define __PLAY_LAYER_H__   
#include "cocos2d.h" 
USING_NS_CC;
#include "BlockLayer.h"
#include "InfoLayer.h"
#include "PropLayer.h"
#include "PropMenuLayer.h"
#include "FoundLayer.h"

class PlayLayer : public CCLayer  
{  
public:
	~PlayLayer();
	virtual bool init(); //CCLayer的儿子们都有这玩意儿。别忘了virtual关键字
	CREATE_FUNC(PlayLayer); //千万别忘了这个风情万种的宏

	//重写触屏相关函数----   
	virtual void onEnter();
	virtual void onExit();  
	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);    
	virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);  
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);

public:
	void back (CCObject* pSender);  
	void menuStopCallback(CCObject* pSender);
	void setPropToPropLayer(CCNode* obj);
	void menuPropDo(CCObject* pSender);
	void menuPropCancel(CCObject* pSender);
	void menuPropSell(CCObject* pSender);
	void removeSprite(CCNode* obj);

public:
	void getPropToPropPack(CCSprite* prop, BLOCK_TYPE type);
	void setStopVisible(bool b);
	void newLevel();
	void levelComplete();
	void showLevelPassInfo();
	void gameOver();
	void gameUpdate();
	void checkProp(BLOCK_TYPE type);
	void updateInfo() { m_infoLayer->updateInfo() ;}
	void foundNewBlockThing(BLOCK_TYPE type);

public:
	//prop
	void updatePropCells() { m_propLayer->updatePropCells(); }
	bool isPropCellFull(BLOCK_TYPE type) { return m_propLayer->getCellNum(type) < 0;}
	void clearPropMenu();

private:
	void showLevel();
	void showHelp();
	void removeHelp();
	void callbackRemoveHelp(CCNode* node);
	bool newthings();
	void removeNewthing();
	void callbackRemoveNewthing(CCNode* node);

private:
	CCMenu* m_stopMenu;
	InfoLayer* m_infoLayer;
	PropLayer* m_propLayer;
	PropMenuLayer* m_propMenuLayer;
	BlockLayer* m_blockLayer;
	CCLayerColor* m_background;
	FoundLayer* m_foundLayer;
	CCNode* m_help;
	CCNode* m_newthing;

	bool m_done;//dont win or lose more times.
};

#endif

