#include "ArrowObject.h"
#include "BaseApplication.h"

ArrowObject::ArrowObject(std::vector<CCPoint>& pattern) : PatternObject(pattern),
														  SPEED(1000/BaseApplication::visibleSize.height),
														  DESTROY_THRESHOLD(BaseApplication::visibleSize.height + height)
{
	_collision = true;
}

ArrowObject::~ArrowObject()
{
}

void ArrowObject::update(float deltaTime)
{
	PatternObject::update(deltaTime);

	translatePoints(0, SPEED);

	if((currentY - height) > DESTROY_THRESHOLD)
	{
		destroy();
	}
}