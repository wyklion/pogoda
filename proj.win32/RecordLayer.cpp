#include "RecordLayer.h"
#include "SceneManager.h"
#include "UserData.h"
#include "MySound.h"

RecordLayer::~RecordLayer(void)
{
}

bool RecordLayer::init()
{
	if(!CCLayerColor::initWithColor(ccc4(61,89,171,255)))
	{  
		return false;  
	}
	m_records = CCArray::create();

	//show record...
	showRecording();
	
    CCSize s = CCDirector::sharedDirector()->getWinSize();

	//Back Button...
	CCMenuItemImage* back = CCMenuItemImage::create(PIC_BUTTON_BACK, PIC_BUTTON_BACK_SELECTED, this, menu_selector(RecordLayer::back));
	CCMenu* menu = CCMenu::create(back, NULL);
	menu->setPosition(ccp(s.width*0.5, 100));
	this->addChild(menu);  

	return true;
}

void RecordLayer::back(CCObject *pSender)  
{
	MySound::playSound(SOUND_SELECT_PROP);
	SceneManager::goMenu();  
}

void RecordLayer::setUserDataWords(const char* xmlName, const char* showName)
{
	int num = CCGetInt(xmlName);
	char buf[80];
	sprintf(buf, showName, num);
	//CCMenuItemFont* item = CCMenuItemFont::create(buf);
	CCLabelTTF* label = CCLabelTTF::create(buf, "Marker Felt", 30.0);
	m_records->addObject(label);
}

void RecordLayer::showRecording()
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();

	int c = m_records->count();
	for(int i = 0; i < RecorderNum; i++)
	{
		if(s_recorderContent[i].xmlName.length() == 0)
			continue;
		int num = CCGetInt(s_recorderContent[i].xmlName.c_str());
		char buf[80];
		sprintf(buf, s_recorderContent[i].showName.c_str(), num);
		CCLabelTTF* label = CCLabelTTF::create(buf, "Marker Felt", 30.0);
		label->setPosition(ccp(s.width*0.5, s.height-100 - 60*i));
		label->setColor(ccc3(0,0,0));
		addChild(label);
	}
}
