#ifndef PATTERN_DETECTED_EVENT_DATA
#define PATTERN_DETECTED_EVENT_DATA

#include "EventData.h"

class PatternDetectedEventData : public EventData
{
	public:
		PatternDetectedEventData(unsigned int id, float distance);
		~PatternDetectedEventData();
		unsigned int id();
		float distance();
	private:
		unsigned int _id;
		float _distance;
};

#endif