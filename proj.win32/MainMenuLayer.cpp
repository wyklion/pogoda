#include "MainMenuLayer.h"
#include "SceneManager.h"
#include "basic.h"
#include "MySound.h"
#include "UserData.h"

MainMenuScene::MainMenuScene(void)
{
}


MainMenuScene::~MainMenuScene(void)
{
}

bool MainMenuScene::init()
{
	if (!CCScene::init())  
    {  
        return false;  
    }

	MainMenuLayer* layer = (MainMenuLayer*)MainMenuLayer::create();
	addChild(layer);
	return true;
}

	
bool MainMenuLayer::init()
{
	if (!CCLayer::init())  
    {  
        return false;  
    }
	
    this->setTouchEnabled(true);//启用触摸
      
    CCSize s = CCDirector::sharedDirector()->getWinSize();

	//background...
	CCSprite* bg = CCSprite::create(PIC_MAIN_BACKGROUND);
    bg->setPosition(ccp(s.width/2, s.height/2));  
    addChild(bg);

	//add sprite...
	const int picnum = 8;
	const char* name[picnum] = {PIC_BLOCK_GOLD, PIC_BLOCK_STEP, PIC_BLOCK_SWEEP, PIC_BLOCK_FLYSHOE, PIC_BLOCK_BOMB, PIC_BLOCK_LIGHT, PIC_BLOCK_TREASURE, PIC_BLOCK_CLOUD};
	for(int i = 0; i < picnum; i++)
	{
		CCSprite* item = CCSprite::create(name[i]);
		CCPoint p = ccp(s.width/picnum*i+s.width/(picnum*2), 100);
		item->setPosition(p);
		item->runAction(CCSequence::create(CCDelayTime::create(0.5*(float)i), 			
			CCCallFuncN::create(this, callfuncN_selector(MainMenuLayer::jumpForever)), 
			NULL));
		addChild(item);
	}

	//admin layer...
	
    CCMenuItemFont* admin = CCMenuItemFont::create("Haha", this, menu_selector(MainMenuLayer::callbackAdmin));
    CCMenu* adminMenu = CCMenu::create(admin, NULL);
    adminMenu->setPosition(ccp(s.width*0.5, s.height*0.5+200));
	addChild(adminMenu);

	/*
    CCScale9Sprite* btnNormal = CCScale9Sprite::create("button1.png");
    CCScale9Sprite* btnDown = CCScale9Sprite::create("buttonHilit1.png");
	btnNormal->setColor(ccc3(255,0,0));
    CCLabelTTF* title = CCLabelTTF::create("HaHa", "Marker Felt", 50);
    CCControlButton* controlBtn = CCControlButton::create(title, btnNormal);
    controlBtn->setBackgroundSpriteForState(btnDown, CCControlStateSelected);
	controlBtn->setPreferredSize(CCSize(300, 50));
    controlBtn->setPosition(ccp(s.width*0.5, s.height*0.5+200));
	controlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(MainMenuLayer::callbackAdmin), CCControlEventTouchUpInside); 
    this->addChild(controlBtn);*/
	
	//music button...
	CCSprite* musicOn = CCSprite::create(PIC_MUSIC_ON);
	CCSprite* musicOff = CCSprite::create(PIC_MUSIC_OFF);
	CCMenuItemSprite* music1 = CCMenuItemSprite::create(musicOn, musicOn);
	CCMenuItemSprite* music2 = CCMenuItemSprite::create(musicOff, musicOff);
	if(!SceneManager::music)
	{
		CCMenuItemSprite* tmp = music1;
		music1 = music2;
		music2 = tmp;
	}
	CCMenuItemToggle *music_toggle = CCMenuItemToggle::createWithTarget(this, menu_selector(MainMenuLayer::music), music1, music2, NULL);
	CCMenu *musicMenu = CCMenu::create(music_toggle,NULL);
	musicMenu->setPosition(ccp(s.width-40,s.height-40));
	this->addChild(musicMenu);

	// 创建一个CCMenuItemFont    
    CCMenuItemFont *item1 = CCMenuItemFont::create("Play", this, menu_selector(MainMenuLayer::menuCallbackPlay) );  
	item1->setFontSizeObj(100);
    //item1->setFontName("American Typewriter");  
    CCMenuItemFont *item2 = CCMenuItemFont::create("Skill", this, menu_selector(MainMenuLayer::menuCallbackSkill) );  
    item2->setFontSizeObj(100);  
    //item2->setFontName("Marker Felt");  
    CCMenuItemFont *item3 = CCMenuItemFont::create("Recording", this, menu_selector(MainMenuLayer::menuCallbackRecording) );  
    item3->setFontSizeObj(100);  


	//创建CCMenu菜单，其他可认为是菜单项   
    CCMenu* menu = CCMenu::create( item1, item2, item3, NULL);  
    menu->alignItemsVertically();//可以理解为设置成垂直排列   
    //menu->alignItemsHorizontally();//水平排列   

	addChild(menu);
    menu->setPosition(ccp(s.width/2, s.height/2));  

	
	/*CCParticleSystemQuad *system = CCParticleSystemQuad::create("mergeEffect.plist");
	system->setPosition(100,100);
	system->setTextureWithRect(CCTextureCache::sharedTextureCache()->addImage("stars.png"), CCRectMake(0,0,32,32));
	addChild(system, 10);
	CCParticleFire* fire = CCParticleFire::create();
	fire->setPosition(s.width/2, 800);
	addChild(fire);*/


	return true;

}

MainMenuLayer::~MainMenuLayer()
{
}

void MainMenuLayer::menuCallbackPlay(CCObject* sender) 
{
	MySound::playSound(SOUND_SELECT_PROP);
	
	UserData::startGame();
	SceneManager::goPlay();
	/*
	if(game_process.firstPlay)
	{
		SceneManager::goFirst();
	}
	else
	{
		SceneManager::goPlay();
	}*/
}

void MainMenuLayer::menuCallbackSkill(CCObject* sender) 
{
	MySound::playSound(SOUND_SELECT_PROP);
	SceneManager::goSkill(0); //1 -> right
}

void MainMenuLayer::menuCallbackRecording(CCObject* sender) 
{
	MySound::playSound(SOUND_SELECT_PROP);
	SceneManager::goRecording();
}

void MainMenuLayer::callbackAdmin(CCObject *pSender) 
{
	MySound::playSound(SOUND_SELECT_PROP);
	SceneManager::goAdmin();
}

void MainMenuLayer::music(CCObject *pSender)
{
	MySound::playSound(SOUND_SELECT_PROP);
	if(SceneManager::music)
	{
		SceneManager::music = false;
		MySound::stopMusic();
	}
	else
	{
		SceneManager::music = true;
		MySound::playBackGround(2);
	}
}

void MainMenuLayer::jumpForever(CCNode* obj)
{
	CCSprite* item = (CCSprite*)obj;

	CCActionInterval* seq = (CCActionInterval*)CCSequence::create(CCJumpBy::create(0.5f, ccp(0,0), 50, 1), CCDelayTime::create(3.5), NULL);
	item->runAction(CCRepeatForever::create(seq));
}