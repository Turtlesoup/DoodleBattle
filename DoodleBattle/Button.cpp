#include "BaseApplication.h"
#include "Button.h"
#include "EventManager.h"
#include "ButtonPressedEventData.h"
#include "SimpleAudioEngine.h"

Button::Button(const std::string& id, const float scaleFactor) : CCSprite(),
																 _id(id),
																 _scaleFactor(scaleFactor)
{
	_pressed = false;
	setScale(BaseApplication::screenScaleFactorFrom768x1024 * _scaleFactor);
}

Button::~Button()
{
}

const std::string& Button::getID()
{
	return _id;
}

void Button::onEnter()
{
    CCSprite::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}

void Button::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCSprite::onExit();
}

bool Button::ccTouchBegan(CCTouch *touch, CCEvent* event)
{
	if(!_pressed)
	{
		CCPoint touchLocation = getParent()->convertTouchToNodeSpace(touch);

		if(boundingBox().containsPoint(touchLocation))
		{
			setScale(BaseApplication::screenScaleFactorFrom768x1024 * _scaleFactor * 0.9);
			_pressed = true;

			//play the button pressed sound
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("audio/click.wav");
		}
	}

    return true;
}

void Button::ccTouchMoved(CCTouch *touch, CCEvent* event)
{
	if(_pressed)
	{
		CCPoint touchLocation = getParent()->convertTouchToNodeSpace(touch);

		if(!boundingBox().containsPoint(touchLocation))
		{
			setScale(BaseApplication::screenScaleFactorFrom768x1024 * _scaleFactor);
			_pressed = false;
		}
	}
}

void Button::ccTouchEnded(CCTouch *touch, CCEvent* event)
{
	if(_pressed)
	{
		CCPoint touchLocation = getParent()->convertTouchToNodeSpace(touch);

		if(boundingBox().containsPoint(touchLocation))
		{
			EventManager::Get()->dispatchEvent(ON_BUTTON_PRESSED, new ButtonPressedEventData(this) );
		}

		setScale(BaseApplication::screenScaleFactorFrom768x1024 * _scaleFactor);
		_pressed = false;
	}
}