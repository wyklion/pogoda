#include "GameOverLayer.h"
#include "SceneManager.h"
#include "MySound.h"
#include "UserData.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;

GameOverLayer::~GameOverLayer(void)
{
}

bool GameOverLayer::initWithData(const GameProcess& data)
{
	if (!CCLayerColor::initWithColor(ccc4(0,0,0,180)))  
	{  
		return false;  
	}  
	// elastic effect
    CCSize s = CCDirector::sharedDirector()->getWinSize();
	
    this->setTouchEnabled(true);
	
	//background...
	CCScale9Sprite* bg = CCScale9Sprite::create(PIC_ROUND);
	bg->setPreferredSize(CCSizeMake(480,640));
	bg->setColor(ccc3(153,51,250));
	bg->setOpacity(128);
	bg->setPosition(ccp(s.width*0.5, s.height*0.5));
	addChild(bg);
	
	char buf[50];
	//game info...
	//all open times...
	sprintf(buf, "Open times: %d", game_process.openTimes);
	CCLabelTTF* label = CCLabelTTF::create(buf, "Marker Felt", 30.0/*, CCSizeMake(200, 50), kCCTextAlignmentLeft*/);
	label->setAnchorPoint(ccp(0, 0.5));
	label->setPosition(ccp(s.width*0.5-120, s.height*0.5 + 200));
	label->setColor(ccc3(0,0,0));
	addChild(label);
	//level money...
	sprintf(buf, " Get money: %d", game_process.getMoney);
	label = CCLabelTTF::create(buf, "Marker Felt", 30.0/*, CCSizeMake(200, 50), kCCTextAlignmentLeft*/);
	label->setAnchorPoint(ccp(0, 0.5));
	label->setPosition(ccp(s.width*0.5-120, s.height*0.5+150));
	label->setColor(ccc3(0,0,0));
	addChild(label);

	//lottery....
	int startWidth = s.width*0.5 - 100;
	for(int i = 0; i < 3; i++)
	{
		CCMenuItemImage* lottery = CCMenuItemImage::create(PIC_LOTTERY,PIC_LOTTERY, this, menu_selector(GameOverLayer::menuLottery));
		lottery->setTag(100+i);
		CCMenu* menuLottery = CCMenu::create(lottery, NULL);
		menuLottery->setPosition(ccp(startWidth+i*100, s.height*0.4));
		menuLottery->setTag(200+i);
		addChild(menuLottery,1);
	}

	return true;
}

void GameOverLayer::quit(CCObject* pSender)
{
	UserData::saveProcess();
	MySound::stopSound();
	this->removeFromParentAndCleanup(false);
	SceneManager::goMenu();  
}

void GameOverLayer::menuLottery(CCObject* pSender)
{
	CCMenuItemImage* lottery = (CCMenuItemImage*)pSender;
	int index = lottery->getTag()-100;
    CCSize s = CCDirector::sharedDirector()->getWinSize();
	int startWidth = s.width*0.5 - 100;
	CCPoint p = ccp(startWidth+index*100, s.height*0.4);
	
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	CCArray* animFrames = CCArray::create();
	for(int i=1;i<4;i++)
	{
		char szName[20];
		sprintf(szName, "open%d.png", i);
		CCSpriteFrame* frame = cache->spriteFrameByName(szName);
        animFrames->addObject(frame);
	}	
	CCAnimation* animation = CCAnimation::createWithSpriteFrames(animFrames,0.1f);
	CCAnimate* action = CCAnimate::create(animation);

	CCSprite* openSprite = CCSprite::create();
	openSprite->setContentSize(CCSizeMake(BLOCK_UNIT,BLOCK_UNIT));
	openSprite->setPosition(p);
	openSprite->setTag(300+index);
	addChild(openSprite,2);

	CCFiniteTimeAction* actions=CCSequence::create(action,
		CCCallFuncN::create(this, callfuncN_selector(GameOverLayer::callBackLottery)),
		NULL); 
	openSprite->runAction(actions);

	MySound::playSound(SOUND_MONEY);
	//lottery->removeFromParentAndCleanup(false);
}

void GameOverLayer::callBackRemove(CCNode* node)
{
	node->removeFromParentAndCleanup(true);
}

void GameOverLayer::callBackLottery(CCNode* node)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
	int startWidth = s.width*0.5 - 100;

	int index = node->getTag() - 300;
	CCMenu* lottery = (CCMenu*)getChildByTag(200+index);
	if(!lottery)
		return;
	lottery->removeFromParentAndCleanup(true);
	lottery = NULL;
	node->removeFromParentAndCleanup(true);

	//3 random lottery
	int a[LOTTERY_NUM];
	for(int i = 0; i < (int)LOTTERY_NUM; i++)
	{
		a[i] = i;
	}
	int lotteryAll = (int)LOTTERY_NUM;
	for(int i = 0; i<3; i++)
	{
		int randLottery = i + rand() % lotteryAll;
		int tmp = a[i];
		a[i] = a[randLottery];
		a[randLottery] = tmp;
		lotteryAll--;
	}

	//treasure show...
	if((Lottery_Type)a[index] == LOTTERY_TREASURE)
	{
		PlayLayer* playLayer = (PlayLayer*)getParent();
		int m = rand()%6+10;
		game_process.money+=m;
		playLayer->updateInfo();
		BlockLayer::showGetting(this, ccp(startWidth+index*100-BLOCK_UNIT/2, s.height*0.4-BLOCK_UNIT/2), PIC_SHOW_MONEY, m);
	}
	
	//selected lottery...
	CCSprite* realLottery = CCSprite::create(s_lottery_pic[a[index]]);
	addChild(realLottery);
	realLottery->setPosition(ccp(startWidth+index*100, s.height*0.4));
	realLottery->runAction(CCSequence::create(CCFadeIn::create(1.0f), CCBlink::create(2.0f, 3), NULL));
	game_process.lottery = (Lottery_Type)a[index];

	//other lottery...
	for(int i = 0; i < 3; i++)
	{
		if(i == index)
			continue;
		CCMenu* lottery = (CCMenu*)getChildByTag(200+i);
		lottery->removeFromParentAndCleanup(true);
		CCSprite* fakelottery = CCSprite::create(PIC_LOTTERY);
		fakelottery->setPosition(ccp(startWidth+i*100, s.height*0.4));
		addChild(fakelottery);
		fakelottery->runAction(CCFadeOut::create(1.0f));

		CCSprite* otherLottery = CCSprite::create(s_lottery_pic[a[i]]);
		otherLottery->setPosition(ccp(startWidth+i*100, s.height*04));
		addChild(otherLottery);
	}
	
	//quit menu
	CCMenuItemImage* quit = CCMenuItemImage::create(
            PIC_QUIT,
            PIC_QUIT_SELECTED,
            this,
            menu_selector(GameOverLayer::quit));
	CCMenu* menu = CCMenu::create(quit, NULL);
	menu->setPosition(ccp(s.width*0.5, 0));
	menu->alignItemsVertically();
	this->addChild(menu);
	menu->runAction(CCMoveTo::create(0.5f, ccp(s.width*0.5, s.height*0.3)));
}