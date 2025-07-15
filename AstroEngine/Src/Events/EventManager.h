#pragma once
#include <typeindex>
#include <queue>
#include <functional>
#include <vector>
#include "../ECS/ECS.h"
#include "CallbackContainer.h"
#include "Events.h"

using CallbackContainerPtr = std::unique_ptr<CallbackContainer>;

/*  Yay event manager
so there are two kinds of events here, immediate call events that fire in a blocking manner,
and queued events that happen in the update loop. If all goes well, we hopefully only need one
EventManager instance to make everything work.
*/


class EventManager
{
public:
	EventManager() {}
	~EventManager() {}

	template<typename EventType, typename... Args>
	void queueEvent(Args&&... args)		// lvalues, rvalues hais
	{
		std::unique_ptr<Event> event = std::make_unique<EventType>(args...);	// std::forward is better?
		eventQueue.push(std::move(event));
	}

	void processEvents()
	{
		while (!eventQueue.empty())
		{
			std::unique_ptr<Event>& event = eventQueue.front();

			auto it = eventCallbackMap.find(std::type_index(typeid(*event)));
			if (it == eventCallbackMap.end())
			{
				std::cerr << "Event queue has no connections" << std::endl;
				continue;
			}

			it->second->broadcast(*event);
			eventQueue.pop();
		}
	}

	template<typename EventType, typename... Args>
	void broadcast(Args&&... args)
	{
		if (eventCallbackMap.find(getTypeIndex<EventType>()) == eventCallbackMap.end())
		{
			std::cerr << "The event type has no connections" << std::endl;
			return;
		}

		EventType event(args...);
		eventCallbackMap[getTypeIndex<EventType>()]->broadcast(event);
	}

	template<typename EventType>	// to connect lambdas to the event system
	void connect(std::function<void(EventType& event)> callback)
	{
		if (eventCallbackMap.find(getTypeIndex<EventType>()) == eventCallbackMap.end()) { createEvent<EventType>(); };
		getCallbackContainer<EventType>().Add(callback);
	}

	// connect takes in a function, which expects an event object of the indicated type,
	// as well as a pointer to the parent 
	template<typename EventType, typename ClassType>	// this is to connect existing functions to the event system
	void connect(void (ClassType::* memberFunction)(EventType&), ClassType* parentInstance)
	{
		if (eventCallbackMap.find(getTypeIndex<EventType>()) == eventCallbackMap.end()) { createEvent<EventType>(); }

		auto callback = [parentInstance, memberFunction](EventType& event) { (parentInstance->*memberFunction)(event); };
		getCallbackContainer<EventType>().Add(callback);
	}

	template<typename EventType>
	void createEvent()
	{
		eventCallbackMap[getTypeIndex<EventType>()] = std::make_unique<CallbackContainerDerived<EventType>>();	// ecs flashback
	}

private:
	std::queue<std::unique_ptr<Event>> eventQueue;
	std::unordered_map<std::type_index, CallbackContainerPtr> eventCallbackMap;

	template<typename EventType> CallbackContainerDerived<EventType>& getCallbackContainer() // this time its a reference, instead of the pointer itself
	{
		auto& derivedPtr = eventCallbackMap[getTypeIndex<EventType>()];
		return static_cast<CallbackContainerDerived<EventType>&>(*derivedPtr);	// dereferencing the pointer to access the value
	}

	template<typename EventType> std::type_index getTypeIndex()
	{
		return std::type_index(typeid(EventType));
	}
};