#ifndef __BASIC_H__
#define __BASIC_H__   

#define CCSetString CCUserDefault::sharedUserDefault()->setStringForKey  
  
#define CCSetInt CCUserDefault::sharedUserDefault()->setIntegerForKey  
  
#define CCSetBool CCUserDefault::sharedUserDefault()->setBoolForKey  
  
#define CCGetString CCUserDefault::sharedUserDefault()->getStringForKey  
  
#define CCGetInt CCUserDefault::sharedUserDefault()->getIntegerForKey  
  
#define CCGetBool CCUserDefault::sharedUserDefault()->getBoolForKey  

#define PIC_MAIN_BACKGROUND "main_background.png"
#define PIC_TPLIST "pic.plist"
#define PIC_BUTTON "button1.png"
#define PIC_BUTTONHILIT "buttonHilit1.png"
#define PIC_ROUND "round.png"
#define PIC_LOTTERY "lottery1.png"
#define PIC_LOTTERY_SMILE "smileFace.png"

#define PIC_BLOCK "block1.png"
#define PIC_BLOCK_HILIT "block2.png"
#define PIC_BLOCK_BACKGROUND "block_background.png"
#define PIC_BLOCK_GOLD "gold1.png"
#define PIC_BLOCK_DEAD1 "dead1.png"
#define PIC_BLOCK_DEAD2 "dead2.png"
#define PIC_BLOCK_DEAD3 "dead3.png"
#define PIC_BLOCK_BOMB "bomb1.png"
#define PIC_BLOCK_LIGHT "light1.png"
#define PIC_BLOCK_CLOUD "cloud1.png"
#define PIC_BLOCK_MONEY "money1.png"
#define PIC_BLOCK_TREASURE "treasure1.png"
#define PIC_BLOCK_FLYSHOE "flyshoe1.png"
#define PIC_BLOCK_STEP "propStep1.png"
#define PIC_BLOCK_MAGNIFIER "magnifier1.png"
#define PIC_BLOCK_FLASHLIGHT "propFlashlight1.png"
#define PIC_BLOCK_SWEEP "propSweep1.png"
#define PIC_BLOCK_EYE "propEye1.png"

#define PIC_BUTTON_BACK "backButton.png"
#define PIC_BUTTON_BACK_SELECTED "backButtonSelected.png"
#define PIC_STOPBUTTON "stopButton.png"
#define PIC_STOPBUTTON_SELECTED "stopButtonSelected.png"
#define PIC_MUSIC_ON "buttonMusic1.png"
#define PIC_MUSIC_OFF "buttonMusic2.png"
#define PIC_SOUND_ON "buttonSound1.png"
#define PIC_SOUND_OFF "buttonSound2.png"
#define PIC_RESUME "resumeButton.png"
#define PIC_RESUME_SELECTED "resumeButtonSelected.png"
#define PIC_QUIT "quitButton.png"
#define PIC_QUIT_SELECTED "quitButtonSelected.png"

#define PIC_PROP_BACKGROUND "prop_background.png"
#define PIC_PROP_DO "propDo1.png"
#define PIC_PROP_CANCEL "propCancel1.png"
#define PIC_PROP_SELL "propSell1.png"
#define PIC_PROP_INFOBACK "propInfoBack1.png"

#define PIC_SHOW_LEVEL "showLevel.png"
#define PIC_SHOW_STEP "showStep.png"
#define PIC_SHOW_MONEY "showMoney.png"
#define PIC_SHOW_GOLD "showGold.png"

#define PIC_SKILL_ORIGINALSTEP "skillOriginalStep1.png"
#define PIC_SKILL_STEP "skillStep1.png"
#define PIC_SKILL_BIGBOMB "skillBigBomb1.png"
#define PIC_SKILL_DEFENSE "defense1.png"
#define PIC_SKILL_MOREMONEY "skillMoreMoney1.png"
#define PIC_SKILL_EYE "skillEye1.png"

#define LAYERNUM_BACKGROUND 0
#define LAYNUM_THING 5
#define LAYERNUM_BATCH 10
#define LAYERNUM_HILIT 15
#define LAYERNUM_PROP 10
#define LAYERNUM_PROP_TOP 20
#define LAYERNUM_SHOWGETTING 30

#define BLOCK_UNIT 80

#define BLOCK_ROW 8
#define BLOCK_COL 8
#define BLOCK_NUM 64


#define PROP_NUM  4

#define game_process UserData::m_process
#define game_skill UserData::m_skill

enum GAME_STATE
{
	GAME_STATE_NORMAL,
	GAME_STATE_WIN,
	GAME_STATE_LOSE,
	GAME_STATE_PROP,
	GAME_STATE_STOP,
	GAME_STATE_FOUND,
	GAME_STATE_FIRSTPLAY,
	GAME_STATE_NEWTHING,
};

enum BLOCK_TYPE
{
	BLOCK_NONE,
	BLOCK_NUMBER,
	BLOCK_GOLD,
	BLOCK_DEAD1,
	BLOCK_DEAD2,
	BLOCK_DEAD3,
	BLOCK_CLOUD,
	BLOCK_BOMB,
	BLOCK_LIGHT,
	BLOCK_MONEY,
	BLOCK_TREASURE,
	BLOCK_FLYSHOE,
	BLOCK_STEP,
	BLOCK_MAGNIFIER,
	BLOCK_FLASHLIGHT,
	BLOCK_SWEEP,
	BLOCK_EYE,

	BLOCK_TYPE_NUM,
};

struct Block_Content
{
	std::string picName;
	bool isCollect;
	int startLevel;
	int leastNum;
	int randNum;
	std::string describe;
};

const Block_Content s_blockContent[BLOCK_TYPE_NUM] = {
	{PIC_BLOCK_BACKGROUND,	0,	0,	0,	0,	"There's nothing, so you\n know there's no gold\n around it"},
	{"",					0,	0,	0,	0,	"The number told you\n how many golds around it"},
	{PIC_BLOCK_GOLD,		0,	1,	0,	0,	"You need to find all\n the other gold to\n complete this level"},
	{PIC_BLOCK_DEAD1,		0,	1,	0,	0,	"It will deduct 2 step"},
	{PIC_BLOCK_DEAD2,		0,	2,	0,	0,	"It will deduct 3 steps"},
	{PIC_BLOCK_DEAD3,		0,	6,	0,	0,	"It will deduct 4 steps"},
	{PIC_BLOCK_CLOUD,		0,	0,	0,	0,	"It's just a cloud...\nbut it may cover the\n number."},
	{PIC_BLOCK_BOMB,		1,	1,	2,	3,	"It can explode on a cross"},
	{PIC_BLOCK_LIGHT,		0,	5,	2,	3,	"It will show you cells\n around"},
	{PIC_BLOCK_MONEY,		0,	4,	2,	3,	"You can get some money\n from it"},
	{PIC_BLOCK_TREASURE,	0,	8,	0,	2,	"It's a lot of money"},
	{PIC_BLOCK_FLYSHOE,		1,	15,	0,	2,	"You can fly to next\n level by it"},
	{PIC_BLOCK_STEP,		0,	2,	2,	3,	"It will add 2 step"},
	{PIC_BLOCK_MAGNIFIER,	1,	11, 0,	2,	"It can show you 9 cells\n content"},
	{PIC_BLOCK_FLASHLIGHT,	1,	13,	0,	2,	"It can open a random gold"},
	{PIC_BLOCK_SWEEP,		1,	18,	0,	2,	"It can sweep all empty cells"},
	{PIC_BLOCK_EYE,			1,	10,	0,	2,	"It can show you\n random 3 dead cells"},
};

const bool s_blockthing_isCollect[BLOCK_TYPE_NUM] = {
	0,0,0,0,1,0,0,0,0,1,0,1,1,1,1
};
const char* const s_blockthing_pic_name[BLOCK_TYPE_NUM] = {
	"",
	"",
	PIC_BLOCK_GOLD,
	PIC_BLOCK_DEAD1,
	PIC_BLOCK_DEAD2,
	PIC_BLOCK_DEAD3,
	PIC_BLOCK_BOMB,
	PIC_BLOCK_LIGHT,
	PIC_BLOCK_CLOUD,
	PIC_BLOCK_MONEY,
	PIC_BLOCK_TREASURE,
	PIC_BLOCK_FLYSHOE,
	PIC_BLOCK_STEP,
	PIC_BLOCK_MAGNIFIER,
	PIC_BLOCK_FLASHLIGHT,
	PIC_BLOCK_SWEEP,
	PIC_BLOCK_EYE
};

struct Prop_Cell
{
	int num;
	BLOCK_TYPE type;
};

enum Lottery_Type
{
	LOTTERY_NONE,
	LOTTERY_TREASURE,
	LOTTERY_BOMB,
	LOTTERY_FLYSHOE,
	LOTTERY_STEP,
	LOTTERY_MAGNIFIER,
	LOTTERY_FLASHLIGHT,
	LOTTERY_SWEEP,
	LOTTERY_EYE,

	LOTTERY_NUM
};

const char* const s_lottery_pic[LOTTERY_NUM] = {
	PIC_LOTTERY_SMILE,
	PIC_BLOCK_TREASURE,
	PIC_BLOCK_BOMB,
	PIC_BLOCK_FLYSHOE,
	PIC_BLOCK_STEP,
	PIC_BLOCK_MAGNIFIER,
	PIC_BLOCK_FLASHLIGHT,
	PIC_BLOCK_SWEEP,
	PIC_BLOCK_EYE
};

struct GameProcess
{
	GameProcess()
	{
		lottery = LOTTERY_NONE;
	}
	
	bool firstPlay;
	int level;
	int step;
	int money;
	int gold;
	int openGoldNum;
	int openDeadNum;
	int getMoney;
	int openTimes;
	bool win;
	bool lose;
	GAME_STATE state;
	Prop_Cell propCells[PROP_NUM];
	int selectProp;
	ccColor3B backColor;
	Lottery_Type lottery;
	bool isBlockFound[BLOCK_TYPE_NUM];
};

enum Skill_Type
{
	SKILL_ORIGINAL_STEP,
	SKILL_MORE_STEP,
	SKILL_BIG_BOMB,
	SKILL_DEFENSE_SKULL,
	SKILL_MORE_MONEY,
	SKILL_EVIL_EYE,

	SKILL_NUM
};

class SkillPayAndValue
{
public:
	static int getPay1(int level);
	static int getPayForBigBomb(int level);
	
	static int getValueMoreMoney(int level);
	static int getValueForOriginalStep(int level);
	static int getValueMoreEye(int level);	
};

struct SkillContent
{
	BLOCK_TYPE requireFound;
	std::string picName;
	std::string name;
	std::string describe;
	int maxLevel;
	int (*getPay)(int level);
	int (*getValue)(int level);
};

const SkillContent s_skill_table[SKILL_NUM] = {
	{BLOCK_NONE,	PIC_SKILL_ORIGINALSTEP,	"Original shovel",	"Original shovel",	5, SkillPayAndValue::getPay1,			SkillPayAndValue::getValueForOriginalStep},
	{BLOCK_STEP,	PIC_SKILL_STEP,			"More shovel",		"More shovel",		5, SkillPayAndValue::getPay1,			NULL},
	{BLOCK_BOMB,	PIC_SKILL_BIGBOMB,			"Big bomb",			"Big bomb",			1, SkillPayAndValue::getPayForBigBomb,	NULL},
	{BLOCK_DEAD1,	PIC_SKILL_DEFENSE,			"Skull defense",	"Skull defense",	5, SkillPayAndValue::getPay1,			NULL},
	{BLOCK_GOLD,	PIC_SKILL_MOREMONEY,		"More money",		"More money",		5, SkillPayAndValue::getPay1,			SkillPayAndValue::getValueMoreMoney},
	{BLOCK_EYE,		PIC_SKILL_EYE,				"Evil eye",			"Evil eye",			5, SkillPayAndValue::getPay1,			SkillPayAndValue::getValueMoreEye},
};

struct GameSkill
{
public:
	const char* getPicName(Skill_Type type)
	{
		return s_skill_table[type].picName.c_str();
	}
	const char* getName(Skill_Type type)
	{
		return s_skill_table[type].name.c_str();
	}
	const char* getDescribe(Skill_Type type)
	{
		return s_skill_table[type].describe.c_str();
	}
	int getMaxLevel(Skill_Type type)
	{
		return s_skill_table[type].maxLevel;
	}
	int getPay(Skill_Type type)
	{
		return (*s_skill_table[type].getPay)(skillLevel[type]);
	}
	int getValue(Skill_Type type)
	{
		return (*s_skill_table[type].getValue)(skillLevel[type]);
	}
public:
	int skillLevel[SKILL_NUM];
};



#endif