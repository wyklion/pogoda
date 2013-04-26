#include "FoundLayer.h"
#include "UserData.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;


bool FoundLayer::init(BLOCK_TYPE type)
{
	if (!CCLayer::init())  
	{  
		return false;  
	}
	
    m_size = CCDirector::sharedDirector()->getWinSize();

	m_allShow = false;
	m_node = CCNode::create();
	addChild(m_node);
	
	//background...
	CCScale9Sprite* bg = CCScale9Sprite::create(PIC_ROUND);
	bg->setPreferredSize(CCSizeMake(400, 400));
	bg->setAnchorPoint(ccp(0.5,0.5));
	bg->setOpacity(200);
	bg->setColor(ccc3(160,82,45));
	bg->setPosition(ccp(m_size.width*0.5, m_size.height*0.5));
	m_node->addChild(bg);

	if(type != BLOCK_NUMBER)
	{
		//you found...
		CCLabelTTF* label = CCLabelTTF::create("Found", "Marker Felt", 50.0/*, CCSizeMake(200, 50), kCCTextAlignmentLeft*/);
		label->setAnchorPoint(ccp(1, 0.5));
		label->setPosition(ccp(m_size.width*0.5-110, m_size.height*0.5+100));
		label->setColor(ccc3(255,215,0));
		m_node->addChild(label);
		label->runAction(CCMoveTo::create(0.2f,ccp(m_size.width*0.5-10, m_size.height*0.5+100)));

		//prop pic...
		CCSprite* item = CCSprite::create(s_blockContent[type].picName.c_str());
		//item->setScale(0.625f);
		item->setAnchorPoint(ccp(0,0.5));
		item->setPosition(ccp(m_size.width*0.5+110, m_size.height*0.5+100));
		m_node->addChild(item);
		item->runAction(CCMoveTo::create(0.2f,ccp(m_size.width*0.5+10, m_size.height*0.5+100)));
	}
	else
	{
		CCLabelTTF* label = CCLabelTTF::create("Found a number", "Marker Felt", 50.0/*, CCSizeMake(200, 50), kCCTextAlignmentLeft*/);
		label->setAnchorPoint(ccp(0.5, 0.5));
		label->setPosition(ccp(m_size.width*0.5, m_size.height*0.5+200));
		label->setColor(ccc3(255,215,0));
		m_node->addChild(label);
		label->runAction(CCMoveTo::create(0.2f,ccp(m_size.width*0.5, m_size.height*0.5+100)));
	}

	//describe...
	char buf[200];
	sprintf(buf, s_blockContent[type].describe.c_str());
	CCLabelTTF* label = CCLabelTTF::create(buf, "Marker Felt", 30.0/*, CCSizeMake(200, 50), kCCTextAlignmentLeft*/);
	label->setAnchorPoint(ccp(0.5, 0.5));
	label->setPosition(ccp(m_size.width*0.5-10, m_size.height*0.5-50));
	label->setColor(ccc3(0,0,0));
	label->setOpacity(0);
	label->runAction(CCSequence::create(CCDelayTime::create(0.2f),
		CCFadeIn::create(1.0f), 
		CCCallFuncN::create(this, callfuncN_selector(FoundLayer::done)), 
		NULL));
	addChild(label);

	return true;
}

void FoundLayer::close()
{	
	runAction(CCSequence::create(CCFadeOut::create(0.1f),
		CCCallFuncN::create(this, callfuncN_selector(FoundLayer::remove)),
		NULL));
}

void FoundLayer::remove(CCNode* obj)
{
	game_process.state = GAME_STATE_NORMAL;
	CCLog("close found...");
	removeFromParentAndCleanup(true);
}
void FoundLayer::done(CCNode* obj)
{
	m_allShow = true;
}