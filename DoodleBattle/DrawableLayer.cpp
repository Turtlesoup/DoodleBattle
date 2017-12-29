#include "DrawableLayer.h"
#include "BaseApplication.h"
#include "Screen.h"
#include "Line.h"
#include "PatternDetectionManager.h"
#include "EventManager.h"

const float MOVE_DISTANCE_THRESHOLD = 5.0;
const float NO_TOUCH_WAIT_TIME_IN_SECONDS = 0.1;
const float FADE_TIME_IN_SECONDS = 0.3;

const char* PATTERNS_FILE = "patterns.txt";

bool createPatternsMode = false;
unsigned int patternsDrawnCount = 0;
const unsigned int NUMBER_OF_PATTERNS = 2;
unsigned int patternIDs[NUMBER_OF_PATTERNS] = {0, 1};
unsigned int numberOfPatternsForEachRequired = 15;

DrawableLayer::DrawableLayer() : LINE_THICKNESS(1.5)
{
	_patternDetection = new PatternDetectionManager();

	if(!createPatternsMode)
	{
		_patternDetection->loadPatternsFromFile(PATTERNS_FILE);
	}

	_verts = std::vector<CCPoint>();
	_points = std::vector<CCPoint>();

	_touchingJustEnded = false;
	_isTouching = false;
	_fading = false;
	_currentFadeTime = FADE_TIME_IN_SECONDS;
}

DrawableLayer::~DrawableLayer()
{
	delete _patternDetection;
}

void DrawableLayer::draw()
{
	float alpha = _currentFadeTime/FADE_TIME_IN_SECONDS;
	ccDrawColor4F(0.0, 0.0, 0.0, alpha);

	unsigned int numVertices;
	CCPoint *first;

	//draw the pattern
	numVertices = _verts.size();
	if(numVertices > 0)
	{
		first = &(_verts[0]);

		ccDrawPoly(first, 0, false);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(sizeof(CCPoint), GL_FLOAT, 0, first);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, numVertices);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}

std::vector<CCPoint>& DrawableLayer::getPatternVertices()
{
	return _verts;
}

void DrawableLayer::clear()
{
	_currentFadeTime = FADE_TIME_IN_SECONDS;
	_fading = true;
}

void DrawableLayer::fadeAndRemoveShapes(float deltaTime)
{
	_currentFadeTime -= deltaTime;
	if(_currentFadeTime < 0 && _verts.size() > 0)
	{
		_verts.clear();
		_points.clear();

		_fading = false;
	}
}

void DrawableLayer::update(float deltaTime)
{
	if(_touchingJustEnded)
	{
		if(createPatternsMode)
		{
			unsigned int idIndex = floor((float)(patternsDrawnCount/numberOfPatternsForEachRequired));
			unsigned int id = patternIDs[idIndex];
			_patternDetection->addPatternToDetectionList(id, _points);
			patternsDrawnCount++;

			if(patternsDrawnCount == (NUMBER_OF_PATTERNS*numberOfPatternsForEachRequired))
			{
				_patternDetection->normalisePatterns();
				_patternDetection->writePatternsFromFile(PATTERNS_FILE);
			}

			clear();
		}
		else
		{
			_patternDetection->getClosestPatternID(_points);
		}

		_touchingJustEnded = false;
	}

	if(_fading)
	{
		fadeAndRemoveShapes(deltaTime);
	}
}

void DrawableLayer::onTouchBegin(const CCPoint &touchPosition)
{
	_fading = false;
	_currentFadeTime = FADE_TIME_IN_SECONDS;

	_verts.clear();
	_points.clear();

	_initialPoition = touchPosition;

	//create a vertex at the initial touch point
	_verts.push_back(_initialPoition);

	//add the actual point to the vector of points used for pattern recognition
	_points.push_back(_initialPoition);

	_isTouching = true;
}

void DrawableLayer::onTouchMoved(const CCPoint &touchPosition)
{
	//if there was no touch begin call then we missed it and we must
	//start the touch process
	if(!_isTouching)
	{
		onTouchBegin(touchPosition);
	}

	float dx = _initialPoition.x - touchPosition.x;
	float dy = _initialPoition.y - touchPosition.y;

	//remove the last end point if one has been added
	if(_verts.size() >= 4)
	{
		_verts.erase(_verts.end()-1);
	}

	//if the distance from the last segment is large enough,
	//add a new segment consisting of two points along the perpendicular line from the motion vector
	if(Line::getCCPointDistance(touchPosition, _initialPoition) > MOVE_DISTANCE_THRESHOLD)
	{
		Line line = Line::getPerpendicularLine(_initialPoition, touchPosition, LINE_THICKNESS * BaseApplication::screenScaleFactorFrom768x1024);

		CCPoint p1 = line.getP1();
		CCPoint p2 = line.getP2();

		//if we have a line in progress then we must match the points to the closest points on the line.
		//this avoids the line "flipping" when the perpendicular line points flip sides;
		if(_verts.size() >= 2)
		{
			float distanceP1 = Line::getCCPointDistance(p1, _verts[_verts.size()-1]);
			float distanceP2 = Line::getCCPointDistance(p2, _verts[_verts.size()-1]);

			if(distanceP1 <= distanceP2)
			{
				_verts.push_back(p2);
				_verts.push_back(p1);
			}
			else
			{
				_verts.push_back(p1);
				_verts.push_back(p2);
			}
		}
		else
		{
			_verts.push_back(p1);
			_verts.push_back(p2);
		}

		_initialPoition = touchPosition;
	}

	//add a new endpoint
	_verts.push_back(touchPosition);

	//add the actual point to the vector of points used for pattern recognition
	_points.push_back(touchPosition);
}

void DrawableLayer::onTouchEnd(const CCPoint &touchPosition)
{
	_touchingJustEnded = true;
	_isTouching = false;
}

void DrawableLayer::onTouchCancel()
{
	_touchingJustEnded = false;
	_isTouching = false;
	_fading = false;
	_verts.clear();
	_points.clear();
}