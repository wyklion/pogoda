#ifndef __FOUND_LAYER_H__
#define __FOUND_LAYER_H__   

#include "cocos2d.h" 
USING_NS_CC;
#include "UserData.h"


class FoundLayer : public CCLayer
{
public:
	virtual bool init(BLOCK_TYPE type);
	static FoundLayer* create(BLOCK_TYPE type)
	{
		FoundLayer *pRet = new FoundLayer();
		if (pRet && pRet->init(type))
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
	void remove(CCNode* obj);
	void done(CCNode* obj);
	void close();

public:
	bool m_allShow;

private:
	CCNode* m_node;
	CCSize m_size;
};

#endif