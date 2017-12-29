#include "Screen.h"
#include "BaseApplication.h"
#include "AppMacros.h"

Screen::Screen(ScreenData* screenData)
{
}

Screen::~Screen()
{
	unschedule(schedule_selector(Screen::onUpdate));
}

void Screen::onUpdate(float deltaTime)
{
}

bool Screen::initialise()
{
	//super init
	if( !CCLayer::init() )
    {
        return false;
    }

	//schedule update
	schedule(schedule_selector(Screen::onUpdate));

	return true;
}

void Screen::draw()
{
}

void Screen::dispose()
{
}