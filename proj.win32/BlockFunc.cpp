#include "BlockLayer.h"
#include "PlayLayer.h"
#include "MySound.h"

void BlockLayer::callBackRealOpen(CCObject* obj)
{
	CCSprite* sprite = (CCSprite*)obj;
	int tag = sprite->getTag() - 1000;
	((CCSprite*)obj)->removeFromParentAndCleanup(true);//remove open animation...	
	m_batchNode->getChildByTag(tag)->removeFromParentAndCleanup(true);//remove block cover...
	
	showGetting(tag);//show value change...

	PlayLayer* playLayer = (PlayLayer*)getParent();
	sprite = getBlockThing(tag);
	if(sprite)
		sprite->setOpacity(255);
	playLayer->gameUpdate();
}

void BlockLayer::bigAndSmallThing(int tag)
{
	CCSprite* sprite = getBlockThing(tag);
	if(!sprite)
		return;
	CCPoint p = sprite->getPosition();
	sprite->setAnchorPoint(ccp(0.5,0.5));
	sprite->setPosition(ccp(p.x+BLOCK_UNIT*0.5,p.y+BLOCK_UNIT*0.5));
	CCActionInterval* seq = (CCActionInterval*)CCSequence::create(CCScaleTo::create(0.1f, 0.7f), CCScaleTo::create(0.1f, 1.0f), NULL);
	sprite->runAction(CCSequence::create(CCRepeat::create(seq, 2),
		CCCallFuncN::create(this, callfuncN_selector(BlockLayer::afterBigAndSmall)),
		NULL));
}
void BlockLayer::afterBigAndSmall(CCNode* node)
{
	CCSprite* sprite = (CCSprite*)node;
	int tag = sprite->getTag()-2000;
	if(tag>=0 && tag < BLOCK_NUM)
	{
		PlayLayer* playLayer = (PlayLayer*)getParent();
		//playLayer->foundNewBlockThing(getBlockTypeByTag(tag));
	}
}

void BlockLayer::moveThing(int tag)
{
	CCSprite* sprite = getBlockThing(tag);
	if(!sprite)
		return;
	CCPoint p = sprite->getPosition();
	sprite->setPosition(ccp(p.x-10,p.y));
	CCFiniteTimeAction* act = CCMoveBy::create(1.0f, ccp(20,0));
	CCActionInterval* seq = (CCActionInterval*)CCSequence::create(act, act->reverse(), NULL);
	sprite->runAction(CCRepeatForever::create(seq));
	m_moveTags.insert(tag);
}

void BlockLayer::callBackRemove(CCNode* obj)
{
	CCSprite* sprite = (CCSprite*)obj;
	int tag = sprite->getTag() - 2000;
	sprite->removeFromParentAndCleanup(true);
	if(tag>=0 && tag < BLOCK_NUM)
	{
		PlayLayer* playLayer = (PlayLayer*)getParent();
		//playLayer->foundNewBlockThing(getBlockTypeByTag(tag));
	}
}

void BlockLayer::blinkAndRemoveThing(int tag)
{
	CCSprite* sprite = getBlockThing(tag);
	if(!sprite)
		return;
	int blinkTimes = 2;
	BLOCK_TYPE type = m_blockData->getBlockType(tag);
	if(type == BLOCK_TREASURE)
		blinkTimes = 3;
	CCFiniteTimeAction* actions=CCSequence::create(CCBlink::create(1.0f, blinkTimes),
		CCCallFuncN::create(this, callfuncN_selector(BlockLayer::callBackRemove)),
		NULL);
	sprite->runAction(actions);
}

void BlockLayer::lightBlockThing(CCSprite* blockThing)
{
	if(!blockThing)
		return;
	reorderChild(blockThing, LAYERNUM_BATCH+1);
	blockThing->setOpacity(180);
}

void BlockLayer::lightBlockAround(int tag)
{
	CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	CCArray *animFrames = CCArray::create();
	CCSpriteFrame* frame = cache->spriteFrameByName("light2.png");
    animFrames->addObject(frame);
	frame = cache->spriteFrameByName("light1.png");
    animFrames->addObject(frame);
	CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames,0.1f);
	CCAnimate* action = CCAnimate::create(animation);	
	CCActionInterval* seq = (CCActionInterval*)CCSequence::create(action, action, CCDelayTime::create(1.0f), NULL);
	
	CCSprite* sprite = getBlockThing(tag);
	sprite->runAction(CCRepeatForever::create(seq));

	CCSprite* blockThing = NULL;
	int row = tag/BLOCK_COL;
	int col = tag%BLOCK_COL;
	if(col > 0 && !m_blockData->m_blocks[tag-1].isOpen())//左
	{
		blockThing = getBlockThing(tag-1);
		lightBlockThing(blockThing);
	}
	if(col < BLOCK_COL-1 && !m_blockData->m_blocks[tag+1].isOpen())//右
	{
		blockThing = getBlockThing(tag+1);
		lightBlockThing(blockThing);
	}
	if(row > 0  && !m_blockData->m_blocks[tag-8].isOpen())//下
	{
		blockThing = getBlockThing(tag-8);
		lightBlockThing(blockThing);
	}
	if(row < BLOCK_ROW-1  && !m_blockData->m_blocks[tag+8].isOpen())//上
	{
		blockThing = getBlockThing(tag+8);
		lightBlockThing(blockThing);
	}
	if(col > 0 && row < BLOCK_ROW-1 && !m_blockData->m_blocks[tag+7].isOpen())//左上
	{
		blockThing = getBlockThing(tag+7);
		lightBlockThing(blockThing);
	}
	if(col > 0 && row > 0 && !m_blockData->m_blocks[tag-9].isOpen())//左下
	{
		blockThing = getBlockThing(tag-9);
		lightBlockThing(blockThing);
	}
	if(col < BLOCK_COL-1 && row < BLOCK_ROW-1 && !m_blockData->m_blocks[tag+9].isOpen())//右上
	{
		blockThing = getBlockThing(tag+9);
		lightBlockThing(blockThing);
	}
	if(col < BLOCK_COL-1 && row > 0 && !m_blockData->m_blocks[tag-7].isOpen())//右上
	{
		blockThing = getBlockThing(tag-7);
		lightBlockThing(blockThing);
	}
}

void BlockLayer::propOnBlock(int tag)
{
	if(game_process.selectProp<0)
		return;
	Prop_Cell& propCell = game_process.propCells[game_process.selectProp];
	if(propCell.num<1)
		return;
	PlayLayer* playLayer = (PlayLayer*)getParent();
	playLayer->clearPropMenu();
	BLOCK_TYPE type = propCell.type;
	switch(type)
	{
		case BLOCK_BOMB:
		{
			MySound::playSound(SOUND_BOMB);
			propBombOnBlock(tag);
			break;
		}
		case BLOCK_MAGNIFIER:
		{
			MySound::playSound(SOUND_FIND_PROP);
			propMagnifierOnBlock(tag);
			break;
		}
		default:
			break;
	}
	propCell.num--;
	game_process.selectProp = -1;
	if(game_process.state == GAME_STATE_PROP)
		game_process.state = GAME_STATE_NORMAL;
	playLayer->updatePropCells();
}

void BlockLayer::propBombOnBlock(int tag)
{
	if(!m_blockData->m_blocks[tag].isOpen())//点中的块
	{
		m_blockData->openBlock(tag, false);
		openAnimation(tag);
	}
	int row = tag/BLOCK_COL;
	int col = tag%BLOCK_COL;
	if(col > 0 && !m_blockData->m_blocks[tag-1].isOpen())//左
	{
		m_blockData->openBlock(tag-1, false);
		openAnimation(tag-1);
	}
	if(col < BLOCK_COL-1 && !m_blockData->m_blocks[tag+1].isOpen())//右
	{
		m_blockData->openBlock(tag+1, false);
		openAnimation(tag+1);
	}
	if(row > 0  && !m_blockData->m_blocks[tag-8].isOpen())//下
	{
		m_blockData->openBlock(tag-8, false);
		openAnimation(tag-8);
	}
	if(row < BLOCK_ROW-1  && !m_blockData->m_blocks[tag+8].isOpen())//上
	{
		m_blockData->openBlock(tag+8, false);
		openAnimation(tag+8);
	}
	if(game_skill.skillLevel[SKILL_BIG_BOMB] == 1)
	{
		if(col > 0 && row < BLOCK_ROW-1 && !m_blockData->m_blocks[tag+7].isOpen())//左上
		{
			m_blockData->openBlock(tag+7, false);
			openAnimation(tag+7);
		}
		if(col > 0 && row > 0 && !m_blockData->m_blocks[tag-9].isOpen())//左下
		{
			m_blockData->openBlock(tag-9, false);
			openAnimation(tag-9);
		}
		if(col < BLOCK_COL-1 && row < BLOCK_ROW-1 && !m_blockData->m_blocks[tag+9].isOpen())//右上
		{
			m_blockData->openBlock(tag+9, false);
			openAnimation(tag+9);
		}
		if(col < BLOCK_COL-1 && row > 0 && !m_blockData->m_blocks[tag-7].isOpen())//右上
		{
			m_blockData->openBlock(tag-7, false);
			openAnimation(tag-7);
		}
	}
}

void BlockLayer::propMagnifierOnBlock(int tag)
{
	CCSprite* blockThing = getBlockThing(tag);
	lightBlockThing(blockThing);

	int row = tag/BLOCK_COL;
	int col = tag%BLOCK_COL;
	if(col > 0 && !m_blockData->m_blocks[tag-1].isOpen())//左
	{
		blockThing = getBlockThing(tag-1);
		lightBlockThing(blockThing);
	}
	if(col < BLOCK_COL-1 && !m_blockData->m_blocks[tag+1].isOpen())//右
	{
		blockThing = getBlockThing(tag+1);
		lightBlockThing(blockThing);
	}
	if(row > 0  && !m_blockData->m_blocks[tag-8].isOpen())//下
	{
		blockThing = getBlockThing(tag-8);
		lightBlockThing(blockThing);
	}
	if(row < BLOCK_ROW-1  && !m_blockData->m_blocks[tag+8].isOpen())//上
	{
		blockThing = getBlockThing(tag+8);
		lightBlockThing(blockThing);
	}
	if(col > 0 && row < BLOCK_ROW-1 && !m_blockData->m_blocks[tag+7].isOpen())//左上
	{
		blockThing = getBlockThing(tag+7);
		lightBlockThing(blockThing);
	}
	if(col > 0 && row > 0 && !m_blockData->m_blocks[tag-9].isOpen())//左下
	{
		blockThing = getBlockThing(tag-9);
		lightBlockThing(blockThing);
	}
	if(col < BLOCK_COL-1 && row < BLOCK_ROW-1 && !m_blockData->m_blocks[tag+9].isOpen())//右上
	{
		blockThing = getBlockThing(tag+9);
		lightBlockThing(blockThing);
	}
	if(col < BLOCK_COL-1 && row > 0 && !m_blockData->m_blocks[tag-7].isOpen())//右上
	{
		blockThing = getBlockThing(tag-7);
		lightBlockThing(blockThing);
	}
}

void BlockLayer::openRandomGold()
{
	std::vector<int> hidenGolds;
	for(int i = 0; i < BLOCK_NUM; i++)
	{
		if(m_blockData->m_blocks[i].getType() == BLOCK_GOLD 
			&& !m_blockData->m_blocks[i].isOpen())
			hidenGolds.push_back(i);
	}
	int index = rand() % hidenGolds.size();
	m_blockData->openBlock(hidenGolds[index], false);
	openAnimation(hidenGolds[index]);
}

void BlockLayer::sweepBlankBlock()
{
	for(int i = 0; i < BLOCK_NUM; i++)
	{
		if(m_blockData->m_blocks[i].getType() == BLOCK_NONE
			&& !m_blockData->m_blocks[i].isOpen())
		{
			m_blockData->openBlock(i, false);
			openAnimation(i);
		}
	}
}

void BlockLayer::showAllDeadBlock()
{
	for(int i = 0; i < BLOCK_NUM; i++)
	{
		if(!m_blockData->m_blocks[i].isOpen() &&
			(m_blockData->m_blocks[i].getType() == BLOCK_DEAD1 ||
				m_blockData->m_blocks[i].getType() == BLOCK_DEAD2 ||
				m_blockData->m_blocks[i].getType() == BLOCK_DEAD3))
		{
			CCSprite* blockThing = getBlockThing(i);
			lightBlockThing(blockThing);
		}
	}
}

void BlockLayer::showSomeDeadBlock(int num)
{
	std::vector<int> unOpenDeadBlocks;
	for(int i = 0; i < BLOCK_NUM; i++)
	{
		if(!m_blockData->m_blocks[i].isOpen() &&
			(m_blockData->m_blocks[i].getType() == BLOCK_DEAD1 ||
				m_blockData->m_blocks[i].getType() == BLOCK_DEAD2 ||
				m_blockData->m_blocks[i].getType() == BLOCK_DEAD3))
		{
			unOpenDeadBlocks.push_back(i);
		}
	}
	int i = 0;
	while(i < num)
	{
		if(unOpenDeadBlocks.size() == 0)
			return;
		int index = rand()%unOpenDeadBlocks.size();
		CCSprite* blockThing = getBlockThing(unOpenDeadBlocks[index]);
		lightBlockThing(blockThing);
		
		std::vector<int>::iterator itr = unOpenDeadBlocks.begin();
		int c = 0;
		while(c < index && itr != unOpenDeadBlocks.end())
		{
			c++;
			itr++;
		}
		unOpenDeadBlocks.erase(itr);

		i++;
	}
}

void BlockLayer::showGetting(const CCPoint& p, char* picName, int num, int index)
{
	showGetting(this, p, picName, num, index);
}

void BlockLayer::showGetting(CCLayer* layer, const CCPoint& p, char* picName, int num, int index)
{
	//get block position...
	//show pic...
	CCNode* node = CCNode::create();
	CCSprite* getDisplay = CCSprite::create(picName);
	getDisplay->setScale(0.5);
	getDisplay->setAnchorPoint(ccp(0,0));
	getDisplay->setPosition(ccp(p.x+BLOCK_UNIT*0.2, p.y+BLOCK_UNIT*0.6+index*25));
	node->addChild(getDisplay);
	//show number...
	char buf[20];
	if(num != 0)
	{
		if(num>0)
			sprintf(buf, "+%d", num);
		else
			sprintf(buf, "-%d", -num);
		CCLabelTTF* getNum = CCLabelTTF::create(buf,"Marker Felt", 25.0f);
		getNum->setAnchorPoint(ccp(0,0));
		getNum->setPosition(ccp(p.x+BLOCK_UNIT*0.6, p.y+BLOCK_UNIT*0.6+index*25));
		if(num<0)
			getNum->setColor(ccc3(255,0,0));
		node->addChild(getNum);
	}
	else
		getDisplay->setPosition(ccp(p.x+27.5, p.y+BLOCK_UNIT*0.6+index*25));


	layer->addChild(node, LAYERNUM_SHOWGETTING);
	node->runAction(CCSequence::create(CCMoveBy::create(1.0f, ccp(0,BLOCK_UNIT/2)),
		CCCallFuncN::create(layer, callfuncN_selector(BlockLayer::callBackRemove)),
		NULL));
}

void BlockLayer::showGetting(int tag)
{
	bool byUser = m_blockData->isBlockOpenByUser(tag);
	if(byUser)
	{
		m_levelData.openTime++;
		game_process.openTimes++;
	}
	PlayLayer* playLayer = (PlayLayer*)getParent();
	CCSprite* blockThing = getBlockThing(tag);
	int row = tag / BLOCK_COL;
	int col = tag % BLOCK_COL;
	CCPoint p = ccp(col*BLOCK_UNIT, row*BLOCK_UNIT);
	BLOCK_TYPE type = m_blockData->getBlockType(tag);
	switch(type)
	{
	case BLOCK_GOLD:
		{
			m_blockData->m_openGoldNum++;
			if(m_blockData->m_openGoldNum == m_blockData->m_Gold)
			{
				game_process.state = GAME_STATE_WIN;
				CCLog("win...");
			}
			game_process.openGoldNum++;
			game_process.step+=1;

			int addMoney = game_skill.getValue(SKILL_MORE_MONEY);
			game_process.money+=addMoney;
			game_process.getMoney+=addMoney;

			game_process.gold+=1;
			if(addMoney>0)
				showGetting(p, PIC_SHOW_MONEY, addMoney, 2); 
			showGetting(p, PIC_SHOW_GOLD, 1, 1); 
			showGetting(p, PIC_SHOW_STEP, 1); 
			MySound::playSound(SOUND_MONEY);
			//playLayer->foundNewBlockThing(type);
			break;
		}
	case BLOCK_DEAD1:
	case BLOCK_DEAD2:
	case BLOCK_DEAD3:
		{
			int deadLevel = (int)type - (int)BLOCK_DEAD1;
			game_process.openDeadNum++;
			bool isDefense = rand()%100 < game_skill.skillLevel[SKILL_DEFENSE_SKULL]*10;
			if(isDefense)
			{
				showGetting(p, PIC_SKILL_DEFENSE, 0);
				MySound::playSound(SOUND_SKILL_DEFENSE);
			}
			else
			{
				int deadValue = 2+ deadLevel;
				game_process.step-=deadValue;
				showGetting(p, PIC_SHOW_STEP, -deadValue);
				bigAndSmallThing(tag);
				MySound::playSound(SOUND_DEAD);
			}
			break;
		}
	case BLOCK_CLOUD:
		{
			moveThing(tag);
			//playLayer->foundNewBlockThing(type);
			break;
		}
	case BLOCK_MONEY:
		{
			int m = rand()%3+1;
			game_process.money+=m;
			game_process.getMoney+=m;
			showGetting(p, PIC_SHOW_MONEY, m);
			blinkAndRemoveThing(tag);
			MySound::playSound(SOUND_MONEY);
			break;
		}
	case BLOCK_TREASURE:
		{
			int m = rand()%6+10;
			game_process.money+=m;
			game_process.getMoney+=m;
			showGetting(p, PIC_SHOW_MONEY, m); 
			blinkAndRemoveThing(tag);
			MySound::playSound(SOUND_MONEY);
			break;
		}
	case BLOCK_STEP:
		{
			int step = 1 + game_skill.skillLevel[SKILL_MORE_STEP];
			game_process.step+=step;
			showGetting(p, PIC_SHOW_STEP, step);
			blinkAndRemoveThing(tag);
			MySound::playSound(SOUND_FIND_PROP);
			break;
		}
	case BLOCK_BOMB:
	case BLOCK_FLYSHOE:
	case BLOCK_MAGNIFIER:
	case BLOCK_FLASHLIGHT:
	case BLOCK_SWEEP:
	case BLOCK_EYE:
		{
			if(playLayer->isPropCellFull(type))
				MySound::playSound(SOUND_PROP_FULL);
			else
				MySound::playSound(SOUND_FIND_PROP);
			CCSprite* prop = getBlockThing(tag);
			playLayer->getPropToPropPack(prop, type);
			break;
		}
	case BLOCK_LIGHT:
		{
			MySound::playSound(SOUND_FIND_PROP);
			lightBlockAround(tag);
			//playLayer->foundNewBlockThing(type);
			break;
		}
	default:
		{
			if(byUser)
			{
				game_process.step--;
				showGetting(p, PIC_SHOW_STEP, -1);
			}
			//playLayer->foundNewBlockThing(type);
			break;
		}
	}
	playLayer->foundNewBlockThing(type);
	if(game_process.step <= 0)
		game_process.state = GAME_STATE_LOSE;
}