#ifndef BUTTON_H
#define BUTTON_H

#include "cocos2d.h"

USING_NS_CC;

class Button : public CCSprite, public CCTargetedTouchDelegate
{
	public:
		Button(const std::string& id, const float scaleFactor);
		virtual ~Button();
		virtual void onEnter();
		virtual void onExit();
		bool ccTouchBegan(CCTouch *touch, CCEvent *pEvent);
		void ccTouchMoved(CCTouch *touch, CCEvent *pEvent);
		void ccTouchEnded(CCTouch *touch, CCEvent *pEvent);
		const std::string& getID();
	private:
		bool _pressed;
		const std::string& _id;
		const float _scaleFactor;
};

#endif