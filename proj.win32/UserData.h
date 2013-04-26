#ifndef __USER_DATA_H__
#define __USER_DATA_H__   
#include "cocos2d.h" 
USING_NS_CC;
#include "basic.h"

class UserData
{
public:
	UserData(void);
	~UserData(void);
public:
	static void init();

	static void load();
	static void save();
	static void loadProcess();
	static void saveProcess();
	static void loadSkill();
	static void saveSkill();
	
	static void startGame();
	static void newLevel();
	static void levelComplete();
	static void gameOver();
	
	static void loadLottery();

	static void saveFoundNewBlockThing(BLOCK_TYPE type);

public:
	static GameProcess m_process;
	static GameSkill m_skill;
};


#endif