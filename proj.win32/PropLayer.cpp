#include "PropLayer.h"
#include "basic.h"
#include "MySound.h"
#include "PlayLayer.h"


PropLayer::~PropLayer(void)
{
}

bool PropLayer::init()
{
	if (!CCLayer::init())  
	{  
		return false;  
	}
	
	CCSpriteBatchNode* batchNode = CCSpriteBatchNode::create(PIC_PROP_BACKGROUND, PROP_NUM);	
	batchNode->setAnchorPoint(ccp(0,0));
	batchNode->setPosition(0,0);
	this->addChild(batchNode);
	for(int i = 0; i < PROP_NUM; i++)
	{
		CCSprite* propBackground = CCSprite::create(PIC_PROP_BACKGROUND);
		propBackground->setAnchorPoint(ccp(0,0));
		propBackground->setPosition(ccp(BLOCK_UNIT*i, 0));
		batchNode->addChild(propBackground, 0, i);
	}
	m_selectTag = -1;

	updatePropCells();
	return true;
}

int PropLayer::getCellNum(BLOCK_TYPE type)
{
	for(int i = 0; i < PROP_NUM; i++)
	{
		if(game_process.propCells[i].num == 0 || game_process.propCells[i].type == type)
			return i;
	}
	return -1;
}

bool PropLayer::addProp(BLOCK_TYPE type)
{
	int cellNum = getCellNum(type);
	if(cellNum<0)
		return false;

	if(game_process.propCells[cellNum].num == 0)
	{
		game_process.propCells[cellNum].num++;
		game_process.propCells[cellNum].type = type;
	}
	else
	{
		game_process.propCells[cellNum].num++;
	}
	return true;
}

void PropLayer::movePropToFront(int index)
{
	CCSprite* item = NULL;
	for(int i = index; i < PROP_NUM - 1; i++)
	{
		game_process.propCells[i].num = game_process.propCells[i+1].num;
		game_process.propCells[i].type = game_process.propCells[i+1].type;
		item = (CCSprite*)getChildByTag(1000+i+1);
		if(item)
		{
			item->setTag(1000+i);
			item->setPosition(ccp(item->getPositionX()-BLOCK_UNIT, item->getPositionY()));
		}
		item = (CCSprite*)getChildByTag(2000+i+1);
		if(item)
		{
			item->setTag(2000+i);
			item->setPosition(ccp(item->getPositionX()-BLOCK_UNIT, item->getPositionY()));
		}
	}
	game_process.propCells[PROP_NUM-1].num = 0;
}

void PropLayer::updatePropCells()
{
	char* pic_name = NULL;
	for(int i = 0; i < PROP_NUM; i++)
	{
		CCSprite* item = NULL;
		if(game_process.propCells[i].num == 0)
		{
			item = (CCSprite*)getChildByTag(1000+i);//prop sprite...
			if(item)
				item->removeFromParentAndCleanup(true);
			item = (CCSprite*)getChildByTag(2000+i);//prop number sprite...
			if(item)
				item->removeFromParentAndCleanup(true);
			if(i < PROP_NUM - 1)
			{
				movePropToFront(i);
				continue;
			}
			else
				break;
		}
		item = (CCSprite*)getChildByTag(1000+i);
		if(!item)
		{
			item = CCSprite::create(s_blockContent[game_process.propCells[i].type].picName.c_str());
			item->setAnchorPoint(ccp(0,0));
			item->setPosition(ccp(BLOCK_UNIT*i, 0));
			addChild(item, 1, 1000+i);
		}
		else
		{
			item->setOpacity(255);
		}
		if(game_process.propCells[i].num == 1)
		{
			CCSprite* item = (CCSprite*)getChildByTag(2000+i);
			if(item)
				item->removeFromParentAndCleanup(true);
		}
		else if(game_process.propCells[i].num>1)
		{
			CCNode* oldLabel = getChildByTag(2000+i);
			if(oldLabel)
				oldLabel->removeFromParentAndCleanup(true);
			char buf[5];
			sprintf(buf, "%d", game_process.propCells[i].num);
			CCLabelTTF* label = CCLabelTTF::create(buf, "Marker Felt", 40.0f, CCSizeMake(BLOCK_UNIT/2, BLOCK_UNIT/2), kCCTextAlignmentCenter);
			label->setAnchorPoint(ccp(0, 0));
			label->setPosition(ccp(BLOCK_UNIT*(i+0.5), BLOCK_UNIT*0.5));
			addChild(label, 2, 2000+i);
		}
	}
	if(game_process.selectProp>=0)
	{
		CCSprite* prop = getProp(game_process.selectProp);
		prop->setOpacity(125);
		//selectProp(game_process.selectProp);
	}
}

CCSprite* PropLayer::getProp(int tag)
{
	if(game_process.propCells[tag].num == 0)
		return NULL;
	CCSprite* prop = (CCSprite*)getChildByTag(1000+tag);
	return prop;
}

void PropLayer::unselectProp()
{
	if(game_process.selectProp == -1)
		return;
	CCSprite* prop = getProp(game_process.selectProp);
	prop->setOpacity(255);
	game_process.selectProp = -1;
	game_process.state = GAME_STATE_NORMAL;
}

void PropLayer::selectProp(int tag)
{
	CCSprite* prop = getProp(tag);
	if(!prop)
		return;
	PlayLayer* playLayer = (PlayLayer*)getParent();
	playLayer->clearPropMenu();
	if(game_process.selectProp>=0)
	{
		if(tag == game_process.selectProp)
		{
			prop->setOpacity(255);
			game_process.selectProp = -1;
			game_process.state = GAME_STATE_NORMAL;
			MySound::playSound(SOUND_DROP_PROP);
			return;
		}
		else
			unselectProp();
	}
	prop->setOpacity(125);
	game_process.selectProp = tag;
	game_process.state = GAME_STATE_PROP;
	MySound::playSound(SOUND_SELECT_PROP);

	playLayer->checkProp(game_process.propCells[tag].type);
}

bool PropLayer::getTouchPointTag(const CCPoint& point, int& col)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
	if(point.x<240 || point.y<s.height-BLOCK_UNIT)
		return false;
	int row = (int)(point.y-s.height+BLOCK_UNIT) / BLOCK_UNIT;
	col = (int)(point.x-240) / BLOCK_UNIT;
	if(row != 0 || col < 0 || col > PROP_NUM-1)
		return false;
	return true;
}

//---SpriteClassModel 按键-----------------
// 别忘记加入 TouchDispatcher  ,TouchDispatcher一般放在onenter 绑定，onexit 删除
bool PropLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	if(game_process.state != GAME_STATE_NORMAL /*&& game_process.state != GAME_STATE_PROP*/)
		return false;
	CCPoint touchPoint = touch->getLocation();
	//CCLOG("%s, (%f, %f)", "touch", touchPoint.x, touchPoint.y);

	int col = 0;
	if(getTouchPointTag(touchPoint, col))
		return true;
	return false;
}

void PropLayer::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
}

void PropLayer::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	CCPoint touchPoint = touch->getLocation();
	int col = 0;
	if(!getTouchPointTag(touchPoint, col))
		return;
	selectProp(col);
}

void PropLayer::onEnter()
{   
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 100, false);
	CCLayer::onEnter();
}

void PropLayer::onExit()
{   
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);  
	CCLayer::onExit();
}
