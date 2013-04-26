#ifndef __INFO_LAYER_H__
#define __INFO_LAYER_H__   
#include "cocos2d.h" 
USING_NS_CC;
#include "UserData.h"

class InfoLayer : public CCLayer
{  
public:
	~InfoLayer();
	virtual bool init(); //CCLayer的儿子们都有这玩意儿。别忘了virtual关键字   

	/*static InfoLayer* create(const GameProcess& data)
	{
		InfoLayer* pRet = new InfoLayer();
		if (pRet && pRet->initWithStep(data))
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
	}*/
	CREATE_FUNC(InfoLayer);   
	//千万别忘了这个风情万种的宏

public:
	void updateInfo();
private:
	void showBackGround();
	void setStep(int step);
	void setMoney(int money);
	void setGold(int gold);

private:
	CCLabelTTF* m_stepLabel;
	CCLabelTTF* m_moneyLabel;
	CCLabelTTF* m_goldLabel;
};

#endif


