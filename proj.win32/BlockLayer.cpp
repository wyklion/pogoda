#include "BlockLayer.h"
#include "PlayLayer.h"
#include "MySound.h"

bool BlockLayer::initWithLevel(int level)  
{  
	if (!CCLayer::init())  
	{  
		return false;  
	}  
	// elastic effect
    CCSize s = CCDirector::sharedDirector()->getWinSize();
	m_unit = s.width / BLOCK_COL;

	//CCMenuItemFont* back=CCMenuItemFont::create("back",this,menu_selector(BlockLayer::back));
	//back->setFontSize(50);
	//CCSize backsize = back->getContentSize();
	////老问题 别忘记类名 还有这个只需要函数名就够了  
	//CCMenu* menu=CCMenu::create(back,NULL);   
	////Items！ 单复数 要注意   
	//menu->setPosition(ccp(s.width*0.5, backsize.height*0.5));
	//this->addChild(menu);  
	/*
	m_picName = "girl.png";	
	CCTextureCache::sharedTextureCache()->addImage(m_picName);*/

	m_levelData.level = level;
	m_levelData.openTime = 0;

	m_hilitBlock = NULL;
	m_level = level;
	m_blockData = new BlockLayerData(level);
	showHideBlock();
	showAllBlock();

	return true;
}  

BlockLayer::~BlockLayer()
{
	delete m_blockData;
}

void BlockLayer::showHideBlock()
{
	m_backgroundBatchNode = CCSpriteBatchNode::create(PIC_BLOCK_BACKGROUND, BLOCK_NUM);
	m_backgroundBatchNode->setAnchorPoint(ccp(0,0));
	m_backgroundBatchNode->setPosition(0,0);
	this->addChild(m_backgroundBatchNode, LAYERNUM_BACKGROUND);
	for(int i = 0; i < BLOCK_NUM; i++)
	{
		int row = i / BLOCK_COL;
		int col = i % BLOCK_COL;
		CCSprite* block = CCSprite::create(PIC_BLOCK_BACKGROUND);
		block->setAnchorPoint(ccp(0,0));
		block->setPosition(ccp(m_unit*col, m_unit*row));
		m_backgroundBatchNode->addChild(block, 0, i);
		showBlockThings(i);			
	}
}

bool BlockLayer::addBlockThing(BLOCK_TYPE type, int tag)
{
	if(type == BLOCK_NONE || type == BLOCK_NUMBER)
		return false;
	CCSprite* item = CCSprite::create(s_blockContent[type].picName.c_str());
	item->setAnchorPoint(ccp(0,0));
	item->setPosition(m_backgroundBatchNode->getChildByTag(tag)->getPosition());
	addChild(item, LAYNUM_THING, 2000+tag);
	return true;
}

void BlockLayer::showBlockThings(int tag)
{
	int num = m_blockData->getBlockNum(tag);
	BLOCK_TYPE type = m_blockData->getBlockType(tag);
	if(type == BLOCK_NUMBER)
	{
		CCLabelTTF* label = CCLabelTTF::create("", "Marker Felt", (float)m_unit, CCSizeMake(m_unit, m_unit), kCCTextAlignmentCenter);
		label->setAnchorPoint(ccp(0, 0));
		label->setPosition(m_backgroundBatchNode->getChildByTag(tag)->getPosition());
		char buf[5];
		sprintf(buf, "%d", num);
		label->setString(buf);
		if(num == 1)
			label->setColor(ccc3(255,215,0));
		else if(num == 2)
			label->setColor(ccc3(127,255,0));
		else if(num == 3)
			label->setColor(ccc3(128,42,42));
		else if(num == 4)
			label->setColor(ccc3(16,32,240));
		else if(num == 5)
			label->setColor(ccc3(255,0,0));
		else if(num == 6)
			label->setColor(ccc3(0,255,0));
		else if(num == 7)
			label->setColor(ccc3(0,0,255));
		else if(num == 8)
			label->setColor(ccc3(0,0,0));
		addChild(label, LAYNUM_THING, 2000+tag);
	}
	else
		addBlockThing(type, tag);
}

void BlockLayer::showAllBlock()
{	
	m_batchNode = CCSpriteBatchNode::create(PIC_BLOCK, BLOCK_NUM);
	m_batchNode->setAnchorPoint(ccp(0,0));
	m_batchNode->setPosition(0,0);
	this->addChild(m_batchNode, LAYERNUM_BATCH);
	int order = 0;
	for(int i = 0; i < BLOCK_ROW; i++)
	{
		for(int j = 0; j < BLOCK_COL; j++)
		{
			CCSprite* block = CCSprite::create(PIC_BLOCK);
			block->setAnchorPoint(ccp(0,0));
			block->setPosition(ccp(m_unit*j, m_unit*i));
			m_batchNode->addChild(block, 0, order);
			order++;
		}
	}
}

void BlockLayer::callBackClearBlock(CCObject* obj)
{
	CCSprite* block = (CCSprite*)obj;
	block->removeFromParentAndCleanup(true);
	block = getBlockThing(block->getTag());
	if(block)
		block->setOpacity(255);
}

float BlockLayer::winAndClearBlock()
{
	m_blockData->setAllOpen();
	float time = 0.1f;
	CCArray* blockArray = m_batchNode->getChildren();
	for(int i = 0; i < (int)m_batchNode->getChildrenCount(); i++)
	{
		//if(m_blockData->isBlockOpen(i))
			//continue;
		CCDelayTime* waitAction = CCDelayTime::create(time);
		CCSprite* block = (CCSprite*)blockArray->objectAtIndex(i);//(CCSprite*)m_batchNode->getChildByTag(i);
		CCFiniteTimeAction* actions=CCSequence::create(waitAction,
			CCCallFuncO::create(this, callfuncO_selector(BlockLayer::callBackClearBlock), block),
			NULL);
		block->runAction(actions);
		time += 0.1f;
	}
	return time;
}

void BlockLayer::showHiddenGold()
{
	for(int i = 0; i < BLOCK_NUM; i++)
	{
		if(m_blockData->isBlockOpen(i))
			continue;
		if(m_blockData->getBlockType(i) == BLOCK_GOLD)
		{
			CCSprite* gold = getBlockThing(i);
			reorderChild(gold, 11);
			gold->runAction(CCRepeatForever::create(CCBlink::create(0.8f, 1)));
		}
	}
}

CCSprite* BlockLayer::getBlockThing(int tag)
{
	CCSprite* item = (CCSprite*)getChildByTag(2000+tag);
	return item;
}

void BlockLayer::openAnimation(int tag)
{
	CCPoint pos = m_batchNode->getChildByTag(tag)->getPosition();

	/*
	CCParticleSystem* m_emitter;
	m_emitter = CCParticleFireworks::create();
	this->addChild(m_emitter, 10);
	m_emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage("pointNormal.png"));
	m_emitter->setPosition(pos);*/

	CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	CCArray *animFrames = CCArray::create();
	for(int i=1;i<4;i++)
	{
		char szName[20];
		sprintf(szName, "open%d.png", i);
		CCSpriteFrame* frame = cache->spriteFrameByName(szName);
        animFrames->addObject(frame);
	}	
	CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames,0.1f);
	CCAnimate* action = CCAnimate::create(animation);

	CCSprite* openSprite = CCSprite::create();
	addChild(openSprite, LAYERNUM_HILIT, 1000+tag);
	openSprite->setContentSize(CCSizeMake(m_unit,m_unit));
	openSprite->setAnchorPoint(ccp(0,0));
	openSprite->setPosition(pos);

	CCFiniteTimeAction* actions=CCSequence::create(action,
		CCCallFuncO::create(this, callfuncO_selector(BlockLayer::callBackRealOpen), openSprite),
		NULL); 
	openSprite->runAction(actions);
}

void BlockLayer::hilit(int tag)
{
	if(m_hilitBlock)
	{
		m_hilitBlock->removeFromParentAndCleanup(true);
		m_hilitBlock = NULL;
	}
	CCSprite* block = (CCSprite*)m_batchNode->getChildByTag(tag);
	if(!block)
		return;
	m_hilitBlock = CCSprite::create(PIC_BLOCK_HILIT);
	m_hilitBlock->setAnchorPoint(ccp(0,0));
	m_hilitBlock->setPosition(block->getPosition());
	addChild(m_hilitBlock, LAYERNUM_HILIT);
}
void BlockLayer::deHilit()
{
	if(m_hilitBlock)
	{
		m_hilitBlock->removeFromParentAndCleanup(true);
		m_hilitBlock = NULL;
	}
}

//---SpriteClassModel 按键-----------------
// 别忘记加入 TouchDispatcher  ,TouchDispatcher一般放在onenter 绑定，onexit 删除
bool BlockLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	if(game_process.state != GAME_STATE_NORMAL && game_process.state != GAME_STATE_PROP)
		return false;
	CCPoint touchPoint = touch->getLocation();
	//CCLOG("%s, (%f, %f)", "touch", touchPoint.x, touchPoint.y);
	int row = (int)touchPoint.y / m_unit;
	int col = (int)touchPoint.x / m_unit;
	m_touchTag = row*BLOCK_COL+col;
	if(game_process.state == GAME_STATE_PROP)
	{
		BLOCK_TYPE type = game_process.propCells[game_process.selectProp].type;
		if(type == BLOCK_BOMB || type == BLOCK_MAGNIFIER)
		{
			if(m_touchTag<0 || m_touchTag >= BLOCK_NUM)
				return false;
			propOnBlock(m_touchTag);
		}
		return false;
	}
	if(m_touchTag<0 || m_touchTag >= BLOCK_NUM || m_blockData->isBlockOpen(m_touchTag))
	{
		CCSprite* prop = getBlockThing(m_touchTag);
		if(prop)
			return true;
		else
			return false;
	}
	else
	{
		hilit(m_touchTag);
		return true;
	}
}

void BlockLayer::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
	if(game_process.state != GAME_STATE_NORMAL)
	{
		deHilit();
		return;
	}
	if(m_touchTag<0)
	{
		deHilit();
		return;
	}
	
	CCPoint touchPoint = touch->getLocation();
	int row = (int)touchPoint.y / m_unit;
	int col = (int)touchPoint.x / m_unit;
	m_touchTag = row*BLOCK_COL+col;
	
	if(m_touchTag<0 || m_touchTag >= BLOCK_NUM)
		m_touchTag = -1;
	else if(m_blockData->isBlockOpen(m_touchTag))
	{
		m_touchTag = -1;
	}
	else
		hilit(m_touchTag);
}

void BlockLayer::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	if(game_process.state != GAME_STATE_NORMAL)
		return;
	if(m_touchTag < 0)
		return;
	if(m_blockData->isBlockOpen(m_touchTag))
	{
		CCSprite* prop = getBlockThing(m_touchTag);
		if(prop)
		{
			BLOCK_TYPE type = m_blockData->getBlockType(m_touchTag);
			if(!s_blockContent[type].isCollect)
				return;
			PlayLayer* playLayer = (PlayLayer*)getParent();
			if(playLayer->isPropCellFull(type))
				MySound::playSound(SOUND_PROP_FULL);
			else
				MySound::playSound(SOUND_FIND_PROP);
			playLayer->getPropToPropPack(prop, type);
		}
		return;
	}
	deHilit();
	MySound::playSound(SOUND_OPEN);
	m_blockData->openBlock(m_touchTag);
	openAnimation(m_touchTag);
}

void BlockLayer::onEnter()
{   
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 100, false);
	CCLayer::onEnter();
}

void BlockLayer::onExit()
{   
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);  
	CCLayer::onExit();
}
