#include "AppDelegate.h"
#include "GameScene.h"
#include "AdsIntergrate.h"
USING_NS_CC;
#include "LoadingScene.h"

AppDelegate::AppDelegate() {
    // banner
//    App ID: ca-app-pub-2955926447585960~2663639339
//    Ad unit ID: ca-app-pub-2955926447585960/4140372538
    
    //full screen
//    App ID: ca-app-pub-2955926447585960~2663639339
//    Ad unit ID: ca-app-pub-2955926447585960/7093838937

}

AppDelegate::~AppDelegate() 
{
	SimpleAudioEngine::getInstance()->end();
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {


    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("Downward");
        director->setOpenGLView(glview);
    }

	glview->setFrameZoomFactor(0.95f);

//	glview->setFrameSize(448,795);
	glview->setDesignResolutionSize(640,1136,ResolutionPolicy::SHOW_ALL);


    // turn on display FPS
    director->setDisplayStats(false);

	// init game data
	//initGameData();

	// ��ʼ��UMͳ��
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = LoadingScene::create();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
    // if you use SimpleAudioEngine, it must be pause
   // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    //SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

void AppDelegate::initGameData()
{
    
	
	
}