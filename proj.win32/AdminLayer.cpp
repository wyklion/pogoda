#include "AdminLayer.h"
#include "SceneManager.h"
#include "UserData.h"
#include "MySound.h"

bool AdminLayer::init()
{
	if(!CCLayerColor::initWithColor(ccc4(61,89,171,255)))
	{  
		return false;  
	}
	
    CCSize s = CCDirector::sharedDirector()->getWinSize();

	

	//add money
    CCMenuItemFont *item1 = CCMenuItemFont::create("Add 1000 money", this, menu_selector(AdminLayer::addMoney) );  
	item1->setFontSizeObj(50);
    //item1->setFontName("American Typewriter");  
    CCMenuItemFont *item2 = CCMenuItemFont::create("reset", this, menu_selector(AdminLayer::reset) );  
    item2->setFontSizeObj(50); 

	//menu
	CCMenu* menu = CCMenu::create(item1, item2, NULL);  
    menu->alignItemsVertically();
	addChild(menu);
    menu->setPosition(ccp(s.width/2, s.height/2));  



	//Back Button...
	CCMenuItemImage* back = CCMenuItemImage::create(PIC_BUTTON_BACK, PIC_BUTTON_BACK_SELECTED, this, menu_selector(AdminLayer::back));
	CCMenu* backMenu = CCMenu::create(back, NULL);
	backMenu->setPosition(ccp(s.width*0.5, 100));
	this->addChild(backMenu);  

	return true;
}

void AdminLayer::back(CCObject* obj)  
{
	MySound::playSound(SOUND_SELECT_PROP);
	SceneManager::goMenu();  
}

void AdminLayer::addMoney(CCObject* obj)  
{
	MySound::playSound(SOUND_SELECT_PROP);

	int money = CCGetInt("Money");
	money += 1000;
	CCSetInt("Money", money);
	CCUserDefault::sharedUserDefault()->flush();
}

void AdminLayer::reset(CCObject* obj)  
{
	MySound::playSound(SOUND_SELECT_PROP);
	
	CCSetBool("firstPlay", true);
	CCSetInt("Money", 0);
	CCSetInt("Gold", 0);
	CCSetInt("High_Level", 0);
	CCSetInt("High_Get_Money", 0);
	CCSetInt("High_Open_Gold", 0);
	CCSetInt("High_Open_Dead", 0);
	CCSetInt("All_Get_Money", 0);
	CCSetInt("All_Open_Gold", 0);
	CCSetInt("All_Open_Dead", 0);
	CCSetInt("All_Open_Times", 0);
	char buf[50];
	//skill...
	for(int i = 0; i < (int)SKILL_NUM; i++)
	{
		sprintf(buf, "skill%d", (Skill_Type)i);
		CCSetInt(buf, 0);
	}	
	//found...
	for(int i = 0; i < BLOCK_TYPE_NUM; i++)
	{
		sprintf(buf, "found%d", i);
		CCSetBool(buf, false);
	}
	CCUserDefault::sharedUserDefault()->flush();
}
