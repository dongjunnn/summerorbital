#pragma once
#include <bitset>
#include <array>
#include <unordered_map>
#include <any>
#include <memory>
#include <typeindex>
#include <algorithm>
#include "ECS.h"
#include "ComponentDataContainer.h"

// ComponentManager creates, destroys components, and assigns, unassigns components from entities
// unfortunately this system is not cache friendly, random lookups when systems look through dense arrays :(
// maybe next time can design a better one

using ComponentBitSet = std::bitset<MAX_COMPONENTS>;
using ComponentDataPtr = std::shared_ptr<ComponentDataContainer>;	// something something inheritence array something

class ComponentManager
{
public:
	
	ComponentManager(){}

	template<typename T> void CreateComponent()  // creates new component
	{
		if (nextComponentID >= MAX_COMPONENTS)
		{
			std::cerr << "Max Components Reached" << std::endl;
			throw std::runtime_error("Too many components");
		}
		std::cout << "Registering a component" << std::endl;
		ComponentTypeIDMap[std::type_index(typeid(T))] = nextComponentID;
		ComponentArray[nextComponentID] = std::make_shared<ComponentData<T>>();
		++nextComponentID;
	}

	template<typename T> ComponentID GetComponentIDByType()
	{
		// std::cout << "here at getbytype: " << typeid(T).name() << std::endl;
		return ComponentTypeIDMap.at(std::type_index(typeid(T)));
	}

	template<typename T> void AddComponent(Entity entity)	// adds component to entity
	{
		ComponentID id = GetComponentIDByType<T>();			// sets the bitset for that particular component and entity
		EntityComponents[entity].set(id);
	}																				

	template<typename T> bool HasComponent(Entity entity)
	{
		ComponentID index = GetComponentIDByType<T>();
		return EntityComponents[entity][index];
	}

	template<typename T> void SetEntityData(Entity entity, T data)
	{
		GetComponentData<T>()->SetEntityDatafromArray(entity, data);
	}

	template<typename T> T& GetEntityData(Entity entity)	
	{
		return GetComponentData<T>()->GetEntityDatafromArray(entity);
	}

	template<typename T> std::vector<T>& GetComponentUserData()
	{
		return GetComponentData<T>()->GetDense();
	}

	template<typename T> std::vector<Entity>& GetComponentUserID()
	{
		return GetComponentData<T>()->GetDenseIndex();
	}

	template<typename... T> ComponentBitSet BuildComponentSignature()
	{
		if (sizeof...(T) == 0) { 
			std::cout << "Building signature with no components!" << std::endl; 
		}
		// std::cout << "getting sig" << std::endl;
		ComponentBitSet signature{};
		(signature.set(GetComponentIDByType<T>()), ...);
		return signature;
	}

	template<typename... T> ComponentID minVectorLength(T... vectors)
	{
		std::array<size_t, sizeof...(vectors)> sizes = { vectors.size()... };
		std::array<ComponentID, sizeof...(vectors)> id = { GetComponentIDByType<T>()... };

		return id[std::distance(sizes.begin(), std::min_element(sizes.begin(), sizes.end()))];
	}

	const ComponentBitSet& GetEntitySignature(Entity entity)
	{
		return EntityComponents[entity];
	}

	void RemoveAllData(Entity entity)							//  removes all component data from entity
	{
		EntityComponents[entity].reset();						//  don't actually have to overwrite the data, system treats it as no components
	}

private:
	std::array<ComponentBitSet, MAX_ENTITIES> EntityComponents;			// stores data on which entity has what component, in a bitset
	std::unordered_map<std::type_index, ComponentID> ComponentTypeIDMap;	// connects Component type to a unique ID
	std::array<ComponentDataPtr, MAX_COMPONENTS> ComponentArray;		// array of pointers pointing to arrays containing data
	std::array<int, MAX_COMPONENTS> ComponentUsage{};					// not used for now
	
	ComponentID nextComponentID = 0;

	template<typename T> std::shared_ptr<ComponentData<T>> GetComponentData()
	{
		return std::static_pointer_cast<ComponentData<T>>(ComponentArray[GetComponentIDByType<T>()]);
	}

};
