#ifndef BUTTON_PRESSED_EVENT_DATA_H
#define BUTTON_PRESSED_EVENT_DATA_H

#include "EventData.h"
#include "Button.h"

class ButtonPressedEventData : public EventData
{
	public:
		ButtonPressedEventData(Button* button);
		~ButtonPressedEventData();
		Button* getButton();
	private:
		Button* _button;
};

#endif