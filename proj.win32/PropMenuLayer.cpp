#include "PropMenuLayer.h"
#include "PlayLayer.h"

PropMenuLayer::~PropMenuLayer(void)
{
}

bool PropMenuLayer::init()
{
	if (!CCLayer::init())  
	{  
		return false;  
	}

	m_menuPropDo = NULL;
	m_menuPropCancel = NULL;
	m_menuPropSell = NULL;
	m_infoBack = NULL;

	return true;
}

void PropMenuLayer::set(BLOCK_TYPE type)
{
	showInfoBack(type);

	switch(type)
	{
	case BLOCK_BOMB:
		{
			showPropOption(PROP_OPTION_CANCEL);
			break;
		}
	case BLOCK_MAGNIFIER:
		{
			showPropOption(PROP_OPTION_CANCEL);
			break;
		}
	case BLOCK_FLYSHOE:
	case BLOCK_FLASHLIGHT:
	case BLOCK_SWEEP:
	case BLOCK_EYE:
		{
			showPropOption(PROP_OPTION_DO_CANCEL);
			break;
		}
	default:
		break;
	}
}

void PropMenuLayer::showInfoBack(BLOCK_TYPE type)
{
	int widthStart = 200;
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	if(m_infoBack)
		m_infoBack->removeAllChildrenWithCleanup(true);
	
	m_infoBack = CCNode::create();
	CCSprite* background = CCSprite::create("propInfoBack1.png");
	background->setAnchorPoint(CCPointZero);
	background->setPosition(ccp(widthStart, s.height-BLOCK_UNIT*3));
	m_infoBack->addChild(background);

	char* propName = "propName";
	char* propExplain = new char[50];//"propExplain";
	switch(type)
	{
	case BLOCK_BOMB:
		{
			propName = "Bomb";
			if(game_skill.skillLevel[SKILL_BIG_BOMB])
				strcpy(propExplain, "Explode on 9 cells");
			else
				strcpy(propExplain, "Explode on cross cells");
			break;
		}
	case BLOCK_MAGNIFIER:
		{
			propName = "Magnifier";
			strcpy(propExplain, "Show hidden property\n on 9 cells");
			break;
		}
	case BLOCK_FLYSHOE:
		{
			propName = "Boots of fly";
			strcpy(propExplain, "Fly to next level");
			//propExplain = "Fly to next level";
			break;
		}
	case BLOCK_FLASHLIGHT:
		{
			propName = "Flashight";
			strcpy(propExplain, "Open a random gold cell");
			//propExplain = "Open a random gold cell";
			break;
		}
	case BLOCK_SWEEP:
		{
			propName = "Sweeper";
			strcpy(propExplain, "Clear all empty cells");
			//propExplain = "Clear all empty cells";
			break;
		}
	case BLOCK_EYE:
		{
			propName = "Evil eye";
			int num = game_skill.getValue(SKILL_EVIL_EYE);
			sprintf(propExplain, "Show %d skulls at most", num);
			break;
		}
	default:
		break;
	}
	
	CCSprite* blockThing = CCSprite::create(s_blockContent[type].picName.c_str());
	blockThing->setScale(0.625f);
	blockThing->setPosition(ccp(widthStart+BLOCK_UNIT*0.5, s.height-30));
	m_infoBack->addChild(blockThing);
	CCLabelTTF* title = CCLabelTTF::create(propName, "AmericanTypewriter-Bold", 50.0f);
	title->setPosition(ccp(widthStart+BLOCK_UNIT*3,s.height - 30));
	title->setColor(ccc3(0,0,0));
	m_infoBack->addChild(title);

	CCLabelTTF* explain = CCLabelTTF::create(propExplain, "Marker Felt", 35.0f);
	explain->setPosition(ccp(widthStart+220,s.height - 90));
	explain->setColor(ccc3(0,0,0));
	m_infoBack->addChild(explain);
	
	delete propExplain;

	CCLabelTTF* forSell = CCLabelTTF::create("Or sell for 3", "Marker Felt", 35.0f);
	forSell->setPosition(ccp(widthStart+250,s.height - 140));
	forSell->setColor(ccc3(0,0,0));
	m_infoBack->addChild(forSell);
	CCSprite* moneySprite = CCSprite::create(PIC_SHOW_MONEY);
	moneySprite->setPosition(ccp(s.width-40,s.height - 140));
	moneySprite->setScale(0.7f);
	m_infoBack->addChild(moneySprite);

	addChild(m_infoBack);
}

void PropMenuLayer::showPropOption(PropOption option)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
	if(m_menuPropDo)
		m_menuPropDo->removeFromParentAndCleanup(true);
	if(m_menuPropCancel)
		m_menuPropCancel->removeFromParentAndCleanup(true);
	if(m_menuPropSell)
		m_menuPropSell->removeFromParentAndCleanup(true);
	switch(option)
	{
	case PROP_OPTION_CANCEL:
		{
			CCMenuItemImage* propSell = CCMenuItemImage::create(PIC_PROP_SELL,PIC_PROP_SELL,this,menu_selector(PropMenuLayer::menuPropSell));
			propSell->setAnchorPoint(ccp(0,0));
			m_menuPropSell = CCMenu::create(propSell, NULL);
			m_menuPropSell->setPosition(ccp(240+BLOCK_UNIT*3,s.height-BLOCK_UNIT*3));
			addChild(m_menuPropSell,20);
			
			CCMenuItemImage* propCancel = CCMenuItemImage::create(PIC_PROP_CANCEL,PIC_PROP_CANCEL,this,menu_selector(PropMenuLayer::menuPropCancel));
			propCancel->setAnchorPoint(ccp(0,0));
			m_menuPropCancel = CCMenu::create(propCancel, NULL);
			m_menuPropCancel->setPosition(ccp(240+BLOCK_UNIT*4,s.height-BLOCK_UNIT*3));
			addChild(m_menuPropCancel,20);
			break;
		}
	case PROP_OPTION_DO_CANCEL:
		{
			CCMenuItemImage* propDo = CCMenuItemImage::create(PIC_PROP_DO,PIC_PROP_DO,this,menu_selector(PropMenuLayer::menuPropDo));
			propDo->setAnchorPoint(ccp(0,0));
			m_menuPropDo = CCMenu::create(propDo, NULL);
			m_menuPropDo->setPosition(ccp(240+BLOCK_UNIT*2,s.height-BLOCK_UNIT*3));
			addChild(m_menuPropDo,20);
			
			CCMenuItemImage* propSell = CCMenuItemImage::create(PIC_PROP_SELL,PIC_PROP_SELL,this,menu_selector(PropMenuLayer::menuPropSell));
			propSell->setAnchorPoint(ccp(0,0));
			m_menuPropSell = CCMenu::create(propSell, NULL);
			m_menuPropSell->setPosition(ccp(240+BLOCK_UNIT*3,s.height-BLOCK_UNIT*3));
			addChild(m_menuPropSell,20);
			
			CCMenuItemImage* propCancel = CCMenuItemImage::create(PIC_PROP_CANCEL,PIC_PROP_CANCEL,this,menu_selector(PropMenuLayer::menuPropCancel));
			propCancel->setAnchorPoint(ccp(0,0));
			m_menuPropCancel = CCMenu::create(propCancel, NULL);
			m_menuPropCancel->setPosition(ccp(240+BLOCK_UNIT*4,s.height-BLOCK_UNIT*3));
			addChild(m_menuPropCancel,20);
			break;
		}
	default:
		break;
	}
}

void PropMenuLayer::menuPropDo(CCObject* pSender)
{
	PlayLayer* playLayer = (PlayLayer*)getParent();
	playLayer->menuPropDo(pSender);
}
void PropMenuLayer::menuPropCancel(CCObject* pSender)
{
	PlayLayer* playLayer = (PlayLayer*)getParent();
	playLayer->menuPropCancel(pSender);
}
void PropMenuLayer::menuPropSell(CCObject* pSender)
{
	PlayLayer* playLayer = (PlayLayer*)getParent();
	playLayer->menuPropSell(pSender);
}

void PropMenuLayer::clearPropMenu()
{
	if(m_menuPropDo)
		m_menuPropDo->removeFromParentAndCleanup(true);
	if(m_menuPropCancel)
		m_menuPropCancel->removeFromParentAndCleanup(true);
	if(m_menuPropSell)
		m_menuPropSell->removeFromParentAndCleanup(true);
	if(m_infoBack)
		m_infoBack->removeFromParentAndCleanup(true);
	m_menuPropDo = NULL;
	m_menuPropCancel = NULL;
	m_menuPropSell = NULL;
	m_infoBack = NULL;
}
