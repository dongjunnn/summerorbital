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

	Entity getPlayerID() const { return id; }

private:
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