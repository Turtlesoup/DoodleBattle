#include "DraggableSprite.h"

DraggableSprite::DraggableSprite() : CCSprite()
{
}

DraggableSprite::~DraggableSprite()
{
}

void DraggableSprite::onEnter()
{
    CCSprite::onEnter();
	_touchOffset = CCPoint(0,0);
	_touching = false;
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}

void DraggableSprite::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCSprite::onExit();
}

bool DraggableSprite::ccTouchBegan(CCTouch *touch, CCEvent* event)
{
	CCPoint touchLocation = getParent()->convertTouchToNodeSpace(touch);

	if(boundingBox().containsPoint(touchLocation))
	{
        //this->removeFromParentAndCleanup(true);
		_touchOffset.x = getPositionX() - touchLocation.x;
		_touchOffset.y = getPositionY() - touchLocation.y;

		_touching = true;
    }

    return true;
}

void DraggableSprite::ccTouchMoved(CCTouch *touch, CCEvent* event)
{
	if(_touching)
	{
		CCPoint touchLocation = getParent()->convertTouchToNodeSpace(touch);
		setPositionX( touchLocation.x + _touchOffset.x );
		setPositionY( touchLocation.y + _touchOffset.y );
	}
}

void DraggableSprite::ccTouchEnded(CCTouch *touch, CCEvent* event)
{
	_touching = false;
}