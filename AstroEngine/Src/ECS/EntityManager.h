#pragma once
#include <array>
#include <vector>
#include <numeric>
#include <bitset>
#include "ECS.h"

class EntityManager
{
public:
	
	EntityManager() 
	{
		availableIDs.resize(MAX_ENTITIES);		// stores available ids; 0 is not a valid id;
		std::iota(availableIDs.rbegin(), availableIDs.rend(), 1);  // iota doesn't stand for anything, its iota, the 9th letter of the greek alphabet
	}

	Entity CreateEntity()		// this is not an object, returns an id for you to add components to
	{							// also Entity is an alias for an int
		Entity assignedID = availableIDs.back();
		availableIDs.pop_back();
		activeEntities[assignedID] = true;
		return assignedID;
	}
	
	void DestroyEntity(Entity entity)
	{
		availableIDs.emplace_back(entity);		// allows entity id to be reused
		activeEntities[entity] = false;
	}

	std::array<bool, MAX_ENTITIES>& GetActiveEntities()	// returns an array of entity active states, not the number 
	{
		return activeEntities;
	}

	bool isActive(Entity entity)
	{
		return activeEntities[entity];
	}

private:
	std::vector<Entity> availableIDs; 
	std::array<bool, MAX_ENTITIES> activeEntities{};		// more for debugging
	
};