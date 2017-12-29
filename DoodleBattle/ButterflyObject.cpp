#include "ButterflyObject.h"
#include "EventManager.h"
#include "BaseApplication.h"

ButterflyObject::ButterflyObject(std::vector<CCPoint>& pattern) : PatternObject(pattern),
																  WING_DURATION(1.0),
																  SPEED(300/BaseApplication::visibleSize.height),
																  BUTTERFLY_SUCCESS_THRESHOLD(BaseApplication::visibleSize.height),
																  BUTTERFLY_DRAWABLE_THRESHOLD(BaseApplication::visibleSize.height*0.25)
{
	unsigned int pointIndex;
	float midX = currentX + (width * 0.5);
	float pointX;
	float distanceFromCenter = 0;

	for(pointIndex = 0; pointIndex < _patternLength; pointIndex++)
	{
		pointX = _pattern[pointIndex].x;

		if(pointX > midX)
		{
			_rightPoints.push_back(&_pattern[pointIndex]);
			distanceFromCenter = pointX - midX;
			_rightPointDistancesFromCenter.push_back(distanceFromCenter);
		}
		else
		{
			_leftPoints.push_back(&_pattern[pointIndex]);
			distanceFromCenter = midX - pointX;
			_leftPointDistancesFromCenter.push_back(distanceFromCenter);
		}
	}

	_leftPointsLength = _leftPoints.size();
	_rightPointsLength = _rightPoints.size();

	for(pointIndex = 0; pointIndex < _leftPointsLength; pointIndex++)
	{
		_leftPointDistancesFromCenter[pointIndex] /= (width*0.5);
	}

	for(pointIndex = 0; pointIndex < _rightPointsLength; pointIndex++)
	{
		_rightPointDistancesFromCenter[pointIndex] /= (width*0.5);
	}

	_currentWingTime = WING_DURATION*0.5;
	_wingDirection = WING_DIRECTION_UP;

	_speed = SPEED;

	_collision = false;

	if(currentY > BUTTERFLY_DRAWABLE_THRESHOLD)
	{
		destroy();
	}
}


void ButterflyObject::update(float deltaTime)
{
	PatternObject::update(deltaTime);

	if(_destroyed)
	{
		return;
	}

	translatePoints(0, _speed);

	unsigned int pointIndex;

	switch(_wingDirection)
	{
		case WING_DIRECTION_UP:
			for(pointIndex = 0; pointIndex < _leftPointsLength; pointIndex++)
			{
				_leftPoints[pointIndex]->x -= _leftPointDistancesFromCenter[pointIndex];
			}

			for(pointIndex = 0; pointIndex < _rightPointsLength; pointIndex++)
			{
				_rightPoints[pointIndex]->x += _rightPointDistancesFromCenter[pointIndex];
			}
			break;
		case WING_DIRECTION_DOWN:
			for(pointIndex = 0; pointIndex < _leftPointsLength; pointIndex++)
			{
				_leftPoints[pointIndex]->x += _leftPointDistancesFromCenter[pointIndex];;
			}

			for(pointIndex = 0; pointIndex < _rightPointsLength; pointIndex++)
			{
				_rightPoints[pointIndex]->x -= _rightPointDistancesFromCenter[pointIndex];;
			}
			break;
	}

	_currentWingTime -= deltaTime;
	if(_currentWingTime <= 0)
	{
		_currentWingTime += WING_DURATION;
		switch(_wingDirection)
		{
			case WING_DIRECTION_UP:
				_wingDirection = WING_DIRECTION_DOWN;
				break;
			case WING_DIRECTION_DOWN:
				_wingDirection = WING_DIRECTION_UP;
				break;
		}
	}

	if((currentY + height) > BUTTERFLY_SUCCESS_THRESHOLD)
	{
		EventManager::Get()->dispatchEvent(ON_BUTTERFLY_REACHED_GOAL, new EventData());
		destroy();
	}
}