#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "LHPauseScene.h"
#include "SimpleAudioEngine.h"
#include "LocalizedString.h"
#include "LHMacros.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{

}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create(LHLocalizedCString("appname"));
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = HelloWorld::createScene();

    // run
    director->runWithScene(scene);
    
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("click.wav");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("chime.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("pop.wav");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("ef.wav");

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    
    int sceneTag = Director::getInstance()->getRunningScene()->getTag();
    if (sceneTag > 0) {
        Director::getInstance()->pushScene(LHPauseScene::createScene(nullptr));
    }
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
