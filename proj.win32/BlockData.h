#ifndef __BLOCK_DATA_H__
#define __BLOCK_DATA_H__   
#include "cocos2d.h" 
USING_NS_CC;
#include "UserData.h"

class OneBlock
{
public:
	OneBlock() : m_isOpen(false), m_openByUser(true), m_type(BLOCK_NONE), m_goldNum(0)
	{
	}
	void setType(BLOCK_TYPE type) { m_type = type; }
	BLOCK_TYPE getType() { return m_type; }
	void open(bool openByUser = true) { m_isOpen = true; m_openByUser = openByUser; }
	bool isOpen() { return m_isOpen;}
	bool isOpenByUser() { return m_openByUser; }
	int getNum() { return m_goldNum; }

public:
	bool m_isOpen;
	bool m_openByUser;
	BLOCK_TYPE m_type;
	int m_goldNum;
};

class BlockLayerData
{
public:
	BlockLayerData(int level);
	~BlockLayerData(void);
public:
	void setBlockThings();
	void getDeadNum(int level, int& dead1, int& dead2, int& dead3);
	void setRandomData(BLOCK_TYPE type, int num);
	void setNumForNone();
	void addGoldNum(int num);
	void openBlock(int tag, bool byUser = true);
	bool isBlockOpen(int tag) { return m_blocks[tag].isOpen(); }
	bool isBlockOpenByUser(int tag) { return m_blocks[tag].isOpenByUser(); }
	int getBlockNum(int tag) { return m_blocks[tag].getNum(); }
	BLOCK_TYPE getBlockType(int tag) { return m_blocks[tag].getType(); }
	void setAllOpen();
public:
	OneBlock m_blocks[BLOCK_NUM];
	int m_noneNum;
	int m_Gold;
	int m_openGoldNum;
	bool m_over;
	int m_level;
};

#endif