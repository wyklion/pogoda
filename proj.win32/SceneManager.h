#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "cocos2d.h"
USING_NS_CC;

class SceneManager : public CCObject
{
public:
	SceneManager(void);
	~SceneManager(void);
	
    static void initLoading();
    static void goMenu(int transType = 0);
	static void goFirst();
	static void goPlay();
	static void goSkill(int transType = 0);
	static void goRecording(int transType = 0);
	static void goStop();
	static void goAdmin();
    static void go(CCLayer* layer, int transType = 0);
    static void push(CCLayer* layer);  
	static void pop();
    static CCScene* wrap(CCLayer* layer);      
	
    //������̬���   
    static CCTransitionScene* createTransition(CCScene* s, int index = 0);  
    //�������� CCTransitionScene ���󣬾��忴.cpp�е�ʵ��
public:
	static bool music;
	static bool sound;
};

#endif