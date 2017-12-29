#ifndef TITLE_SCREEN_H
#define TITLE_SCREEN_H

#include "Screen.h"
#include "ScreenManager.h"

USING_NS_CC;

class TitleScreen : public Screen
{
	public:
		TitleScreen(ScreenData* screenData = NULL);
		~TitleScreen();
		virtual bool initialise();
		virtual void draw();
		virtual void onUpdate(float deltaTime);
		virtual void ccTouchesBegan(CCSet* touches, CCEvent* event);
		virtual void ccTouchesEnded(CCSet* touches, CCEvent* event);
	private:
		CCSpriteBatchNode* _spriteBatch;
};

#endif