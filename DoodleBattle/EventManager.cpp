#include "EventManager.h"
#include "PatternDetectedEventData.h"
#include "ButtonPressedEventData.h"

EventManager* EventManager::_instance = NULL;

EventManager::EventManager()
{
	_callbackFunctions = std::map< EVENT_TYPE, std::vector< std::function< void( EventData* ) > > >();
}

EventManager::~EventManager()
{
}

EventManager* EventManager::Get()
{
	if(!_instance)
	{
		_instance = new EventManager();
	}
		
	return _instance;
}

void EventManager::addEventListener( EVENT_TYPE eventType, std::function< void( EventData* ) > callbackFunction)
{
	//if the key doesnt exist, create a vector for that key
	if( EventManager::_callbackFunctions.find(eventType) == EventManager::_callbackFunctions.end() )
	{
		EventManager::_callbackFunctions[eventType] = std::vector< std::function< void( EventData* ) > >();
	}

	//add the element to the corresponding vector for the given key
	EventManager::_callbackFunctions[eventType].push_back(callbackFunction);
}

void EventManager::removeEventListener(EVENT_TYPE eventType, std::function< void( EventData* ) > callbackFunction)
{
	//removes the given callback from the callback list for the given type if such a callback exists
	std::vector< std::function< void( EventData* ) > >& callbacks = EventManager::_callbackFunctions[eventType];
	unsigned int callbacksLength = callbacks.size();
	for(unsigned int callbackIndex = 0; callbackIndex < callbacksLength; callbackIndex++)
	{
		if( callbacks[callbackIndex].target<void( EventData* )>() == callbackFunction.target<void( EventData* )>() )
		{
			callbacks.erase(callbacks.begin() + callbackIndex);
			break;
		}
	}
}

void EventManager::dispatchEvent(EVENT_TYPE eventType, EventData *eventData)
{
	//dispatch the given event to all callbacks of the given type
	std::vector< std::function< void( EventData* ) > > callbacks = EventManager::_callbackFunctions[eventType];
	unsigned int callbacksLength = callbacks.size();
	for(unsigned int callbackIndex = 0; callbackIndex < callbacksLength; callbackIndex++)
	{
		callbacks[callbackIndex](eventData);
	}
	//destroy the event
	delete eventData;
}