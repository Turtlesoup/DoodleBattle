#ifndef WIN_SCREEN_H
#define WIN_SCREEN_H

#include "Screen.h"
#include "ScreenManager.h"
#include "EventData.h"

class WinScreen : public Screen
{
	public:
		WinScreen(ScreenData* screenData = NULL);
		~WinScreen();
		virtual bool initialise();
	private:
		void onButtonPressed(EventData* eventData);
		unsigned int _score;
		CCSpriteBatchNode* _spriteBatch;
		CCLabelBMFont* _scoreTextField;
		std::stringstream _scoreText;
		const std::string RETRY_BUTTON_ID;
		const std::string EXIT_BUTTON_ID;
};

#endif