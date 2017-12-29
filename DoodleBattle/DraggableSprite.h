#ifndef DRAGGABLE_SPRITE_H
#define DRAGGABLE_SPRITE_H

#include "cocos2d.h"

USING_NS_CC;

class DraggableSprite : public CCSprite, public CCTargetedTouchDelegate
{
	public:
		DraggableSprite();
		virtual ~DraggableSprite();
		virtual void onEnter();
		virtual void onExit();
		bool ccTouchBegan(CCTouch *touch, CCEvent *pEvent);
		void ccTouchMoved(CCTouch *touch, CCEvent *pEvent);
		void ccTouchEnded(CCTouch *touch, CCEvent *pEvent);
	private:
		bool _touching;
		CCPoint _touchOffset;
};


#endif