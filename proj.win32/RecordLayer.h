#ifndef __RECORD_LAYER_H__
#define __RECORD_LAYER_H__   

#include "cocos2d.h" 
USING_NS_CC;
#include "UserData.h"

const int RecorderNum = 9;
struct RecorderContent
{
	std::string xmlName;
	std::string showName;
};
const RecorderContent s_recorderContent[RecorderNum] = {
	{"High_Level", "High Level: %d"},
	{"High_Get_Money", "High Get Money: %d"},
	{"High_Open_Gold", "High Open Gold: %d"},
	{"High_Open_Dead", "High Open Dead: %d"},
	{"", ""},
	{"All_Open_Times", "All Open Times: %d"},
	{"All_Get_Money", "All Get Money: %d"},
	{"All_Open_Gold", "All Open Gold: %d"},
	{"All_Open_Dead", "All Open Dead: %d"},
};

class RecordLayer : public CCLayerColor
{
public:
	~RecordLayer(void);
	virtual bool init();
	CREATE_FUNC(RecordLayer);
public:
	void showRecording();
	void back(CCObject *pSender);
private:
	void setUserDataWords(const char* xmlName, const char* showName);
private:
	CCArray* m_records;
};

#endif