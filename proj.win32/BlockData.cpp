#include "BlockData.h"
#include "UserData.h"

BlockLayerData::BlockLayerData(int level)
{
	m_level = level;
	m_noneNum = BLOCK_NUM;
	m_Gold = 5;//4 + rand()%3;
	m_openGoldNum = 0;

	setRandomData(BLOCK_GOLD, m_Gold);
	
	int dead1;
	int dead2;
	int dead3;
	getDeadNum(level, dead1, dead2, dead3);
	setRandomData(BLOCK_DEAD1, dead1);
	setRandomData(BLOCK_DEAD2, dead2);
	setRandomData(BLOCK_DEAD3, dead3);
	
	int randNum = rand()%3+2;
	setRandomData(BLOCK_CLOUD, randNum);
	setNumForNone();
	
	setBlockThings();
}

BlockLayerData::~BlockLayerData(void)
{
}

void BlockLayerData::setBlockThings()
{
	int thingNum = BLOCK_TYPE_NUM - BLOCK_BOMB;

	for(int i = (int)BLOCK_BOMB; i < (int)BLOCK_TYPE_NUM; i++)
	{
		if(!game_process.isBlockFound[i] && m_level < s_blockContent[i].startLevel)
			continue;
		int randNum = s_blockContent[i].leastNum + rand() % s_blockContent[i].randNum;
		setRandomData((BLOCK_TYPE)i, randNum);
	}
}

void BlockLayerData::getDeadNum(int level, int& dead1, int& dead2, int& dead3)
{
	int allDead = 5 + level/10;;
	dead1 = 0;
	dead2 = 0;
	dead3 = 0;
	
	if(level <=5)
	{
		dead2 = rand()%level;
		dead1 = allDead - dead2;
	}
	else if(level > 5 && level < 25)
	{
		dead3 = rand()%(level/5)+1;
		allDead -= dead3;
		if(allDead < 1)
			return;
		dead2 = rand()%allDead;
		dead1 = allDead - dead2;
	}
	else if(level>=25)
	{
		dead3 = 2+(level-25)/10+ rand()%((level-25)/10+1);
		dead2 = allDead - dead3;
	}	 
}

void BlockLayerData::setRandomData(BLOCK_TYPE type, int num)
{
	int count = 0;
	while(count < num)
	{
		if(m_noneNum == 0)
			return;
		int randIndex = rand() % m_noneNum;
		int i = -1;
		int index = -1;
		while(index < randIndex)
		{
			i++;
			if(m_blocks[i].getType() == BLOCK_NONE)
				index++;
		}
		m_blocks[i].setType(type);
		m_noneNum--;
		count++;
	}
}

void BlockLayerData::addGoldNum(int num)
{
	if(num >= 0 && num < BLOCK_NUM)
	{
		BLOCK_TYPE type = m_blocks[num].getType();
		if(type != BLOCK_NONE)
			return;
		m_blocks[num].m_goldNum++;
	}
}

void BlockLayerData::setNumForNone()
{
	for(int i = 0; i < BLOCK_NUM; i++)
	{
		int row = i/BLOCK_COL;
		int col = i%BLOCK_COL;
		BLOCK_TYPE type = m_blocks[i].getType();
		if(type != BLOCK_GOLD)
			continue;
		if(col > 0)
			addGoldNum(i-1);//左
		if(col < BLOCK_COL-1)
			addGoldNum(i+1);//右
		if(row > 0)
			addGoldNum(i-8);//下
		if(row < BLOCK_ROW-1)
			addGoldNum(i+8);//上
		if(col > 0 && row > 0)
			addGoldNum(i-9);//左下
		if(col < BLOCK_COL-1 && row > 0)
			addGoldNum(i-7);//右下
		if(col > 0 && row < BLOCK_ROW-1)
			addGoldNum(i+7);//左上
		if(col < BLOCK_COL-1 && row < BLOCK_ROW-1)
			addGoldNum(i+9);//右上
	}
	
	for(int i = 0; i < BLOCK_NUM; i++)
	{
		if(m_blocks[i].getNum()>0)
		{
			m_blocks[i].setType(BLOCK_NUMBER);
			m_noneNum--;
		}
	}
}

void BlockLayerData::openBlock(int tag, bool byUser)
{
	m_blocks[tag].open(byUser);
}

void BlockLayerData::setAllOpen()
{
	for(int i = 0; i < BLOCK_NUM; i++)
	{
		m_blocks[i].open();
	}
}