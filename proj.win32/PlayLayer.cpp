#include "PlayLayer.h"
#include "SceneManager.h"
#include "StopLayer.h"
#include "GameOverLayer.h"
#include "MySound.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;

bool PlayLayer::init()
{	
	if (!CCLayer::init())  
	{  
		return false;  
	}
    CCSize s = CCDirector::sharedDirector()->getWinSize();

	//stop button...
	CCMenuItemImage* stopItem = CCMenuItemImage::create(
            PIC_STOPBUTTON,
            PIC_STOPBUTTON_SELECTED,
            this,
            menu_selector(PlayLayer::menuStopCallback));
	stopItem->setPosition(ccp(s.width - 40, s.height - 40));
	// Create a menu with the "close" menu item, it's an auto release object.
	m_stopMenu = CCMenu::create(stopItem, NULL);
	m_stopMenu->setPosition(CCPointZero);
	this->addChild(m_stopMenu, 15);
	
	//backgound...
	//m_background = CCLayerColor::create(ccc4(game_process.backColor.r,game_process.backColor.g,game_process.backColor.b,255));
	m_background = CCLayerColor::create(ccc4(135,206,235,255));
	m_background->setContentSize(CCSizeMake(640,320));
	m_background->setAnchorPoint(ccp(0,0));
	m_background->setPosition(ccp(0, 640));
	addChild(m_background, 0);
	
	//prop layer...
	m_propLayer = PropLayer::create();
	m_propLayer->setContentSize(CCSizeMake(320,80));
	m_propLayer->setAnchorPoint(ccp(0, 0));
	m_propLayer->setPosition(240, s.height-80);
	this->addChild(m_propLayer, 5);

	//block layer...
	m_blockLayer = BlockLayer::create(game_process.level);
	m_blockLayer->setContentSize(CCSizeMake(s.width, s.width));
	m_blockLayer->setAnchorPoint(ccp(0,0));
	m_blockLayer->setPosition(0,0);
	this->addChild(m_blockLayer, 10);

	//info layer...
	m_infoLayer = InfoLayer::create();
	//m_infoLayer->setContentSize(CCSizeMake(200,200));
	//m_infoLayer->setAnchorPoint(ccp(0, 0));
	//m_infoLayer->setPosition(0, s.height-200);
	this->addChild(m_infoLayer, 15);
	
	//prop menu layer...
	m_propMenuLayer = PropMenuLayer::create();
	this->addChild(m_propMenuLayer, 20);

	if(game_process.state == GAME_STATE_FIRSTPLAY)
		showHelp();
	else
	{
		game_process.state = GAME_STATE_NORMAL;
		if(!newthings())
			showLevel();
	}
	if(SceneManager::music)
		MySound::playBackGround(1);

	m_foundLayer = NULL;

	m_done = false;

	return true;
}

void PlayLayer::showHelp()
{
	game_process.state = GAME_STATE_FIRSTPLAY;
	m_help = CCNode::create();
	addChild(m_help,1000);
	
    CCSize s = CCDirector::sharedDirector()->getWinSize();
	//background...
	CCSprite* help = CCSprite::create("help1.png");
	help->setAnchorPoint(ccp(0.5,0.5));
	help->setPosition(ccp(s.width*0.5, s.height*0.5));
	m_help->addChild(help);

}

void PlayLayer::removeHelp()
{
	m_help->runAction(CCSequence::create(CCFadeOut::create(0.5f),
		CCCallFuncN::create(this, callfuncN_selector(PlayLayer::callbackRemoveHelp)),
		NULL));
}

void PlayLayer::callbackRemoveHelp(CCNode* node)
{
	m_help->removeFromParentAndCleanup(true);
	game_process.state = GAME_STATE_NORMAL;
	if(!newthings())
		showLevel();
}

bool PlayLayer::newthings()
{	
	std::vector<BLOCK_TYPE> newthings;
	for(int i = (int)BLOCK_NONE; i < (int)BLOCK_TYPE_NUM; i++)
	{
		if(!game_process.isBlockFound[i] && game_process.level == s_blockContent[i].startLevel)
			newthings.push_back((BLOCK_TYPE)i);
	}
	int num = newthings.size();
	if(num>0)
	{
		game_process.state = GAME_STATE_NEWTHING;
		m_newthing = CCNode::create();
		addChild(m_newthing,1000);
		CCSize s = CCDirector::sharedDirector()->getWinSize();

		//background...
		CCScale9Sprite* bg = CCScale9Sprite::create(PIC_ROUND);
		bg->setPreferredSize(CCSizeMake(400, 400));
		bg->setAnchorPoint(ccp(0.5,0.5));
		bg->setColor(ccc3(163,148,128));
		//bg->setOpacity(128);
		bg->setPosition(ccp(s.width*0.5, s.height*0.5));
		m_newthing->addChild(bg);
		
		//new things: label
		CCLabelTTF* label = CCLabelTTF::create("New things:", "Marker Felt", 30.0/*, CCSizeMake(200, 50), kCCTextAlignmentLeft*/);
		label->setAnchorPoint(ccp(0.5, 0.5));
		label->setPosition(ccp(s.width*0.5, s.height*0.5 + 100));
		label->setColor(ccc3(0,0,0));
		m_newthing->addChild(label);
	
		//new things...
		int left = (s.width - num * BLOCK_UNIT - (num-1)*BLOCK_UNIT*0.5)*0.5;
		for(int i = 0; i < num; i++)
		{
			CCSprite* newthing = CCSprite::create(s_blockContent[newthings[i]].picName.c_str());
			newthing->setAnchorPoint(ccp(0,0.5));
			newthing->setPosition(ccp(left + i*BLOCK_UNIT*1.5, s.height*0.5));
			m_newthing->addChild(newthing);
		}
		m_newthing->setPosition(ccp(-520,0));
		m_newthing->runAction(CCSequence::create(CCDelayTime::create(0.5f), CCMoveBy::create(0.5f, ccp(520,0)), NULL));
		return true;
	}
	return false;
}

void PlayLayer::removeNewthing()
{
	m_newthing->runAction(CCSequence::create(CCMoveBy::create(0.5f, ccp(520,0)),
		CCCallFuncN::create(this, callfuncN_selector(PlayLayer::callbackRemoveNewthing)),
		NULL));
}

void PlayLayer::callbackRemoveNewthing(CCNode* node)
{
	m_newthing->removeFromParentAndCleanup(true);
	game_process.state = GAME_STATE_NORMAL;
	showLevel();
}

void PlayLayer::showLevel()
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
	char buf[20];
	sprintf(buf, "Level %d", game_process.level);
	CCLabelTTF* label = CCLabelTTF::create(buf, "Marker Felt", 100.0);
	label->setColor(ccc3(227,23,13));
	label->setPosition(ccp(s.width*0.5,s.height*0.5-200));
	addChild(label,30);
	CCActionInterval* act1 = (CCActionInterval*)CCSpawn::create(CCFadeIn::create(1.0f),CCMoveBy::create(1.0f, ccp(0,200)),NULL);
	CCActionInterval* act2 = (CCActionInterval*)CCSpawn::create(CCFadeOut::create(1.5f),CCMoveBy::create(1.5f, ccp(0,200)),NULL);
	label->runAction(CCSequence::create(act1, CCDelayTime::create(1.0f), act2, NULL));
}

void PlayLayer::removeSprite(CCNode* obj)
{
	CCSprite* sprite = (CCSprite*)obj;
	sprite->removeFromParentAndCleanup(true);
}

void PlayLayer::gameUpdate()
{
	m_infoLayer->updateInfo();
	if(m_done)
		return;
	if(game_process.state == GAME_STATE_WIN)
	{
		m_done = true;
		levelComplete();
	}
	else if(game_process.state == GAME_STATE_LOSE)
	{
		m_done = true;
		gameOver();
	}
}

void PlayLayer::gameOver()
{
	if(m_foundLayer)
		m_foundLayer->removeAllChildrenWithCleanup(true);

	UserData::gameOver();
	setTouchEnabled(false);
	m_blockLayer->showHiddenGold();	
	GameOverLayer* gameOverLayer = GameOverLayer::create(game_process);
	this->addChild(gameOverLayer, 100);
	MySound::stopMusic();
	MySound::playSound(SOUND_GAMEOVER);
}

void PlayLayer::levelComplete()
{
	if(m_foundLayer)
		m_foundLayer->removeAllChildrenWithCleanup(true);

	game_process.backColor = ccc3(rand()%256,rand()%256,rand()%256);
	m_background->runAction(CCTintTo::create(1.0f, game_process.backColor.r, game_process.backColor.g, game_process.backColor.b));
	MySound::playSound(SOUND_LEVEL_COMPLETE);
	UserData::levelComplete();
	float time = m_blockLayer->winAndClearBlock();
	showLevelPassInfo();
	CCDelayTime* waiting=CCDelayTime::create(time);  
	runAction(CCSequence::create(waiting, CCCallFunc::create(this, callfunc_selector(PlayLayer::newLevel)), NULL)); 
}

void PlayLayer::showLevelPassInfo()
{
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	CCNode* passInfo = CCNode::create();
	passInfo->setAnchorPoint(ccp(0.5,0.5));
	passInfo->setPosition(ccp(s.width*0.5, s.height*0.5 + 100));
	addChild(passInfo, 1000);

	//background...
	CCScale9Sprite* bg = CCScale9Sprite::create(PIC_ROUND);
	bg->setPreferredSize(CCSizeMake(400, 400));
	bg->setAnchorPoint(ccp(0.5,0.5));
	bg->setColor(ccc3(163,148,128));
	//bg->setOpacity(128);
	bg->setPosition(ccp(s.width*0.5, s.height*0.5));
	passInfo->addChild(bg);

	//new things: label
	CCLabelTTF* label = CCLabelTTF::create("Level Complete", "Marker Felt", 50.0/*, CCSizeMake(200, 50), kCCTextAlignmentLeft*/);
	label->setAnchorPoint(ccp(0.5, 0.5));
	label->setPosition(ccp(s.width*0.5, s.height*0.5 + 150));
	label->setColor(ccc3(50,60,70));
	passInfo->addChild(label);

	char buf[50];
	//level step...
	sprintf(buf, " Open Times: %d", m_blockLayer->m_levelData.openTime);
	label = CCLabelTTF::create(buf, "Marker Felt", 30.0/*, CCSizeMake(200, 50), kCCTextAlignmentLeft*/);
	label->setAnchorPoint(ccp(0, 0.5));
	label->setPosition(ccp(s.width*0.5-120, s.height*0.5 + 50));
	label->setColor(ccc3(0,0,0));
	passInfo->addChild(label);
	//level money...
	sprintf(buf, "Level Bonus: %d", m_blockLayer->m_levelData.level);
	label = CCLabelTTF::create(buf, "Marker Felt", 30.0/*, CCSizeMake(200, 50), kCCTextAlignmentLeft*/);
	label->setAnchorPoint(ccp(0, 0.5));
	label->setPosition(ccp(s.width*0.5-120, s.height*0.5));
	label->setColor(ccc3(0,0,0));
	passInfo->addChild(label);

	////new things...
	//int left = (s.width - num * BLOCK_UNIT - (num-1)*BLOCK_UNIT*0.5)*0.5;
	//for(int i = 0; i < num; i++)
	//{
	//	CCSprite* newthing = CCSprite::create(s_blockContent[newthings[i]].picName.c_str());
	//	newthing->setAnchorPoint(ccp(0,0.5));
	//	newthing->setPosition(ccp(left + i*BLOCK_UNIT*1.5, s.height*0.5));
	//	m_newthing->addChild(newthing);
	//}
	passInfo->setPosition(ccp(0,480));
	passInfo->runAction(CCEaseExponentialOut::create(CCMoveBy::create(1.0f, ccp(0,-480))));
}

void PlayLayer::newLevel()
{
	UserData::newLevel();
	SceneManager::goPlay();
}

void PlayLayer::back(cocos2d::CCObject *pSender)  
{  
	SceneManager::goMenu();  
	//back就是回主菜单，这里调用了人见人爱 花见花开的静态函数 goMenu  
}

PlayLayer::~PlayLayer()
{

}

//---SpriteClassModel 按键-----------------
// 别忘记加入 TouchDispatcher  ,TouchDispatcher一般放在onenter 绑定，onexit 删除
bool PlayLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	if(game_process.state == GAME_STATE_FIRSTPLAY)
	{
		removeHelp();
		return false;
	}
	else if(game_process.state == GAME_STATE_NEWTHING)
	{
		removeNewthing();
		return false;
	}
	else if(game_process.state == GAME_STATE_WIN)
	{
		CCLog("newLevel...");
		PlayLayer::newLevel();
		return false;
	}
	else if(game_process.state == GAME_STATE_LOSE)
		return false;
	else if(game_process.state == GAME_STATE_FOUND)
	{
		if(m_foundLayer && m_foundLayer->m_allShow)
		{
			m_foundLayer->close();
			m_foundLayer = NULL;
		}
		return false;
	}
	else
		return true;
}

void PlayLayer::ccTouchMoved(CCTouch* touch, CCEvent* event)
{   
	if(game_process.state != GAME_STATE_NORMAL)
		return;
	//m_pieceManager->move(touch); 
	//CCPoint touchPoint = touch->getLocation();  	
	//获取当前用户触屏点坐标并将3d坐标映射2d坐标系   
	//touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);
	//touchPoint.x, getPosition().y   
	//if(touchPoint.x> CCDirector::sharedDirector()->getWinSize().width/2)
		//CCLog("%s","精灵22~~ccTouchMoved");
}

void PlayLayer::ccTouchEnded(CCTouch* touch, CCEvent* event)
{    
	if(game_process.state != GAME_STATE_NORMAL)
		return;
	//m_pieceManager->endTouch(touch);
	
	//if(m_pieceManager->isFinished())
	//	SceneManager::goMenu();
	//CCLog("%s","精灵33~~~ccTouchEnded");
}

void PlayLayer::onEnter()
{   
	//JigsawSound::resumeMusic();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
	CCLayer::onEnter();
}

void PlayLayer::onExit()
{   
	//MySound::stopAll();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);  
	CCLayer::onExit();
}

void PlayLayer::menuStopCallback(CCObject* pSender)
{
	/*if(game_process.state == GAME_STATE_WIN || 
		game_process.state == GAME_STATE_LOSE ||
		game_process.state == GAME_STATE_FOUND ||
		game_process.state == GAME_STATE_FIRSTPLAY ||
		game_process.state == GAME_STATE_NEWTHING ||
		game_process.state == GAME_STATE_PROP)
		return;*/
	if(game_process.state != GAME_STATE_NORMAL)
		return;
	MySound::playSound(SOUND_SELECT_PROP);
	StopLayer* stop = StopLayer::create();
	stop->initWithColor(ccc4(0,0,0,125));
	this->addChild(stop, 100);
	setStopVisible(false);
	//SceneManager::goStop(); 
}

void PlayLayer::setStopVisible(bool b)
{
	m_stopMenu->setVisible(b);
}

void PlayLayer::foundNewBlockThing(BLOCK_TYPE type)
{
	if(game_process.state == GAME_STATE_FOUND ||
		game_process.state == GAME_STATE_WIN)
		return;
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	if(game_process.isBlockFound[type])
		return;
	
	game_process.state = GAME_STATE_FOUND;
	CCLog("found...");
	UserData::saveFoundNewBlockThing(type);
	game_process.isBlockFound[type] = true;
	m_foundLayer = FoundLayer::create(type);
	addChild(m_foundLayer, 100);
}

void PlayLayer::getPropToPropPack(CCSprite* prop, BLOCK_TYPE type)
{
	int cellNum = m_propLayer->getCellNum(type);
	if(cellNum<0)
	{
		foundNewBlockThing(type);
		return;
	}
	m_propLayer->addProp(type);

	prop->setAnchorPoint(ccp(0.5,0.5));
	CCPoint pos = prop->getPosition();
	prop->setPosition(ccp(pos.x+BLOCK_UNIT/2,pos.y+BLOCK_UNIT/2));
	prop->getParent()->reorderChild(prop, LAYERNUM_PROP_TOP);
	CCFiniteTimeAction* action = CCScaleBy::create(0.2f, 1.5f);
    CCSize s = CCDirector::sharedDirector()->getWinSize();
	pos = ccp(240 + BLOCK_UNIT*(cellNum+0.5), s.height-80 + BLOCK_UNIT*0.5);
	prop->runAction(CCSequence::create(action,
		/*CCSpawn::create(*/action->reverse(), CCMoveTo::create(0.5f, pos), 
		CCCallFuncN::create(this, callfuncN_selector(PlayLayer::setPropToPropLayer)), 
		NULL));
	//prop->runAction(CCSequence::create(CCDelayTime::create(1.0f), 
	//	CCCallFuncN::create(this, callfuncN_selector(PlayLayer::setPropToPropLayer)), 
	//	NULL));
}

void PlayLayer::setPropToPropLayer(CCNode* obj)
{
	int tag = obj->getTag()-2000;
	((CCSprite*)obj)->removeFromParentAndCleanup(true);
	m_propLayer->updatePropCells();
	foundNewBlockThing(m_blockLayer->getBlockTypeByTag(tag));
}

void PlayLayer::menuPropDo(CCObject* pSender)
{
	Prop_Cell& propCell = game_process.propCells[game_process.selectProp];
	switch(propCell.type)
	{
	case BLOCK_FLYSHOE:
		{
			m_propLayer->unselectProp();
			game_process.state = GAME_STATE_WIN;
			propCell.num--;
			updatePropCells();
			gameUpdate();
			break;
		}
	case BLOCK_FLASHLIGHT:
		{
			m_propLayer->unselectProp();
			propCell.num--;
			updatePropCells();
			m_blockLayer->openRandomGold();
			break;
		}
	case BLOCK_SWEEP:
		{
			MySound::playSound(SOUND_PROP_DO);
			m_propLayer->unselectProp();
			propCell.num--;
			updatePropCells();
			m_blockLayer->sweepBlankBlock();
			break;
		}
	case BLOCK_EYE:
		{
			MySound::playSound(SOUND_PROP_DO);
			m_propLayer->unselectProp();
			propCell.num--;
			updatePropCells();
			//m_blockLayer->showAllDeadBlock();
			m_blockLayer->showSomeDeadBlock(game_skill.getValue(SKILL_EVIL_EYE));
			break;
		}
	default:
		break;
	}
	clearPropMenu();
}

void PlayLayer::menuPropSell(CCObject* pSender)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();	
	CCPoint p = ccp(s.width/2-BLOCK_UNIT/2, s.height-BLOCK_UNIT*3);

	Prop_Cell& propCell = game_process.propCells[game_process.selectProp];
	switch(propCell.type)
	{
	case BLOCK_FLASHLIGHT:
	case BLOCK_SWEEP:
	case BLOCK_EYE:
	default:
		game_process.money+=3;
		game_process.getMoney+=3;
		MySound::playSound(SOUND_MONEY);
		m_blockLayer->showGetting(p, PIC_SHOW_MONEY, 3);
		break;
	}
	m_propLayer->unselectProp();
	propCell.num--;
	updatePropCells();
	clearPropMenu();
	m_infoLayer->updateInfo();
}

void PlayLayer::menuPropCancel(CCObject* pSender)
{
	if(game_process.state != GAME_STATE_PROP)
		return;
	m_propLayer->selectProp(game_process.selectProp);
}

void PlayLayer::clearPropMenu()
{
	m_propMenuLayer->clearPropMenu();
	CCNode* cover = getChildByTag(1999);
	if(cover)
		cover->removeFromParentAndCleanup(true);
}

void PlayLayer::checkProp(BLOCK_TYPE type)
{
	m_propMenuLayer->set(type);

	switch(type)
	{
	case BLOCK_FLYSHOE:
	case BLOCK_FLASHLIGHT:
	case BLOCK_SWEEP:
	case BLOCK_EYE:
		{
			CCLayerColor* cover = CCLayerColor::create(ccc4(0,0,0,128));
			cover->setContentSize(CCSizeMake(BLOCK_UNIT*BLOCK_COL,BLOCK_UNIT*BLOCK_ROW));
			cover->setAnchorPoint(ccp(0,0));
			cover->setPosition(ccp(0,0));
			addChild(cover, 30, 1999);
			break;
		}
	case BLOCK_BOMB:
	case BLOCK_MAGNIFIER:
	default:
		break;
	}
}