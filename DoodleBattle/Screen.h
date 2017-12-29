#ifndef SCREEN_H
#define SCREEN_H

#include "cocos2d.h"
#include "ScreenData.h"

USING_NS_CC;

class Screen : public CCLayer
{
	public:
		Screen(ScreenData* screenData = NULL);
		~Screen();
		virtual bool initialise();
		virtual void draw();
		virtual void onUpdate(float deltaTime);
		virtual void dispose();
};

#endif