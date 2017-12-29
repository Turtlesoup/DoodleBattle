#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include "EventData.h"
#include <map>
#include <vector>
#include <functional>


enum EVENT_TYPE
{
	ON_PATTERN_DETECTION,
	ON_BUTTERFLY_REACHED_GOAL,
	ON_BUTTON_PRESSED
};

class EventManager
{

public:
	EventManager();
	~EventManager();
	static EventManager* Get();
	void addEventListener(EVENT_TYPE eventType, std::function< void( EventData* ) > callbackFunction);
	void removeEventListener(EVENT_TYPE eventType, std::function< void( EventData* ) > callbackFunction);
	void dispatchEvent(EVENT_TYPE eventType, EventData *event);

private:
	static EventManager* _instance;
	std::map< EVENT_TYPE, std::vector< std::function< void( EventData* ) > > > _callbackFunctions;
};


#endif