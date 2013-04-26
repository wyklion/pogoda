#include "FirstLayer.h"
#include "SceneManager.h"
#include "MySound.h"
#include "UserData.h"

bool FirstLayer::init()  
{  
	if (!CCLayer::init())
	{  
		return false;  
	}

	CCSize s = CCDirector::sharedDirector()->getWinSize();

	m_helpLayer = CCLayer::create();
	m_scroll = CCScrollView::create();
	m_scroll->setPosition(CCPointZero);
	m_scroll->setContentOffset(CCPointZero);
	m_scroll->setViewSize(CCSizeMake(640, 960));
	m_scroll->setContentSize(CCSizeMake(640*3, 960));
	m_scroll->setDirection(kCCScrollViewDirectionHorizontal);
	m_scroll->setContainer(m_helpLayer);
	m_scroll->setTouchEnabled(false);
	m_scroll->setDelegate(this);
	this->addChild(m_scroll);

	char buf[30];
	for (int i = 1; i <= 3; ++ i)
	{
		sprintf(buf,"first%d.png",i);
		CCSprite *sprite = CCSprite::create(buf);
		sprite->setPosition(ccp(s.width * (i-0.5f), s.height / 2));
		m_helpLayer->addChild(sprite);
	}
	
	CCSpriteFrameCache *pCache =  CCSpriteFrameCache::sharedSpriteFrameCache();
	pCache->addSpriteFrame(CCSpriteFrame::create(PIC_POINT_NORMAL,CCRectMake(0, 0, 15, 15)),PIC_POINT_NORMAL);
	pCache->addSpriteFrame(CCSpriteFrame::create(PIC_POINT_SELECTED,CCRectMake(0, 0, 15, 15)),PIC_POINT_SELECTED);
	for (int i = 1; i <= 3; ++ i)
	{
		CCSprite *pPoint = CCSprite::createWithSpriteFrameName(PIC_POINT_NORMAL);
		pPoint->setTag(i);
		pPoint->setPosition(ccp(s.width/2 + 30 * (i-2), 100));
		this->addChild(pPoint);
	}
	CCSprite *pPoint = (CCSprite *)this->getChildByTag(1);
	pPoint->setDisplayFrame(pCache->spriteFrameByName(PIC_POINT_SELECTED));

	m_nCurPage = 1;
	return true;
}

void FirstLayer::scrollViewDidScroll(cocos2d::extension::CCScrollView *view)
{
	//adjustScrollView();
	//CCLOG("scroll");
}

void FirstLayer::scrollViewDidZoom(cocos2d::extension::CCScrollView *view)
{
	//CCLOG("zoom");
}

void FirstLayer::onEnter()
{
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
}

void FirstLayer::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCLayer::onExit();
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName(PIC_POINT_NORMAL);
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName(PIC_POINT_SELECTED);
}


bool FirstLayer::ccTouchBegan(CCTouch *touch, CCEvent *pEvent)
{
	m_touchPoint = touch->getLocation();
	m_offsetPoint = m_scroll->getContentOffset();
	return true;
}

void FirstLayer::ccTouchMoved(CCTouch *touch, CCEvent *pEvent)
{
	CCPoint endPoint = touch->getLocation();
	float distance = endPoint.x - m_touchPoint.x;
	m_scroll->setContentOffset(CCPoint(m_offsetPoint.x+distance, 0));
}

void FirstLayer::ccTouchEnded(CCTouch *touch, CCEvent *pEvent)
{
	CCPoint endPoint = touch->getLocation();
	float distance = endPoint.x - m_touchPoint.x;
	if(fabs(distance) > 50)
		adjustScrollView(distance);
}

void FirstLayer::ccTouchCancelled(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent)
{
	CCPoint endPoint = touch->getLocation();
	float distance = endPoint.y - m_touchPoint.y;
	if(fabs(distance) > 50)
		adjustScrollView(distance);
}

void FirstLayer::adjustScrollView(float offset)
{
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	CCSpriteFrameCache *pCache =  CCSpriteFrameCache::sharedSpriteFrameCache();
	CCSprite *pPoint = (CCSprite *)this->getChildByTag(m_nCurPage);
	pPoint->setDisplayFrame(pCache->spriteFrameByName(PIC_POINT_NORMAL));
	if (offset<0)
	{
		m_nCurPage ++;
	}else
	{
		m_nCurPage --;
	}

	if (m_nCurPage <1)
	{
		m_nCurPage = 1;
	}

	if(m_nCurPage > 3)
	{
		game_process.firstPlay = false;
		CCSetBool("firstPlay", false);
		CCUserDefault::sharedUserDefault()->flush();
		SceneManager::goPlay();
	}
	else
	{
		pPoint = (CCSprite *)this->getChildByTag(m_nCurPage);
		pPoint->setDisplayFrame(pCache->spriteFrameByName(PIC_POINT_SELECTED));
		CCPoint adjustPos = ccp(-s.width * (m_nCurPage-1), 0);
		m_scroll->setContentOffset(adjustPos, true);
	}
}