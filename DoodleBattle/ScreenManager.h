#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include "cocos2d.h"
#include "SpriteBatchManager.h"
#include "Screen.h"

class ScreenManager
{
	public:
		ScreenManager();
		~ScreenManager();

		static enum screenIDs
		{
			TITLE_SCREEN,
			MAIN_SCREEN,
			WIN_SCREEN
		};

		static ScreenManager* getInstance();
		Screen* createScreen(screenIDs screenID, ScreenData* screenData = NULL);
		SpriteBatchManager* getSpriteBatchManager();
		CCScene* getCurrentScene();
		void changeScene(Screen* currentScreen, screenIDs nextScreenID, ScreenData* screenData = NULL);
	private:
		static ScreenManager* _instance;
		SpriteBatchManager* _spriteBatchManager;
		CCScene* _currentScene;
};

#endif