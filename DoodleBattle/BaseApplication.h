#ifndef BASE_APPLICATION_H
#define BASE_APPLICATION_H

#include "cocos2d.h"

class BaseApplication : private cocos2d::CCApplication
{
	public:

		BaseApplication();

		~BaseApplication();

		/**
		@brief    Implement CCDirector and CCScene init code here.
		@return true    Initialize success, app continue.
		@return false   Initialize failed, app terminate.
		*/
		virtual bool applicationDidFinishLaunching();

		/**
		@brief  The function be called when the application enter background
		@param  the pointer of the application
		*/
		virtual void applicationDidEnterBackground();

		/**
		@brief  The function be called when the application enter foreground
		@param  the pointer of the application
		*/
		virtual void applicationWillEnterForeground();

		/**
		@brief	Initialises the application and starts the first scene
		@return	a pointer to the initialised scene
		*/
		cocos2d::CCScene* initialise();

		static cocos2d::CCPoint origin;
		static cocos2d::CCSize visibleSize;
		static unsigned int screenWidth;
		static unsigned int screenHeight;
		static float screenScaleFactorFrom768x1024;

		static void BaseApplication::openConsole();
};

#endif