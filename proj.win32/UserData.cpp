#include "UserData.h"
#include "MySound.h"
#include "SceneManager.h"

GameProcess UserData::m_process;
GameSkill UserData::m_skill;

UserData::UserData(void)
{
}

UserData::~UserData(void)
{
}

void UserData::init()
{
	if (!CCGetBool("_IS_EXISTED"))   
	{
		CCSetBool("_IS_EXISTED", true);  
	}  
}

void UserData::newLevel()
{
	m_process.win = false;
	m_process.lose = false;
	m_process.level++;
	game_process.selectProp = -1;
}

void UserData::levelComplete()
{
	m_process.step+=3;
	m_process.money+=m_process.level;
	m_process.win = true;
	m_process.state = GAME_STATE_WIN;
}

void UserData::gameOver()
{
	m_process.lose = true;
	m_process.state = GAME_STATE_LOSE;
	saveProcess();
}

void UserData::startGame()
{
	load();

	m_process.firstPlay = CCGetBool("firstPlay", true);
	m_process.level = 1;
	m_process.win = false;
	m_process.lose = false;
	m_process.state = m_process.firstPlay ? GAME_STATE_FIRSTPLAY : GAME_STATE_NORMAL;

	m_process.getMoney = 0;
	m_process.openGoldNum = 0;
	m_process.openDeadNum = 0;

	m_process.step = (*s_skill_table[SKILL_ORIGINAL_STEP].getValue)(m_skill.skillLevel[SKILL_ORIGINAL_STEP]);
	for(int i = 0; i < PROP_NUM; i++)
	{
		m_process.propCells[i].num = 0;
	}
	game_process.selectProp = -1;

	//m_process.backColor = ccc3(0,0,0);
	m_process.backColor = ccc3(rand()%256,rand()%256,rand()%256);

	loadLottery();

}

void UserData::loadLottery()
{
	switch(m_process.lottery)
	{
	case LOTTERY_NONE:
		return;
	case LOTTERY_BOMB:
		{
			m_process.propCells[0].num = 1;
			m_process.propCells[0].type = BLOCK_BOMB;
			break;
		}
	case LOTTERY_FLYSHOE:
		m_process.propCells[0].num = 1;
		m_process.propCells[0].type = BLOCK_FLYSHOE;
		break;
	case LOTTERY_STEP:
		m_process.step += 3;
		break;
	case LOTTERY_MAGNIFIER:
		m_process.propCells[0].num = 1;
		m_process.propCells[0].type = BLOCK_MAGNIFIER;
		break;
	case LOTTERY_FLASHLIGHT:
		m_process.propCells[0].num = 1;
		m_process.propCells[0].type = BLOCK_FLASHLIGHT;
		break;		
	case LOTTERY_SWEEP:
		m_process.propCells[0].num = 1;
		m_process.propCells[0].type = BLOCK_SWEEP;
		break;
	case LOTTERY_EYE:
		m_process.propCells[0].num = 1;
		m_process.propCells[0].type = BLOCK_EYE;
		break;
	default:
		break;
	}
}

void UserData::load()
{
	loadSkill();
	loadProcess();
}
void UserData::save()
{
	saveSkill();
	saveProcess();
}

void UserData::loadProcess()
{
	m_process.money = CCGetInt("Money");
	m_process.gold = CCGetInt("Gold");
	for(int i = 0; i < BLOCK_TYPE_NUM; i++)
	{
		char buf[10];
		sprintf(buf, "found%d", i);
		m_process.isBlockFound[i] = CCGetBool(buf, false);
	}
}

void UserData::saveProcess()
{
	CCSetInt("Money", m_process.money);
	CCSetInt("Gold", m_process.gold);
	
	//high user data...
	int level = CCGetInt("High_Level");
	if(m_process.level > level)
		CCSetInt("High_Level", m_process.level);

	//high money
	int highGetMoney= CCGetInt("High_Get_Money");
	if(m_process.getMoney > highGetMoney)
		CCSetInt("High_Get_Money", m_process.getMoney);
	//high gold
	int highOpenGold= CCGetInt("High_Open_Gold");
	if(m_process.openGoldNum > highOpenGold)
		CCSetInt("High_Open_Gold", m_process.openGoldNum);
	//high dead
	int highOpenDead= CCGetInt("High_Open_Dead");
	if(m_process.openDeadNum > highOpenDead)
		CCSetInt("High_Open_Dead", m_process.openDeadNum);

	//total user data...
	int allGetMoney = CCGetInt("All_Get_Money");
	CCSetInt("All_Get_Money", allGetMoney + m_process.getMoney);
	int allOpenGold = CCGetInt("All_Open_Gold");
	CCSetInt("All_Open_Gold", allOpenGold + m_process.openGoldNum);
	int allOpenDead = CCGetInt("All_Open_Dead");
	CCSetInt("All_Open_Dead", allOpenDead + m_process.openDeadNum);
	int allOpenTimes = CCGetInt("All_Open_Times");
	CCSetInt("All_Open_Times", allOpenTimes + m_process.openTimes);
	
	if(m_process.firstPlay)
	{
		m_process.firstPlay = false;
		CCSetBool("firstPlay", false);
	}

	CCUserDefault::sharedUserDefault()->flush();
}

void UserData::loadSkill()
{
	init();
	char buf[10];
	for(int i = 0; i < (int)SKILL_NUM; i++)
	{
		sprintf(buf, "skill%d", (Skill_Type)i);
		m_skill.skillLevel[i] = CCGetInt(buf);
		if(m_skill.skillLevel[i]<0 || m_skill.skillLevel[i]>m_skill.getMaxLevel((Skill_Type)i))
			m_skill.skillLevel[i] = 0;
	}
}

void UserData::saveSkill()
{
	char buf[10];
	for(int i = 0; i < (int)SKILL_NUM; i++)
	{
		sprintf(buf, "skill%d", (Skill_Type)i);
		CCSetInt(buf, m_skill.skillLevel[i]);
	}
	CCUserDefault::sharedUserDefault()->flush();
}

void UserData::saveFoundNewBlockThing(BLOCK_TYPE type)
{
	char buf[10];
	sprintf(buf, "found%d", (int)type);	
	CCSetBool(buf, true);
	CCUserDefault::sharedUserDefault()->flush();
}