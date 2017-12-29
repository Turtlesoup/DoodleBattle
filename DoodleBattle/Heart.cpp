#include "Heart.h"

Heart::Heart() : CCSprite(),
				 _on(false)
{
}

Heart::~Heart()
{
}

void Heart::on(bool value)
{
	if(_on != value)
	{
		_on = value;
		if(_on)
		{
			setOpacity(255);
		}
		else
		{
			setOpacity(100);
		}
	}
}