#include "PatternDetectedEventData.h"

PatternDetectedEventData::PatternDetectedEventData(unsigned int id, float distance) : EventData(),
																					  _id(id),
																					  _distance(distance)
{
}

PatternDetectedEventData::~PatternDetectedEventData()
{
}

unsigned int PatternDetectedEventData::id()
{
	return _id;
}

float PatternDetectedEventData::distance()
{
	return _distance;
}