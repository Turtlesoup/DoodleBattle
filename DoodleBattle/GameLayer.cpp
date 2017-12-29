#include "GameLayer.h"

GameLayer::GameLayer()
{

}

GameLayer::~GameLayer()
{
	unsigned int numPatternObjects = _patternObjects.size();
	for(unsigned int patternObject = 0; patternObject < numPatternObjects; patternObject++)
	{
		delete _patternObjects[patternObject];
	}
}

void GameLayer::update(float deltaTime)
{
	unsigned int numPatternObjects = _patternObjects.size();
	for(unsigned int patternObject = 0; patternObject < numPatternObjects; patternObject++)
	{
		_patternObjects[patternObject]->update(deltaTime);
	}
}

void GameLayer::draw()
{
	unsigned int numPatternObjects = _patternObjects.size();
	for(unsigned int patternObject = 0; patternObject < numPatternObjects; patternObject++)
	{
		_patternObjects[patternObject]->draw();
	}
}

void GameLayer::addPatternObject(PatternObject* patternObject)
{
	_patternObjects.push_back(patternObject);
}

std::vector<PatternObject*>& GameLayer::getPatternObjects()
{
	return _patternObjects;
}