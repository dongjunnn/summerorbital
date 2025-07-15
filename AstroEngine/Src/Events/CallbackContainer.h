#pragma once
#include "Events.h"

class CallbackContainer
{
public:
	virtual ~CallbackContainer() = default; 
	virtual void broadcast(Event& event) = 0;
};


template<typename EventType>
class CallbackContainerDerived : public CallbackContainer	// contains callback functions 
{
public:

	// adding callback to list of callbacks attached to this particular event
	void Add(std::function<void(EventType&)> callback)
	{
		callbacks.push_back(callback);
	}

	// broadcasts the event of this particular type in a blocking manner
	void broadcast(Event& event) override
	{
		// calling from base class looks cleaner
		EventType& event_casted = static_cast<EventType&>(event);	
		for (auto& callback : callbacks)
		{
			callback(event_casted);
		}
	}

private:
	std::vector<std::function<void(EventType&)>> callbacks;
};