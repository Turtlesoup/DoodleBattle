#include <cmath>
#include <io.h>
#include <fcntl.h>
#include "BaseApplication.h"
#include "AppMacros.h"
#include "Screen.h"
#include "ScreenManager.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace std;

CCPoint BaseApplication::origin;
CCSize BaseApplication::visibleSize;
unsigned int BaseApplication::screenWidth;
unsigned int BaseApplication::screenHeight;
float BaseApplication::screenScaleFactorFrom768x1024;

BaseApplication::BaseApplication()
{
	//openConsole();
}

BaseApplication::~BaseApplication() 
{
}

bool BaseApplication::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);

    // Set the design resolution
    pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionNoBorder);

	BaseApplication::origin = CCDirector::sharedDirector()->getVisibleOrigin();
	BaseApplication::visibleSize = CCDirector::sharedDirector()->getVisibleSize();

	CCSize frameSize = pEGLView->getFrameSize();

	BaseApplication::screenWidth = frameSize.width;
	BaseApplication::screenHeight = frameSize.height;
	BaseApplication::screenScaleFactorFrom768x1024 = max((float)screenWidth/768, (float)screenHeight/1024);
    
    // if the frame's height is larger than the height of medium resource size, select large resource.
	if (frameSize.height > mediumResource.size.height)
	{
        pDirector->setContentScaleFactor(MIN(largeResource.size.height/designResolutionSize.height, largeResource.size.width/designResolutionSize.width));
	}
    // if the frame's height is larger than the height of small resource size, select medium resource.
    else if (frameSize.height > smallResource.size.height)
    {
        pDirector->setContentScaleFactor(MIN(mediumResource.size.height/designResolutionSize.height, mediumResource.size.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium resource size, select small resource.
	else
    {
        pDirector->setContentScaleFactor(MIN(smallResource.size.height/designResolutionSize.height, smallResource.size.width/designResolutionSize.width));
    }
	
	//preload the sound effects
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("audio/successSound.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("audio/cat.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("audio/click.wav");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("audio/lifeLost.wav");

    // turn on display FPS
    //pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    CCScene *pScene = initialise();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void BaseApplication::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void BaseApplication::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
   CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

cocos2d::CCScene* BaseApplication::initialise()
{
	CCScene *scene = ScreenManager::getInstance()->getCurrentScene();
	Screen *screen = ScreenManager::getInstance()->createScreen(ScreenManager::TITLE_SCREEN);
    scene->addChild(screen);

    return scene;
}



//open the console on windows for debugging
void BaseApplication::openConsole()
{
    int outHandle, errHandle, inHandle;
    FILE *outFile, *errFile, *inFile;
    AllocConsole();
    CONSOLE_SCREEN_BUFFER_INFO coninfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
    coninfo.dwSize.Y = 9999;
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

    outHandle = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
    errHandle = _open_osfhandle((long)GetStdHandle(STD_ERROR_HANDLE),_O_TEXT);
    inHandle = _open_osfhandle((long)GetStdHandle(STD_INPUT_HANDLE),_O_TEXT );

    outFile = _fdopen(outHandle, "w" );
    errFile = _fdopen(errHandle, "w");
    inFile =  _fdopen(inHandle, "r");

    *stdout = *outFile;
    *stderr = *errFile;
    *stdin = *inFile;

    setvbuf( stdout, NULL, _IONBF, 0 );
    setvbuf( stderr, NULL, _IONBF, 0 );
    setvbuf( stdin, NULL, _IONBF, 0 );

    std::ios::sync_with_stdio();
}