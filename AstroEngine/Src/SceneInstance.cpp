#include "SceneInstance.h"

// Client-side implementation (generates a new local ID)
Entity SceneInstance::CreateProjectile(const Vector2D position, const Vector2D velocity, const SpriteComponent sprite)
{
    Entity proj = CreateEntity();

    // Step 1: Add the components (one argument)
    AddComponent<TransformComponent>(proj);
    AddComponent<VelocityComponent>(proj);
    AddComponent<SpriteComponent>(proj);
    AddComponent<ProjectileComponent>(proj);

    // Step 2: Set the data for each component
    SetEntityData<TransformComponent>(proj, { position });
    SetEntityData<VelocityComponent>(proj, { velocity });
    SetEntityData<SpriteComponent>(proj, sprite);

    // Update the views so systems can see this new entity
    ComponentBitSet newSig = GetEntitySignature(proj);
    UpdateViews(proj, ComponentBitSet{}, newSig);

    return proj; // Return the new client-side ID
}

// Server-side implementation (generates a new server ID)
Entity SceneInstance::CreateProjectile(const Vector2D position, const Vector2D velocity, const Vector2D colDimensions)
{
    Entity proj = CreateEntity();

    // Step 1: Add the components
    AddComponent<TransformComponent>(proj);
    AddComponent<VelocityComponent>(proj);
    AddComponent<ColliderComponent>(proj);
    AddComponent<ProjectileComponent>(proj);

    // Step 2: Set the data
    SetEntityData<TransformComponent>(proj, { position });
    SetEntityData<VelocityComponent>(proj, { velocity });
    ColliderComponent col = { { 
        static_cast<int>(position.x), 
        static_cast<int>(position.y), 
        static_cast<int>(colDimensions.x), 
        static_cast<int>(colDimensions.y) 
    } };
    SetEntityData<ColliderComponent>(proj, col);

    ComponentBitSet newSig = GetEntitySignature(proj);
    UpdateViews(proj, ComponentBitSet{}, newSig);

    return proj;
}

Entity SceneInstance::CreatePlayer(const Vector2D position, const SpriteComponent sprite)
{
	Entity player = CreateEntity();

	AddComponent<TransformComponent>(player);
	AddComponent<VelocityComponent>(player);
	AddComponent<SpriteComponent>(player);
	AddComponent<ColliderComponent>(player);
	AddComponent<PlayerComponent>(player);	// just a flag for now

	TransformComponent tfm = { Vector2D { position.x, position.y } };
	ColliderComponent col = { { position.x, position.y, sprite.dstRect.w, sprite.dstRect.h } };

	SetEntityData<TransformComponent>(player, tfm);
	SetEntityData<VelocityComponent>(player, { Vector2D {0,0} });
	SetEntityData<SpriteComponent>(player, sprite);
	SetEntityData<ColliderComponent>(player, col);

	ComponentBitSet newSig = GetEntitySignature(player);		// updating views
	UpdateViews(player, ComponentBitSet{}, newSig);

	return player;
}

Entity SceneInstance::CreatePlayer(const Vector2D position, const Vector2D colDimensions)
{
	Entity player = CreateEntity();

	AddComponent<TransformComponent>(player);
	AddComponent<VelocityComponent>(player);
	AddComponent<ColliderComponent>(player);
	AddComponent<PlayerComponent>(player);	// just a flag for now

	TransformComponent tfm = { Vector2D { position.x, position.y } };
	ColliderComponent col = { { position.x, position.y, colDimensions.x, colDimensions.y } };

	SetEntityData<TransformComponent>(player, tfm);
	SetEntityData<VelocityComponent>(player, { Vector2D {0,0} });
	SetEntityData<ColliderComponent>(player, col);

	ComponentBitSet newSig = GetEntitySignature(player);		// updating views
	UpdateViews(player, ComponentBitSet{}, newSig);

	return player;
}


