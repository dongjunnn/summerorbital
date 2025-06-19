#pragma once
#include <array>
#include <vector>
#include <map>
#include "ECS.h"

class ComponentDataContainer
{
public:
	virtual ~ComponentDataContainer() = default;
};


template <typename T> 
class ComponentData : public ComponentDataContainer
{
public:

	void SetEntityDatafromArray(Entity entity, T& data)		// references here references there, dont see nice python syntax anywhere
	{
		if (!componentAvail[entity])	// if component is added to a new entity, add a new entry to the dense array
		{
			componentDense.emplace_back(data);
			denseIndex.emplace_back(entity);
			componentIndex[entity] = componentDense.size() - 1;	
			componentAvail[entity] = true;					// componentIndex value at entity index points to where it is stored in dense array 
		}
		componentDense[componentIndex[entity]] = data;
	}

	void SetEntityUsageFlag(Entity entity, bool flag)
	{
		componentAvail[entity] = flag;
	}

	T& GetEntityDatafromArray(Entity entity)
	{
		return componentDense[componentIndex[entity]];
	}

	std::vector<Entity>& GetDenseIndex()
	{
		return denseIndex;
	}

	std::vector<T>& GetDense()
	{
		return componentDense;
	}

private:
	std::array<int, MAX_ENTITIES> componentIndex{};
	std::array<bool, MAX_ENTITIES> componentAvail{};
	std::vector<Entity> denseIndex;
	std::vector<T> componentDense;
};

