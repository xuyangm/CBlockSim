#include "EventEngine.h"
#include <iostream>

multiset<Event>::iterator EventEngine::AddEvent(Event& e)
{
	return eventPool.insert(e);
}

void EventEngine::RemoveEvent(multiset<Event>::iterator iter)
{
	eventPool.erase(iter);
}

multiset<Event>::iterator EventEngine::GetEvent()
{
	return eventPool.begin();
}

bool EventEngine::IsEmpty()
{
	return eventPool.empty();
}

EventEngine::EventEngine()
{

}