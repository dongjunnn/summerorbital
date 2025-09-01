#pragma once

class Event
{
public:
	virtual ~Event() = default;
};								

class PlayerJoinedEvent : public Event
{
public:
	PlayerJoinedEvent(Entity id)
		: Event(), id(id) {};

	Entity id;
};

class PlayerDisconnectEvent : public Event
{
public:
	PlayerDisconnectEvent(Entity id)
		: Event(), id(id) {
	};

	Entity id;
};

class PlayerDiedEvent : public Event
{
public:
	PlayerDiedEvent(Entity id) 
		: Event(), id(id) {};
	
	Entity id;
};

class CullProjectilesEvent : public Event
{
public:
	CullProjectilesEvent() {}
};

class MatchRestartEvent : public Event
{
public:
	MatchRestartEvent(Entity id)
		: Event(), id(id) {};

	Entity id;
};