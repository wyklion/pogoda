#ifndef __BLOCK_LAYER_H__
#define __BLOCK_LAYER_H__   
#include "cocos2d.h" 
USING_NS_CC;
#include "BlockData.h"
#include "basic.h"

struct BlockLevelData
{
	int level;
	int openTime;
};

class BlockLayer : public CCLayer  
{  
	friend class PlayLayer;
public:
	~BlockLayer();
	virtual bool initWithLevel(int level); //CCLayer的儿子们都有这玩意儿。别忘了virtual关键字   
	
	static BlockLayer* create(int step)
	{
		BlockLayer* pRet = new BlockLayer();
		if (pRet && pRet->initWithLevel(step))
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
	//CREATE_FUNC(BlockLayer);   
	//千万别忘了这个风情万种的宏

	//重写触屏相关函数----   
	virtual void onEnter();
	virtual void onExit();  
	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);    
	virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);  
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
public:
	BLOCK_TYPE getBlockTypeByTag(int tag) { return m_blockData->getBlockType(tag);}
	float winAndClearBlock();
	void showHiddenGold();
	void openRandomGold();
	void sweepBlankBlock();
	void showAllDeadBlock();
	void showSomeDeadBlock(int num);
	static void showGetting(CCLayer* layer, const CCPoint& p, char* picName, int num, int index = 0);
	void showGetting(const CCPoint& p, char* picName, int num, int index = 0);
public:
	void callBackClearBlock(CCObject* obj);
	void callBackRemove(CCNode* obj);
private:
	bool addBlockThing(BLOCK_TYPE type, int tag);
	CCSprite* getBlockThing(int tag);
	void showHideBlock();
	void showBlockThings(int tag);
	void showAllBlock();
	void openAnimation(int tag);
	void hilit(int tag);
	void deHilit();

private:
	//prop func...
	void callBackRealOpen(CCObject* obj);
	void showGetting(int tag);// show prop change value on block...
	void lightBlockThing(CCSprite* blockThing);
	void lightBlockAround(int tag);
	void propOnBlock(int tag);
	void propBombOnBlock(int tag);
	void propMagnifierOnBlock(int tag);
	void moveThing(int tag);
	void blinkAndRemoveThing(int tag);
	void bigAndSmallThing(int tag);
	void afterBigAndSmall(CCNode* node);

private:
	int m_unit;
	int m_level;
	CCSpriteBatchNode* m_batchNode;
	CCSpriteBatchNode* m_backgroundBatchNode;
	BlockLayerData* m_blockData;
	int m_touchTag;
	CCSprite* m_hilitBlock;
private:
	std::set<int> m_lightTags;
	std::set<int> m_moveTags;

	BlockLevelData m_levelData;
};

#endif

