#ifndef __ADMIN_LAYER_H__
#define __ADMIN_LAYER_H__   

#include "cocos2d.h" 
USING_NS_CC;
#include "UserData.h"

class AdminLayer : public CCLayerColor
{
public:
	virtual bool init();
	CREATE_FUNC(AdminLayer);
public:
	void addMoney(CCObject* obj);
	void reset(CCObject* obj);
	void back(CCObject* obj);
};

#endif