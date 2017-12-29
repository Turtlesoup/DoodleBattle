#include "ScreenManager.h"
#include "MainScreen.h"
#include "TitleScreen.h"
#include "WinScreen.h"

ScreenManager* ScreenManager::_instance = NULL;
SpriteBatchManager* _spriteBatchManager = NULL;

ScreenManager::ScreenManager()
{
	if(_instance != NULL)
	{
		std::cerr << "ScreenManager is a singleton class and can therefore only be instantiated once" << std::endl;
	}

	_spriteBatchManager = new SpriteBatchManager();
	_currentScene = CCScene::create();
}

ScreenManager::~ScreenManager()
{
}

CCScene* ScreenManager::getCurrentScene()
{
	return _currentScene;
}

ScreenManager* ScreenManager::getInstance()
{
	if(_instance == NULL)
	{
		_instance = new ScreenManager();
	}

	return ScreenManager::_instance;
}

Screen* ScreenManager::createScreen(screenIDs screenID, ScreenData* screenData)
{
	Screen* screen;
	switch(screenID)
	{
		case TITLE_SCREEN:
			screen = new TitleScreen(screenData);
			break;
		case MAIN_SCREEN:
			screen = new MainScreen(screenData);
			break;
		case WIN_SCREEN:
			screen = new WinScreen(screenData);
			break;
	}

	if(screen)
	{
		screen->initialise();
		return screen;
	}

	std::cerr << "Screen type: " << screenID << " does not exists" << std::endl;
}

void ScreenManager::changeScene(Screen* currentScreen, screenIDs nextScreenID, ScreenData* screenData)
{
	_currentScene->removeChild(currentScreen, true);
	CCScene* newScene = CCScene::create();
	CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(0.5, newScene));
	currentScreen->autorelease();
	Screen* newScreen = createScreen(nextScreenID, screenData);
	newScene->addChild(newScreen);
	_currentScene = newScene;
}

SpriteBatchManager* ScreenManager::getSpriteBatchManager()
{
	return _spriteBatchManager;
}