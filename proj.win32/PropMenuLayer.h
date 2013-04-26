#ifndef __PROP_MENU_LAYER_H__
#define __PROP_MENU_LAYER_H__   

#include "cocos2d.h" 
USING_NS_CC;
#include "basic.h"

enum PropOption
{
	PROP_OPTION_CANCEL,
	PROP_OPTION_DO_CANCEL,
};

class PropMenuLayer : public CCLayer
{
public:
	~PropMenuLayer(void);
	virtual bool init();	
	CREATE_FUNC(PropMenuLayer);
	
	void set(BLOCK_TYPE type);
	
	void menuPropDo(CCObject* pSender);
	void menuPropCancel(CCObject* pSender);
	void menuPropSell(CCObject* pSender);
public:
	void showInfoBack(BLOCK_TYPE type);
	void showPropOption(PropOption option);
	void clearPropMenu();

private:
	CCMenu* m_menuPropDo;
	CCMenu* m_menuPropCancel;
	CCMenu* m_menuPropSell;
	CCNode* m_infoBack;
};

#endif