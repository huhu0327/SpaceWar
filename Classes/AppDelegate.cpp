#include "AppDelegate.h"
#include "MenuScene.h"

USING_NS_CC;
#pragma comment(linker,"/entry:wWinMainCRTStartup /subsystem:console")

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
        glview = GLViewImpl::createWithRect("SpaceWar", Rect(0, 0, 576.0f, 1024.0f));
        director->setOpenGLView(glview);
    }

	glview->setDesignResolutionSize(576.0f, 1024.0f, ResolutionPolicy::FIXED_HEIGHT);

	// turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto menuScene = MenuScene::createScene();

	director->pushScene(menuScene);

    // run
    //director->runWithScene(menuScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

}
