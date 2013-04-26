#include "InfoLayer.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;

bool InfoLayer::init()  
{  
	if (!CCLayer::init())
	{  
		return false;  
	}
	
    CCSize s = CCDirector::sharedDirector()->getWinSize();

	//background...
	showBackGround();

	//level...
	CCSprite* showSprite = CCSprite::create(PIC_SHOW_LEVEL);
	showSprite->setAnchorPoint(ccp(0,0));
	//showSprite->setPosition(ccp(0,150));
	showSprite->setPosition(ccp(0,s.height-50));
	addChild(showSprite,0,1);
	
	char buf[10];
	sprintf(buf, "%d", game_process.level);
	m_moneyLabel = CCLabelTTF::create(buf, "Marker Felt", 50.0, CCSizeMake(130, 50), kCCTextAlignmentLeft);
	m_moneyLabel->setAnchorPoint(ccp(0, 0));
	//m_moneyLabel->setPosition(ccp(80, 150));
	m_moneyLabel->setPosition(ccp(70, s.height-50));
	addChild(m_moneyLabel);
	
	//step...
	showSprite = CCSprite::create(PIC_SHOW_STEP);
	showSprite->setAnchorPoint(ccp(0,0));
	//showSprite->setPosition(ccp(0,100));
	showSprite->setPosition(ccp(0, s.height-100));
	addChild(showSprite,0,2);
	
	//sprintf(buf, "%d", game_process.step);
	m_stepLabel = CCLabelTTF::create("", "Marker Felt", 50.0, CCSizeMake(130, 50), kCCTextAlignmentLeft);
	m_stepLabel->setAnchorPoint(ccp(0, 0));
	//m_stepLabel->setPosition(ccp(80, 100));
	m_stepLabel->setPosition(ccp(70, s.height-100));
	addChild(m_stepLabel);
	setStep(game_process.step);
	
	//money...
	showSprite = CCSprite::create(PIC_SHOW_MONEY);
	showSprite->setAnchorPoint(ccp(0,0));
	//showSprite->setPosition(ccp(0,50));
	showSprite->setPosition(ccp(0, s.height-150));
	addChild(showSprite,0,3);

	sprintf(buf, "%d", game_process.money);
	m_moneyLabel = CCLabelTTF::create(buf, "Marker Felt", 50.0, CCSizeMake(130, 50), kCCTextAlignmentLeft);
	m_moneyLabel->setAnchorPoint(ccp(0, 0));
	//m_moneyLabel->setPosition(ccp(80, 50));
	m_moneyLabel->setPosition(ccp(70, s.height-150));
	addChild(m_moneyLabel);
	
	//gold...
	showSprite = CCSprite::create(PIC_SHOW_GOLD);
	showSprite->setAnchorPoint(ccp(0,0));
	//showSprite->setPosition(ccp(0,0));
	showSprite->setPosition(ccp(0, s.height-200));
	addChild(showSprite,0,4);

	sprintf(buf, "%d", game_process.gold);
	m_goldLabel = CCLabelTTF::create(buf, "Marker Felt", 50.0, CCSizeMake(130, 50), kCCTextAlignmentLeft);
	m_goldLabel->setAnchorPoint(ccp(0, 0));
	//m_goldLabel->setPosition(ccp(80, 0));
	m_goldLabel->setPosition(ccp(70, s.height-200));
	addChild(m_goldLabel);	

	return true;
}  

InfoLayer::~InfoLayer()
{
}

void InfoLayer::showBackGround()
{	
    CCSize s = CCDirector::sharedDirector()->getWinSize();
	CCScale9Sprite* bg = CCScale9Sprite::create(PIC_ROUND);
	bg->setPreferredSize(CCSizeMake(220, 210));
	bg->setColor(ccc3(255,128,0));
	bg->setPosition(ccp(100, s.height-100));
	addChild(bg);
}

void InfoLayer::setStep(int step)
{
	char buf[10];
	sprintf(buf, "%d", step);
	m_stepLabel->setString(buf);
	CCSprite* showSprite = (CCSprite*)getChildByTag(2);
	if(step>0 && step<=3)
	{
		//CCActionInterval* act1 = CCTintBy::create(1.0,128,0,0);
		//CCActionInterval* act2 = CCTintBy::create(1.0,-128,0,0);
		//CCActionInterval* act3 = (CCActionInterval*)CCSequence::create(act1, act2, NULL))
		showSprite->runAction(CCRepeatForever::create(CCBlink::create(1.0, 1)));
	}
	else if(showSprite->isRunning())
	{
		showSprite->removeFromParentAndCleanup(true);
		showSprite = CCSprite::create(PIC_SHOW_STEP);
		showSprite->setAnchorPoint(ccp(0,0));
		CCSize s = CCDirector::sharedDirector()->getWinSize();
		showSprite->setPosition(ccp(0,s.height-100));
		addChild(showSprite,0,2);
	}
}

void InfoLayer::setMoney(int money)
{
	char buf[10];
	sprintf(buf, "%d", money);
	m_moneyLabel->setString(buf);
}

void InfoLayer::setGold(int gold)
{
	char buf[10];
	sprintf(buf, "%d", gold);
	m_goldLabel->setString(buf);
}

void InfoLayer::updateInfo()
{
	if(game_process.step<=0)
		game_process.step = 0;
	setStep(game_process.step);
	setMoney(game_process.money);
	setGold(game_process.gold);
}