#include "TitleScreen.h"
#include "BaseApplication.h"
#include "SimpleAudioEngine.h"

TitleScreen::TitleScreen(ScreenData* screenData) : Screen(screenData)
{
}

TitleScreen::~TitleScreen()
{
	ScreenManager::getInstance()->getSpriteBatchManager()->removeSpriteBatch("gameSprites");
}

bool TitleScreen::initialise()
{
	//super init
	if(!Screen::initialise())
	{
		return false;
	}

	setTouchEnabled(true);

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	float screenHalfWidth = visibleSize.width*0.5;
	float screenHalfHeight = visibleSize.height*0.5;
	CCPoint origin = BaseApplication::origin;

	//create and add a sprite batch to the sprite batch manager
	_spriteBatch = ScreenManager::getInstance()->getSpriteBatchManager()->createSpriteBatch("gameSprites", "assetsList.plist", "assetsList.png", 20);
	this->addChild(_spriteBatch, -1);
    
	//create the background
	CCSprite* background = CCSprite::createWithSpriteFrameName("paper.png");
	CCSize backgroundSpriteSize = background->boundingBox().size;
	background->setScaleX(visibleSize.width/backgroundSpriteSize.width);
	background->setScaleY(visibleSize.height/backgroundSpriteSize.height);
	background->setPosition(ccp(screenHalfWidth + origin.x, screenHalfHeight + origin.y));
	_spriteBatch->addChild(background, 0);

	//create the title image
	CCSprite* titleImage = CCSprite::createWithSpriteFrameName("title.png");
	CCSize titleSpriteSize = titleImage->boundingBox().size;
	titleImage->setScaleX(visibleSize.width/titleSpriteSize.width);
	titleImage->setScaleY(titleImage->getScaleX());
	titleImage->setPosition(ccp(screenHalfWidth + origin.x, screenHalfHeight + origin.y + (titleSpriteSize.height*0.25)));
	_spriteBatch->addChild(titleImage, 1);

	//create the touch to start image
	CCSprite* touchInfoImage = CCSprite::createWithSpriteFrameName("touchToStart.png");
	CCSize touchInfoSpriteSize = touchInfoImage->boundingBox().size;
	touchInfoImage->setScale(visibleSize.width/touchInfoSpriteSize.width*0.5);
	touchInfoImage->setPosition(ccp(screenHalfWidth + origin.x, origin.y + touchInfoSpriteSize.height));
	_spriteBatch->addChild(touchInfoImage, 1);

	return true;
}

void TitleScreen::onUpdate(float deltaTime)
{

}

void TitleScreen::draw()
{
}

void TitleScreen::ccTouchesBegan(CCSet* touches, CCEvent* event)
{
}

void TitleScreen::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
	CCPoint touchPos = this->getParent()->convertTouchToNodeSpace(dynamic_cast<CCTouch*>(touches->anyObject()));

	ScreenManager::getInstance()->changeScene(this, ScreenManager::MAIN_SCREEN);

	//play the pressed sound
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("audio/click.wav");
}