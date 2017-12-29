#ifndef BUTTERFLY_OBJECT_H
#define BUTTERFLY_OBJECT_H

#include "PatternObject.h"

USING_NS_CC;

class ButterflyObject : public PatternObject
{
	public:
		ButterflyObject(std::vector<CCPoint>& pattern);
		virtual void update(float deltaTime);
	private:
		const enum WING_DIRECTION
		{
			WING_DIRECTION_UP,
			WING_DIRECTION_DOWN
		};
		std::vector<CCPoint*> _leftPoints;
		std::vector<CCPoint*> _rightPoints;
		std::vector<float> _leftPointDistancesFromCenter;
		std::vector<float> _rightPointDistancesFromCenter;
		unsigned int _leftPointsLength;
		unsigned int _rightPointsLength;
		WING_DIRECTION _wingDirection;
		float _currentWingTime;
		const float WING_DURATION;
		const float SPEED;
		float _speed;
		float BUTTERFLY_SUCCESS_THRESHOLD;
		float BUTTERFLY_DRAWABLE_THRESHOLD;
};

#endif