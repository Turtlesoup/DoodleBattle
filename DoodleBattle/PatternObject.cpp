#include "PatternObject.h"
#include <algorithm>

PatternObject::PatternObject(std::vector<CCPoint>& pattern) : FADE_TIME_IN_SECONDS(0.3)
{
	_pattern = std::vector<CCPoint>(pattern);
	_patternLength = pattern.size();

	//move the points such that the top left point is located at 0,0
	//and set the initial x and y position to this offset
	float minX = INT_MAX;
	float minY = INT_MAX;
	float maxX = INT_MIN;
	float maxY = INT_MIN;
	float pointX;
	float pointY;
	unsigned int pointIndex;
	for(pointIndex = 0; pointIndex < _patternLength; pointIndex++)
	{
		pointX = _pattern[pointIndex].x;
		pointY = _pattern[pointIndex].y;

		if(pointX < minX)
		{
			minX = pointX;
		}
		if(pointY < minY)
		{
			minY = pointY;
		}

		if(pointX > maxX)
		{
			maxX = pointX;
		}
		if(pointY > maxY)
		{
			maxY = pointY;
		}
	}

	_destroyed = false;

	_collision = false;

	_fading = false;
	_currentFadeTime = FADE_TIME_IN_SECONDS;

	currentX = minX;
	currentY = minY;
	width = maxX - minX;
	height = maxY - minY;
}

PatternObject::~PatternObject()
{

}

void PatternObject::update(float deltaTime)
{
	if(_fading)
	{
		fadeAndRemove(deltaTime);
	}
}


void PatternObject::translatePoints(float dx, float dy)
{
	unsigned int pointIndex;
	for(pointIndex = 0; pointIndex < _patternLength; pointIndex++)
	{
		_pattern[pointIndex].x += dx;
		_pattern[pointIndex].y += dy;
	}

	currentX += dx;
	currentY += dy;
}

void PatternObject::draw()
{
	float currentAlpha = _currentFadeTime/FADE_TIME_IN_SECONDS;
	ccDrawColor4F(0.0, 0.0, 0.0, currentAlpha);
	CCPoint *first;

	//draw the pattern
	if(_patternLength > 0)
	{
		first = &(_pattern[0]);

		ccDrawPoly(first, 0, false);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(sizeof(CCPoint), GL_FLOAT, 0, first);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, _patternLength);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}

void PatternObject::destroy()
{
	_destroyed = true;
	_collision = false;
	_fading = true;
}

void PatternObject::fadeAndRemove(float deltaTime)
{
	_currentFadeTime -= deltaTime;
	if(_currentFadeTime < 0)
	{
		_pattern.clear();
		_patternLength = 0;
		_fading = false;
	}
}

bool PatternObject::checkCollision(float colX, float colY, float colWidth, float colHeight)
{
	return (currentX + width > colX - currentX &&
			currentX < colX + colWidth - currentX &&
			currentY + height > colY &&
			currentY < colY + colHeight);
}

bool PatternObject::isDestroyed()
{
	return _destroyed;
}

bool PatternObject::getCollision()
{
	return _collision;
}