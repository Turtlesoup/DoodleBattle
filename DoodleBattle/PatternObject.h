#ifndef PATTERN_OBJECT_H
#define PATTERN_OBJECT_H

#include <vector>
#include "cocos2d.h"

USING_NS_CC;

class PatternObject
{
	public:
		PatternObject(std::vector<CCPoint>& pattern);
		~PatternObject();
		virtual void update(float deltaTime);
		void draw();
		void destroy();
		float currentX;
		float currentY;
		float width;
		float height;
		bool checkCollision(float x, float y, float width, float height);
		bool isDestroyed();
		bool getCollision();
	protected:
		unsigned int _patternLength;
		std::vector<CCPoint> _pattern;
		void translatePoints(float dx, float dy);
		bool _collision;
		bool _destroyed;
	private:
		void fadeAndRemove(float deltaTime);
		const float FADE_TIME_IN_SECONDS;
		float _currentFadeTime;
		bool _fading;
};

#endif