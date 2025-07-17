#pragma once
#include <array>
#include <vector>
#include <map>
#include <algorithm>
#include "ECS.h"

class ComponentDataContainer
{
public:
	virtual ~ComponentDataContainer() = default;
	virtual void RemoveEntity(Entity entity) = 0;
	virtual const type_info& getType() = 0;	// debug
	virtual void printEntityData(Entity entity) = 0; //debug
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
		if (componentIndex[entity] >= componentDense.size())
		{
			std::cerr << "[OOPS] Catastrophic error!" << std::endl;
			std::cout << "Entity " << entity << " tried to access something it didnt have" << std::endl;
			std::cout << "componentIndex[entity]: " << componentIndex[entity] << std::endl;
			std::cout << "componentDense size: " << componentDense.size() << std::endl;
			throw std::runtime_error("oops");
		}
		return componentDense[componentIndex[entity]];
	}
	
	void DeleteEntityDatafromArray(Entity entity)
	{	/*
		auto it = std::find(denseIndex.begin(), denseIndex.end(), entity);	// deletion index in dense
		if (it == denseIndex.end()) { std::cerr << "Deletion index error" << std::endl; return; }

		// swap sparse stuff
		componentIndex[denseIndex.back()] = componentIndex[entity];
		componentIndex[entity] = 0;
		componentAvail[entity] = false;

		// swap dense stuff
		std::swap(denseIndex[it - denseIndex.begin()], denseIndex.back());
		std::swap(componentDense[it - denseIndex.begin()], componentDense.back());
		
		// pop off last value of dense
		denseIndex.pop_back();
		componentDense.pop_back(); */

		auto it = std::find(denseIndex.begin(), denseIndex.end(), entity);
		if (it == denseIndex.end()) {
			std::cerr << "Deletion index error" << std::endl;
			return;
		}

		size_t indexToDelete = it - denseIndex.begin();
		size_t lastIndex = denseIndex.size() - 1;
		Entity lastEntity = denseIndex[lastIndex];

		// Move last entity's data to the slot being deleted
		componentDense[indexToDelete] = componentDense[lastIndex];
		denseIndex[indexToDelete] = lastEntity;

		// Update the moved entity's sparse index
		componentIndex[lastEntity] = indexToDelete;

		// Remove last element
		componentDense.pop_back();
		denseIndex.pop_back();

		// Invalidate the deleted entity
		componentAvail[entity] = false;
		componentIndex[entity] = 0;
	}

	void RemoveEntity(Entity entity) override
	{
		DeleteEntityDatafromArray(entity);
	}

	std::vector<Entity>& GetDenseIndex()
	{
		return denseIndex;
	}

	std::vector<T>& GetDense()
	{
		return componentDense;
	}

	const type_info& getType() override		// debug
	{
		return typeid(T);
	}
	
	void printEntityData(Entity entity) override
	{
		if (componentIndex[entity] >= componentDense.size()) std::cout << "[DEBUG] * This entity does not map to dense \n";
		else std::cout << GetEntityDatafromArray(entity) << std::endl;
	}

private:
	std::array<int, MAX_ENTITIES> componentIndex{};
	std::array<bool, MAX_ENTITIES> componentAvail{};
	std::vector<Entity> denseIndex;
	std::vector<T> componentDense;
};


