#include "ButtonPressedEventData.h"

ButtonPressedEventData::ButtonPressedEventData(Button* button) : EventData(),
																 _button(button)
{
}

ButtonPressedEventData::~ButtonPressedEventData()
{
}

Button* ButtonPressedEventData::getButton()
{
	return _button;
}