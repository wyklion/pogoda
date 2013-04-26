#ifndef __FIRST_LAYER_H__
#define __FIRST_LAYER_H__   
#include "cocos2d.h" 
USING_NS_CC;
#include "cocos-ext.h"
USING_NS_CC_EXT;
#include "UserData.h"

#define PIC_POINT_NORMAL "pointNormal.png"
#define PIC_POINT_SELECTED "pointSelected.png"

class FirstLayer : public CCLayer, public CCScrollViewDelegate
{
public:	
	virtual bool init(); //CCLayer�Ķ����Ƕ������������������virtual�ؼ���
	CREATE_FUNC(FirstLayer);
	
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
	
	void adjustScrollView(float offset);

public:

private:
	CCLayer* m_helpLayer;
	CCScrollView* m_scroll;
	int m_nCurPage;
	CCPoint m_touchPoint;
	CCPoint m_offsetPoint;
};


#endif