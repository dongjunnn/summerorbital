#pragma once
#include "ECS/ECS.h"
#include "Components/Components.h"
#include "ECS/EntityManager.h"
#include "ECS/ComponentManager.h"

class Scene			// wrapper class?
{
public:
	
	Scene() = default;

	Entity CreateEntity()
	{
		return entityManager.CreateEntity();
	}

	void DestroyEntity(Entity entity)
	{
		auto& oldSig = componentManager.GetEntitySignature(entity);	// initial signature before erase
		auto newSig = ComponentBitSet {};							// new signature is empty
		UpdateViews(entity, oldSig, newSig);						// applies changes to all views
		componentManager.RemoveAllData(entity);						// entity has blank signature
		entityManager.DestroyEntity(entity);						// frees entity id for reuse
	}

	std::array<bool, MAX_ENTITIES>& GetActiveEntities()
	{
		return entityManager.GetActiveEntities();
	}

	bool isActive(Entity entity)
	{
		return entityManager.isActive(entity);
	}

	template<typename T> void CreateComponent()
	{
		componentManager.CreateComponent<T>();
	}

	template<typename T> void AddComponent(Entity entity)
	{
		componentManager.AddComponent<T>(entity);
	}

	template<typename T> bool HasComponent(Entity entity)
	{
		return componentManager.HasComponent<T>(entity);
	}

	template<typename T> void SetEntityData(Entity entity, T data)
	{
		componentManager.SetEntityData<T>(entity, data);
	}

	template<typename T> T& GetEntityData(Entity entity)
	{
		return componentManager.GetEntityData<T>(entity);
	}

	template<typename T> std::vector<T>& GetCompoenentUserData()
	{
		return componentManager.GetComponentUserData<T>();
	}

	template<typename T> std::vector<Entity>& GetComponentUserID()
	{
		return componentManager.GetComponentUserID<T>();
	}

	template<typename... T> ComponentBitSet BuildComponentSignature()
	{
		return componentManager.BuildComponentSignature<T...>();
	}

	template<typename... T> ComponentID minVectorLength(T&&... vectors)
	{
		return componentManager.minVectorLength(std::forward<T>(vectors)...);
	}

	void RemoveAllData(Entity entity)
	{
		componentManager.RemoveAllData(entity);
	}

	const ComponentBitSet& GetEntitySignature(Entity entity)
	{
		return componentManager.GetEntitySignature(entity);
	}


	// view methods, this was painful //
	template<typename... T> std::vector<Entity>& GetView()
	{	
		ComponentBitSet signature = BuildComponentSignature<T... >();

		// if view is not cached, create it
		if (viewCache.find(signature) == viewCache.end())
		{
			std::cout << "making a view" << std::endl;
			std::vector<Entity> newView;
			auto& activeEntities = GetActiveEntities();

			// this part generates the list of entities for that view
			for (Entity e = 0; e < MAX_ENTITIES; e++)
			{
				if (!activeEntities[e]) continue;

				const ComponentBitSet& entitySig = componentManager.GetEntitySignature(e);
				if ((entitySig & signature) == signature)
				{
					newView.push_back(e);
				}

			}
			viewCache[signature] = std::move(newView);
		}
		return viewCache[signature];
	}

	void UpdateViews(Entity entity, const ComponentBitSet& oldSig, const ComponentBitSet& newSig)
	{
		for (auto& [sig, entities] : viewCache)
		{
			// checks for changes in signature, so entities can be added or removed accordingly
			bool matchedBefore = (oldSig & sig) == sig;
			bool matchesNow = (newSig & sig) == sig;

			if (!matchedBefore && matchesNow)
			{
				entities.push_back(entity);		// adds entity to view if it matches the signature now
			}
			else if (matchedBefore && !matchesNow)
			{
				auto id = std::find(entities.begin(), entities.end(), entity);		// really it should only find one
				if (id != entities.end()) entities.erase(id);
			}
		}
	}

	void RemoveEntityFromViews(Entity entity)
	{
		for (auto& [sig, entities] : viewCache)
		{
			entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
		}
	}

private:
	ComponentManager componentManager;
	EntityManager entityManager;
	std::unordered_map<ComponentBitSet, std::vector<Entity>> viewCache;
};