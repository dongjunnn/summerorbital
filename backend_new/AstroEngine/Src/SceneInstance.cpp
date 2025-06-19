#include "SceneInstance.h"

Entity SceneInstance::CreateProjectile(const Vector2D position, const Vector2D velocity, const SpriteComponent sprite)
{
	Entity proj = CreateEntity();

	AddComponent<TransformComponent>(proj);
	AddComponent<VelocityComponent>(proj);
	AddComponent<SpriteComponent>(proj);
	AddComponent<ColliderComponent>(proj);
	AddComponent<ProjectileComponent>(proj);	// currently just a flag, will add effects later

	TransformComponent tfm = { Vector2D { position.x, position.y } };
	VelocityComponent vel = { Vector2D { velocity.x, velocity.y } };
	ColliderComponent col = { { position.x, position.y, sprite.dstRect.w, sprite.dstRect.h } };

	SetEntityData<TransformComponent>(proj, tfm);
	SetEntityData<VelocityComponent>(proj, vel);
	SetEntityData<SpriteComponent>(proj, sprite);
	SetEntityData<ColliderComponent>(proj, col);

	ComponentBitSet newSig = GetEntitySignature(proj);		// updating views
	UpdateViews(proj, ComponentBitSet{}, newSig);			// many problems in rendering are because its not in a view

	return proj;
}

Entity SceneInstance::CreateProjectile(const Vector2D position, const Vector2D velocity, const Vector2D colDimensions)
{
	Entity proj = CreateEntity();

	AddComponent<TransformComponent>(proj);
	AddComponent<VelocityComponent>(proj);
	AddComponent<ColliderComponent>(proj);
	AddComponent<ProjectileComponent>(proj);	// currently just a flag, will add effects later

	TransformComponent tfm = { Vector2D { position.x, position.y } };
	VelocityComponent vel = { Vector2D { velocity.x, velocity.y } };
	ColliderComponent col = { { position.x, position.y, colDimensions.x, colDimensions.y } };

	SetEntityData<TransformComponent>(proj, tfm);
	SetEntityData<VelocityComponent>(proj, vel);
	SetEntityData<ColliderComponent>(proj, col);

	ComponentBitSet newSig = GetEntitySignature(proj);		// updating views
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


