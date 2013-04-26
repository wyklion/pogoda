#ifndef __PROP_LAYER_H__
#define __PROP_LAYER_H__   

#include "cocos2d.h" 
USING_NS_CC;
#include "BlockLayer.h"
#include "UserData.h"

class PropLayer : public CCLayer
{
public:
	~PropLayer(void);
	virtual bool init();
	CREATE_FUNC(PropLayer);   
	
	//重写触屏相关函数----   
	virtual void onEnter();
	virtual void onExit();  
	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);    
	virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);  
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);

public:
	int getCellNum(BLOCK_TYPE type);
	bool addProp(BLOCK_TYPE type);
	
	void movePropToFront(int index);
	void updatePropCells();
	CCSprite* getProp(int tag);
	void selectProp(int tag);
	void unselectProp();
	bool getTouchPointTag(const CCPoint& point, int& col);

private:
	int m_selectTag;
};

#endif