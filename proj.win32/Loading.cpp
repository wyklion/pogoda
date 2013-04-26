#include "Loading.h"
#include "SceneManager.h"
#include "basic.h"
#include "MySound.h"

bool InitLoading::init()
{
	if(!CCLayer::init())
		return false;
	m_nNumberOfSprites = 49;
	m_nNumberOfLoadedSprites = 0;
    CCSize size = CCDirector::sharedDirector()->getWinSize();

    m_pLabelLoading = CCLabelTTF::create("Loading...", "Arial", 50);
	m_pLabelLoading->setColor(ccc3(255,215,0));
    m_pLabelPercent = CCLabelTTF::create("%0", "Arial", 50);
	m_pLabelPercent->setColor(ccc3(255,215,0));

    m_pLabelLoading->setPosition(ccp(size.width / 2, size.height / 2 - 30));
    m_pLabelPercent->setPosition(ccp(size.width / 2, size.height / 2 + 30));

    this->addChild(m_pLabelLoading);
    this->addChild(m_pLabelPercent);

    // load textrues
	//some pics...
    CCTextureCache::sharedTextureCache()->addImageAsync(PIC_MAIN_BACKGROUND, this, callfuncO_selector(InitLoading::loadingCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync(PIC_BUTTON, this, callfuncO_selector(InitLoading::loadingCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync(PIC_BUTTONHILIT, this, callfuncO_selector(InitLoading::loadingCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync(PIC_BLOCK, this, callfuncO_selector(InitLoading::loadingCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync(PIC_BLOCK_HILIT, this, callfuncO_selector(InitLoading::loadingCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync(PIC_BLOCK_BACKGROUND, this, callfuncO_selector(InitLoading::loadingCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync(PIC_ROUND, this, callfuncO_selector(InitLoading::loadingCallBack));	
    CCTextureCache::sharedTextureCache()->addImageAsync(PIC_LOTTERY, this, callfuncO_selector(InitLoading::loadingCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync(PIC_LOTTERY_SMILE, this, callfuncO_selector(InitLoading::loadingCallBack));	
	
	//block pic...
    CCTextureCache::sharedTextureCache()->addImageAsync(PIC_BLOCK_GOLD, this, callfuncO_selector(InitLoading::loadingCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync(PIC_BLOCK_DEAD1, this, callfuncO_selector(InitLoading::loadingCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync(PIC_BLOCK_DEAD2, this, callfuncO_selector(InitLoading::loadingCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync(PIC_BLOCK_DEAD3, this, callfuncO_selector(InitLoading::loadingCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync(PIC_BLOCK_BOMB, this, callfuncO_selector(InitLoading::loadingCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync(PIC_BLOCK_MONEY, this, callfuncO_selector(InitLoading::loadingCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync(PIC_BLOCK_TREASURE, this, callfuncO_selector(InitLoading::loadingCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync(PIC_BLOCK_FLYSHOE, this, callfuncO_selector(InitLoading::loadingCallBack));
	CCTextureCache::sharedTextureCache()->addImageAsync(PIC_BLOCK_STEP, this, callfuncO_selector(InitLoading::loadingCallBack));
	CCTextureCache::sharedTextureCache()->addImageAsync(PIC_BLOCK_MAGNIFIER, this, callfuncO_selector(InitLoading::loadingCallBack));	
	CCTextureCache::sharedTextureCache()->addImageAsync(PIC_BLOCK_FLASHLIGHT, this, callfuncO_selector(InitLoading::loadingCallBack));	
	CCTextureCache::sharedTextureCache()->addImageAsync(PIC_BLOCK_SWEEP, this, callfuncO_selector(InitLoading::loadingCallBack));	
	CCTextureCache::sharedTextureCache()->addImageAsync(PIC_BLOCK_EYE, this, callfuncO_selector(InitLoading::loadingCallBack));	
	//button pic...
    CCTextureCache::sharedTextureCache()->addImageAsync(PIC_STOPBUTTON, this, callfuncO_selector(InitLoading::loadingCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync(PIC_STOPBUTTON_SELECTED, this, callfuncO_selector(InitLoading::loadingCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync(PIC_MUSIC_ON, this, callfuncO_selector(InitLoading::loadingCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync(PIC_MUSIC_OFF, this, callfuncO_selector(InitLoading::loadingCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync(PIC_SOUND_ON, this, callfuncO_selector(InitLoading::loadingCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync(PIC_SOUND_OFF, this, callfuncO_selector(InitLoading::loadingCallBack));	
    CCTextureCache::sharedTextureCache()->addImageAsync(PIC_RESUME, this, callfuncO_selector(InitLoading::loadingCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync(PIC_RESUME_SELECTED, this, callfuncO_selector(InitLoading::loadingCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync(PIC_QUIT, this, callfuncO_selector(InitLoading::loadingCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync(PIC_QUIT_SELECTED, this, callfuncO_selector(InitLoading::loadingCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync(PIC_BUTTON_BACK, this, callfuncO_selector(InitLoading::loadingCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync(PIC_BUTTON_BACK_SELECTED, this, callfuncO_selector(InitLoading::loadingCallBack));
	//prop pic...
    CCTextureCache::sharedTextureCache()->addImageAsync(PIC_PROP_BACKGROUND, this, callfuncO_selector(InitLoading::loadingCallBack));
    CCTextureCache::sharedTextureCache()->addImageAsync(PIC_PROP_DO, this, callfuncO_selector(InitLoading::loadingCallBack));
	CCTextureCache::sharedTextureCache()->addImageAsync(PIC_PROP_CANCEL, this, callfuncO_selector(InitLoading::loadingCallBack));
	CCTextureCache::sharedTextureCache()->addImageAsync(PIC_PROP_SELL, this, callfuncO_selector(InitLoading::loadingCallBack));	
	CCTextureCache::sharedTextureCache()->addImageAsync(PIC_PROP_INFOBACK, this, callfuncO_selector(InitLoading::loadingCallBack));		
	//show pic...
	CCTextureCache::sharedTextureCache()->addImageAsync(PIC_SHOW_LEVEL, this, callfuncO_selector(InitLoading::loadingCallBack));
	CCTextureCache::sharedTextureCache()->addImageAsync(PIC_SHOW_STEP, this, callfuncO_selector(InitLoading::loadingCallBack));
	CCTextureCache::sharedTextureCache()->addImageAsync(PIC_SHOW_MONEY, this, callfuncO_selector(InitLoading::loadingCallBack));
	CCTextureCache::sharedTextureCache()->addImageAsync(PIC_SHOW_GOLD, this, callfuncO_selector(InitLoading::loadingCallBack));	
	//skill pic...
	CCTextureCache::sharedTextureCache()->addImageAsync(PIC_SKILL_ORIGINALSTEP, this, callfuncO_selector(InitLoading::loadingCallBack));	
	CCTextureCache::sharedTextureCache()->addImageAsync(PIC_SKILL_STEP, this, callfuncO_selector(InitLoading::loadingCallBack));	
	CCTextureCache::sharedTextureCache()->addImageAsync(PIC_SKILL_BIGBOMB, this, callfuncO_selector(InitLoading::loadingCallBack));	
	CCTextureCache::sharedTextureCache()->addImageAsync(PIC_SKILL_DEFENSE, this, callfuncO_selector(InitLoading::loadingCallBack));	
	CCTextureCache::sharedTextureCache()->addImageAsync(PIC_SKILL_MOREMONEY, this, callfuncO_selector(InitLoading::loadingCallBack));	
	CCTextureCache::sharedTextureCache()->addImageAsync(PIC_SKILL_EYE, this, callfuncO_selector(InitLoading::loadingCallBack));	

	MySound::loadSounds();
	
	CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	cache->addSpriteFramesWithFile(PIC_TPLIST);

	return true;
}

void InitLoading::loadingCallBack(CCObject *obj)
{
    ++m_nNumberOfLoadedSprites;
    char tmp[10];
    sprintf(tmp,"%d%%", (int)(((float)m_nNumberOfLoadedSprites / m_nNumberOfSprites) * 100));
    m_pLabelPercent->setString(tmp);

    if (m_nNumberOfLoadedSprites == m_nNumberOfSprites)
    {
        this->removeChild(m_pLabelLoading, true);
        this->removeChild(m_pLabelPercent, true);
        SceneManager::goMenu();
    }
}
