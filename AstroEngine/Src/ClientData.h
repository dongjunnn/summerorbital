#pragma once
#include "ECS/ECS.h"

class ClientData
{
public:	// if we want, we can store player nicknames and stuff here ig, but i doubt we are going to
	ClientData() {}

	void assignPlayer(Entity id);
	void disassignPlayer(Entity id);

	int getPlayerRefbyID(Entity id) const;
	Entity getPlayerIDbyRef(int reference);

private:
	Entity p1 = 0;
	Entity p2 = 0;
};