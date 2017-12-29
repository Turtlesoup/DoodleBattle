#include "WinScreen.h"
#include "Button.h"
#include "ButtonPressedEventData.h"
#include "EventManager.h"
#include "BaseApplication.h"

WinScreen::WinScreen(ScreenData* screenData) : Screen(screenData),
											   RETRY_BUTTON_ID("retry"),
											   EXIT_BUTTON_ID("exit")
{
	_score = screenData->intParameters["score"];
}

WinScreen::~WinScreen()
{
	ScreenManager::getInstance()->getSpriteBatchManager()->removeSpriteBatch("gameSprites");

	EventManager::Get()->removeEventListener(ON_BUTTON_PRESSED, std::bind(&WinScreen::onButtonPressed, this, std::placeholders::_1));
}

bool WinScreen::initialise()
{
	//super init
	if(!Screen::initialise())
	{
		return false;
	}

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

	//create the game over image
	CCSprite* gameOverImage = CCSprite::createWithSpriteFrameName("gameOver.png");
	CCSize gameOverSpriteSize = gameOverImage->boundingBox().size;
	gameOverImage->setScaleX(visibleSize.width/gameOverSpriteSize.width*0.5);
	gameOverImage->setScaleY(gameOverImage->getScaleX());
	gameOverImage->setPosition(ccp(origin.x + screenHalfWidth, origin.y + visibleSize.height*0.65 + (gameOverSpriteSize.height*0.25)));
	_spriteBatch->addChild(gameOverImage, 1);

	//create the retry button
	Button* retryButton = new Button(RETRY_BUTTON_ID, 0.5);
	retryButton->initWithSpriteFrameName("retryButton.png");
	retryButton->setPosition(ccp(origin.x + visibleSize.width*0.25, origin.y + visibleSize.height*0.1));
	_spriteBatch->addChild(retryButton, 1);

	//create the exit button
	Button* exitButton = new Button(EXIT_BUTTON_ID, 0.5);
	exitButton->initWithSpriteFrameName("exitButton.png");
	exitButton->setPosition(ccp(origin.x + visibleSize.width*0.75, origin.y + visibleSize.height*0.1));
	_spriteBatch->addChild(exitButton, 1);

	//create the score text
	_scoreText << "Score: " << _score;

	//create the score textfield
	_scoreTextField = CCLabelBMFont::create(_scoreText.str().c_str(), "font1.fnt");
	_scoreTextField->setPosition(ccp(origin.x + visibleSize.width*0.5, origin.y + visibleSize.height*0.4));
	addChild(_scoreTextField);

	//add the button pressed event listener
	EventManager::Get()->addEventListener(ON_BUTTON_PRESSED, std::bind(&WinScreen::onButtonPressed, this, std::placeholders::_1));

	return true;
}

void WinScreen::onButtonPressed(EventData* eventData)
{
	ButtonPressedEventData* buttonEventData = (ButtonPressedEventData*)eventData;
	std::string buttonID = buttonEventData->getButton()->getID();
	if(buttonID == RETRY_BUTTON_ID)
	{
		ScreenManager::getInstance()->changeScene(this, ScreenManager::MAIN_SCREEN);
	}
	else if(buttonID == EXIT_BUTTON_ID)
	{
		ScreenManager::getInstance()->changeScene(this, ScreenManager::TITLE_SCREEN);
	}
}