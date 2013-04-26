#include "cocos2d.h"
#include "CCEGLView.h"
#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "SceneManager.h"
#include "UserData.h"

using namespace CocosDenshion;

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::end();
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());
	
	CCSize screenSize = CCEGLView::sharedOpenGLView()->getFrameSize();
	CCSize designSize = CCSizeMake(640, 960);
    pDirector->setContentScaleFactor(1);
	CCEGLView::sharedOpenGLView()->setDesignResolutionSize(designSize.width,designSize.height, kResolutionNoBorder);

    // turn on display FPS
    //pDirector->setDisplayStats(true);
    pDirector->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    //CCScene *pScene = HelloWorld::scene();

    // run
    //pDirector->runWithScene(pScene);
	
	srand((unsigned)time(0));

	SceneManager::initLoading();
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
	
	if(SceneManager::music)
		SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();

	if(SceneManager::music)
		SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();

	//CCSize winSize = CCDirector::sharedDirector()->getWinSize();  
    //Loading *loading = new Loading();  
    //CCDirector::sharedDirector()->getRunningScene()->addChild((CCNode *)loading);
}
