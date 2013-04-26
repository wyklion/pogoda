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
	virtual bool init(); //CCLayer�Ķ����Ƕ������������������virtual�ؼ���
	CREATE_FUNC(SkillLayer);
	
	//scrollview������ʱ������
	void scrollViewDidScroll(CCScrollView* view);
	//scrollview���ŵ�ʱ������
	void scrollViewDidZoom(CCScrollView* view);

	//��д������غ���----   
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