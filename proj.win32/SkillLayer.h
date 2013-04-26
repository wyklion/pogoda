#ifndef __SKILL_LAYER_H__
#define __SKILL_LAYER_H__   
#include "cocos2d.h" 
USING_NS_CC;
#include "cocos-ext.h"
USING_NS_CC_EXT;
#include "UserData.h"

class SkillLayer : public CCLayerColor, public CCScrollViewDelegate
{
public:	
	virtual bool init(); //CCLayer的儿子们都有这玩意儿。别忘了virtual关键字
	CREATE_FUNC(SkillLayer);
	
	//scrollview滚动的时候会调用
	void scrollViewDidScroll(CCScrollView* view);
	//scrollview缩放的时候会调用
	void scrollViewDidZoom(CCScrollView* view);

	//重写触屏相关函数----   
	virtual void onEnter();
	virtual void onExit();  
	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* pEvent);    
	virtual void ccTouchMoved(CCTouch* touch, CCEvent* pEvent);  
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	
	void adjustScrollView();

public:
	void back(CCObject* pSender);	
	void buySkill(CCObject *pSender);

	void createThings();
	void updateSkill();
	void expand(int index);
	void fold(int index);
	void unfold(int index);

private:
	int m_expanding;
	bool m_isScroll;
	CCNode* m_items[SKILL_NUM];
	CCLayer* m_itemLayer;
	CCPoint m_touchPoint;
	CCPoint m_offsetPoint;
	CCScrollView* m_itemScroll;
};


#endif