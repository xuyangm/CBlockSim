#pragma once
#include <set>
#include "Models/Event.h"

struct EventCmpAscending {
	bool operator() (Event a, Event b) const {
		return a.timestamp < b.timestamp;
	}
};

using namespace std;

class EventEngine
{
	public:
		multiset<Event, EventCmpAscending> eventPool;
		
		multiset<Event>::iterator AddEvent(Event& e);
		void RemoveEvent(multiset<Event>::iterator iter);
		multiset<Event>::iterator GetEvent();
		bool IsEmpty();

		EventEngine();
};