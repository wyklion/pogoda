#ifndef __LOADING_H__
#define __LOADING_H__   

#include "cocos2d.h" 
USING_NS_CC;

class InitLoading : public CCLayer
{
public:
	virtual bool init(); 
	CREATE_FUNC(InitLoading);   

    void addSprite();
    void loadingCallBack(cocos2d::CCObject *obj);

private:
    cocos2d::CCLabelTTF *m_pLabelLoading;
    cocos2d::CCLabelTTF *m_pLabelPercent;
    int m_nNumberOfSprites;
    int m_nNumberOfLoadedSprites;
};


#endif