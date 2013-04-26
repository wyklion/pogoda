#include "SkillLayer.h"
#include "SceneManager.h"
#include "MySound.h"

bool SkillLayer::init()  
{  
	if (!CCLayerColor::initWithColor(ccc4(107,142,35,255)))
	{  
		return false;  
	}
	
	UserData::load();

	createThings();

	//Back Button...
    CCSize s = CCDirector::sharedDirector()->getWinSize();
	CCMenuItemImage* back = CCMenuItemImage::create(PIC_BUTTON_BACK, PIC_BUTTON_BACK_SELECTED, this, menu_selector(SkillLayer::back));
	back->setScale(0.7f);
	CCMenu* menu = CCMenu::create(back, NULL);
	menu->setPosition(ccp(70, s.height-30));

	this->addChild(menu);  
	m_expanding = -1;
	return true;
}

void SkillLayer::createThings()
{	
	//User Money...
    CCSize s = CCDirector::sharedDirector()->getWinSize();
	CCScale9Sprite* bg = CCScale9Sprite::create(PIC_ROUND);
	bg->setPreferredSize(CCSizeMake(640, 60));
	bg->setAnchorPoint(ccp(0,0));
	bg->setColor(ccc3(50,205,50));
	bg->setPosition(ccp(0, s.height-60));
	addChild(bg);

	CCLabelTTF* label = CCLabelTTF::create("You have:", "Marker Felt", 30.0/*, CCSizeMake(200, 50), kCCTextAlignmentLeft*/);
	label->setAnchorPoint(ccp(0, 0.5));
	label->setPosition(ccp(s.width-300, s.height-30));
	label->setColor(ccc3(0,0,0));
	addChild(label);
	
	char buf[10];
	sprintf(buf, "%d", game_process.money);
	label = CCLabelTTF::create(buf, "Marker Felt", 30.0/*, CCSizeMake(150, 50), kCCTextAlignmentLeft*/);
	label->setAnchorPoint(ccp(1, 0.5));
	label->setPosition(ccp(s.width-60, s.height-30));
	label->setColor(ccc3(255,255,0));
	addChild(label,0,8888);
	CCSprite* moneyShow = CCSprite::create(PIC_SHOW_MONEY);
	moneyShow->setScale(0.6f);
	moneyShow->setPosition(ccp(s.width-30, s.height-30));
	addChild(moneyShow);
	
	//scrollview...
	m_itemScroll = CCScrollView::create();
	m_itemLayer = CCLayer::create();
	m_itemScroll->setPosition(CCPointZero);
	m_itemScroll->setContentOffset(CCPointZero);
	m_itemScroll->setViewSize(CCSizeMake(640, 900));
	m_itemScroll->setContentSize(CCSizeMake(640, 960));
	m_itemScroll->setDirection(kCCScrollViewDirectionVertical);
    m_itemScroll->setContainer(m_itemLayer);
	m_itemScroll->setTouchEnabled(false);
	m_itemScroll->setDelegate(this);
	this->addChild(m_itemScroll);
	
	int top = s.height-50;
	for(int i = 0; i < (int)SKILL_NUM; i++)
	{
		m_items[i] = CCNode::create();
		//background...
		CCScale9Sprite* bg = CCScale9Sprite::create(PIC_ROUND);
		bg->setPreferredSize(CCSizeMake(640, 90));
		bg->setAnchorPoint(ccp(0,0.5));
		bg->setColor(ccc3(0,199,140));
		bg->setPosition(ccp(0, top - (i+1)*100));
		m_items[i]->addChild(bg,0,1);

		//skill type...
		Skill_Type type = (Skill_Type)i;
		
		//skill pic...
		CCSprite* pic = CCSprite::create(game_skill.getPicName(type));
		pic->setPosition(ccp(40, top - (i+1)*100));
		m_items[i]->addChild(pic,0,2);

		//skill name...
		CCLabelTTF* label = CCLabelTTF::create(game_skill.getName(type), "Marker Felt", 30.0/*, CCSizeMake(300, 50), kCCTextAlignmentLeft*/);
		label->setAnchorPoint(ccp(0, 0.5));
		label->setColor(ccc3(0,0,0));
		label->setPosition(ccp(80, top - (i+1)*100));
		m_items[i]->addChild(label,0,3);
		
		//skill level...
		char buf[15];
		sprintf(buf, "Lv.%d", game_skill.skillLevel[type]);
		label = CCLabelTTF::create(buf, "Marker Felt", 30.0/*, CCSizeMake(150, 50), kCCTextAlignmentLeft*/);
		label->setAnchorPoint(ccp(0, 0.5));
		label->setColor(ccc3(210,105,30));
		label->setPosition(ccp(360, top - (i+1)*100));
		m_items[i]->addChild(label,0,4);	
		
		//skill describe...
		label = CCLabelTTF::create(game_skill.getDescribe(type), "Marker Felt", 25.0/*, CCSizeMake(150, 50), kCCTextAlignmentLeft*/);
		label->setAnchorPoint(ccp(0, 1));
		label->setColor(ccc3(0,0,0));
		label->setPosition(ccp(80, top - (i+1)*100-40));
		label->setVisible(false);
		m_items[i]->addChild(label,0,7);	

		//pay money... require found...
		CCNode* requireNode = CCNode::create();
		m_items[i]->addChild(requireNode,0,11);
		bool enable = true;
		if(game_skill.skillLevel[type] == game_skill.getMaxLevel(type))
		{
			sprintf(buf, "");
			enable = false;
		}
		else
		{
			int pay = game_skill.getPay(type);
			sprintf(buf, "%d", pay);
			if(game_process.money < pay)
				enable = false;
			BLOCK_TYPE requireType = s_skill_table[type].requireFound;
			if(requireType != BLOCK_NONE && !game_process.isBlockFound[requireType])
			{
				label = CCLabelTTF::create("Require found", "Marker Felt", 20.0);
				label->setPosition(ccp(450, top - (i+2)*100));
				label->setAnchorPoint(ccp(1, 0.5));
				label->setColor(ccc3(210,105,30));
				requireNode->addChild(label);
				CCSprite* blockPic = CCSprite::create(s_blockContent[requireType].picName.c_str());
				blockPic->setPosition(ccp(480, top - (i+2)*100));
				blockPic->setScale(0.3f);
				requireNode->addChild(blockPic);
				requireNode->setVisible(false);
				enable = false;
			}
		}
		label = CCLabelTTF::create(buf, "Marker Felt", 30.0);
		label->setPosition(ccp(s.width-50, top - (i+1)*100));
		label->setAnchorPoint(ccp(1, 0.5));
		label->setColor(ccc3(210,105,30));
		m_items[i]->addChild(label,0,5);
		CCSprite* payPic = CCSprite::create(PIC_SHOW_MONEY);
		payPic->setScale(0.6f);
		payPic->setPosition(ccp(s.width-30, top - (i+1)*100));
		m_items[i]->addChild(payPic,0,6);

		//skill pay for update level...
		CCMenuItemFont* buy = CCMenuItemFont::create("BUY", this, menu_selector(SkillLayer::buySkill));
		buy->setFontSize(30);
		buy->setTag(100+i);
		buy->setAnchorPoint(ccp(0, 0.5));
		CCMenu* buyMenu = CCMenu::create(buy, NULL);
		//buyMenu->setAnchorPoint(ccp(0, 1));
		buyMenu->setPosition(ccp(550, top - (i+2)*100));
		if(enable)
		{
			buyMenu->setColor(ccc3(255,0,0));
		}
		else
		{
			buyMenu->setColor(ccc3(96,96,96));
			buyMenu->setEnabled(false);
		}
		buyMenu->setVisible(false);
		m_items[i]->addChild(buyMenu,0,10);
		m_itemLayer->addChild(m_items[i]);
	}
}

void SkillLayer::updateSkill()
{
	//update money...
	CCLabelTTF* label = (CCLabelTTF*)getChildByTag(8888);
	char buf[10];
	sprintf(buf, "%d", game_process.money);
	label->setString(buf);
	
	//update skill items...//update skill items...
    CCSize s = CCDirector::sharedDirector()->getWinSize();
	int top = s.height-50;
	for(int i = 0; i < (int)SKILL_NUM; i++)
	{
		//skill type...
		Skill_Type type = (Skill_Type)i;

		//skill level...
		char buf[15];
		sprintf(buf, "Lv.%d", game_skill.skillLevel[type]);
		CCLabelTTF* label = (CCLabelTTF*)m_items[i]->getChildByTag(4);
		label->setString(buf);
		
		//pay money...
		label = (CCLabelTTF*)m_items[i]->getChildByTag(5);
		CCSprite* payPic = (CCLabelTTF*)m_items[i]->getChildByTag(6);
		bool enable = true;
		if(game_skill.skillLevel[type] == game_skill.getMaxLevel(type))
		{
			sprintf(buf, "");
			payPic->setVisible(false);
			enable = false;
		}
		else
		{
			int pay = game_skill.getPay(type);
			sprintf(buf, "%d", pay);
			if(game_process.money < pay)
				enable = false;
		}
		label->setString(buf);

		//buy menu...
		CCMenu* buyMenu = (CCMenu*)m_items[i]->getChildByTag(10);
		if(enable)
		{
			buyMenu->setColor(ccc3(255,0,0));
		}
		else
		{
			buyMenu->setColor(ccc3(96,96,96));
			buyMenu->setEnabled(false);
		}
	}
}

void SkillLayer::back(CCObject *pSender)  
{
	MySound::playSound(SOUND_SELECT_PROP);
	SceneManager::goMenu(0);//2 -> left
}

void SkillLayer::buySkill(CCObject *pSender)
{
	MySound::playSound(SOUND_UPGRADE);
	CCNode* node = (CCNode*)pSender;
	Skill_Type type = (Skill_Type)(node->getTag()-100);
	game_process.money -= game_skill.getPay(type);
	game_skill.skillLevel[type]++;
	if(game_skill.skillLevel[type]>game_skill.getMaxLevel(type))
		game_skill.skillLevel[type] = game_skill.getMaxLevel(type);
	UserData::save();
	updateSkill();
}

void SkillLayer::fold(int index)
{
	CCScale9Sprite* bg = (CCScale9Sprite*)m_items[index]->getChildByTag(1);
	bg->setScaleY(1.0f);
	bg->setPositionY(bg->getPositionY()+50);
	((CCMenu*)m_items[index]->getChildByTag(10))->setVisible(false);
	((CCLabelTTF*)m_items[index]->getChildByTag(7))->setVisible(false);
	((CCNode*)m_items[index]->getChildByTag(11))->setVisible(false);

}

void SkillLayer::unfold(int index)
{
	CCScale9Sprite* bg = (CCScale9Sprite*)m_items[index]->getChildByTag(1);
	bg->setScaleY(2.0f);
	bg->setPositionY(bg->getPositionY()-50);
	((CCMenu*)m_items[index]->getChildByTag(10))->setVisible(true);
	((CCLabelTTF*)m_items[index]->getChildByTag(7))->setVisible(true);
	((CCNode*)m_items[index]->getChildByTag(11))->setVisible(true);
	float y = bg->getPositionY();
	CCPoint p = m_itemScroll->getContentOffset();
	float offset = y + p.y - 760;
	/*CCLog(",,,%f", y);
	CCLog("touch %f", p.y);
	CCLog("%f",offset);*/
	if(offset>0)
		m_itemScroll->setContentOffset(ccp(0,760-y), true);
}

void SkillLayer::expand(int index)
{
	CCScale9Sprite* bg = NULL;
	if(m_expanding == -1)
	{
		unfold(index);

		for(int i = index+1; i < (int)SKILL_NUM; i++)
		{
			float y = m_items[i]->getPositionY();
			m_items[i]->setPositionY(y-100);
		}
		m_expanding = index;
	}
	else if(m_expanding == index)
	{
		fold(index);

		for(int i = index + 1; i < (int)SKILL_NUM; i++)
		{
			m_items[i]->setPositionY(m_items[i]->getPositionY()+100);
		}
		m_expanding = -1;
	}
	else
	{
		//old expand...
		fold(m_expanding);
		//new expand...
		unfold(index);
		
		//other...
		if(m_expanding < index)
		{
			for(int i = m_expanding + 1; i <= index; i++)
			{
				m_items[i]->setPositionY(m_items[i]->getPositionY()+100);
			}
		}
		else if(m_expanding > index)
		{
			for(int i = index+1; i < m_expanding+1; i++)
			{
				float y = m_items[i]->getPositionY();
				m_items[i]->setPositionY(y-100);
			}
		}
		m_expanding = index;
	}
}

void SkillLayer::scrollViewDidScroll(cocos2d::extension::CCScrollView *view)
{
	//adjustScrollView();
	//CCLOG("scroll");
}

void SkillLayer::scrollViewDidZoom(cocos2d::extension::CCScrollView *view)
{
	//CCLOG("zoom");
}

void SkillLayer::onEnter()
{
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
}

void SkillLayer::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCLayer::onExit();
}


bool SkillLayer::ccTouchBegan(CCTouch *touch, CCEvent *pEvent)
{
	m_touchPoint = touch->getLocation();
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	if(m_touchPoint.y>s.height-60)
		return false;
	m_offsetPoint = m_itemScroll->getContentOffset();
	m_isScroll = false;
	return true;
}

void SkillLayer::ccTouchMoved(CCTouch *touch, CCEvent *pEvent)
{
	CCPoint endPoint = touch->getLocation();
	float distance = endPoint.y - m_touchPoint.y;
	if(fabs(distance) > 20)
		m_isScroll = true;
	if(m_isScroll)
	{

		m_itemScroll->setContentOffset(CCPoint(0, m_offsetPoint.y + distance));
	}
}

void SkillLayer::ccTouchEnded(CCTouch *touch, CCEvent *pEvent)
{
	CCPoint endPoint = touch->getLocation();
	if(!m_isScroll)
	{
		CCPoint p = m_itemScroll->getContentOffset();
		float touchOffset = m_touchPoint.y - p.y;
		//CCLog("touch %f", touchOffset);
		CCSize s = CCDirector::sharedDirector()->getWinSize();
		int topOffset = s.height-100 - touchOffset;
		if(topOffset < 0)
			return;
		int index = topOffset/100;
		if(m_expanding != -1)
		{
			if(m_expanding < index)
				index--;
		}
		if(index < (int)SKILL_NUM)
			expand(index);
		return;
	}
	adjustScrollView();
}

void SkillLayer::ccTouchCancelled(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent)
{
	//CCPoint endPoint = touch->getLocation();
	//float distance = endPoint.y - m_touchPoint.y;
	adjustScrollView();
}

void SkillLayer::adjustScrollView()
{
	CCPoint p = m_itemScroll->getContentOffset();
	if(p.y < 0)
		m_itemScroll->setContentOffset(ccp(0,0), true);
	else if(p.y > 300)
		m_itemScroll->setContentOffset(ccp(0,300), true);
}


//skill pay fuc...
int SkillPayAndValue::getPay1(int level)
{
	int pay = 100;
	for(int i = 0; i < level; i++)
		pay*=2;
	return pay;;
}
int SkillPayAndValue::getPayForBigBomb(int level)
{
	return 5000;
}

//skill get value...
int SkillPayAndValue::getValueForOriginalStep(int level)
{
	return 10 + level*2;
}
int SkillPayAndValue::getValueMoreMoney(int level)
{
	return level;
}
int SkillPayAndValue::getValueMoreEye(int level)
{
	return 3+level;
}
