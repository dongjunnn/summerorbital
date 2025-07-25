#include "SceneInstance.h"
#include <algorithm>

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
    SetEntityData<ProjectileComponent>(proj, { 20 });        // dmg can change ltr

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
    SetEntityData<ProjectileComponent>(proj, { 20 });    // dmg can change ltr

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
	AddComponent<PlayerComponent>(player);	// is alive bool
    //AddComponent<AnimationComponent>(player); // only for the client 
    AddComponent<HealthComponent>(player);      // yay health component
    AddComponent<RotationComponent>(player);    

	TransformComponent tfm = { Vector2D { position.x, position.y } };
	ColliderComponent col = { { position.x, position.y, sprite.dstRect.w, sprite.dstRect.h } };
    /* AnimationComponent ani = {{{{std::string("idle"), {0,3,100}},
                                   {std::string("walk"), { 1,8,100 } } } } }; */

	SetEntityData<TransformComponent>(player, tfm);
	SetEntityData<VelocityComponent>(player, { Vector2D {0,0} });
	SetEntityData<SpriteComponent>(player, sprite);
	SetEntityData<ColliderComponent>(player, col);
    SetEntityData<PlayerComponent>(player, { true });
    //SetEntityData<AnimationComponent>(player, ani);   // not for now
    SetEntityData<HealthComponent>(player, {100} );
    SetEntityData<RotationComponent>(player, { 0.0f });

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
    AddComponent<HealthComponent>(player); 
    AddComponent<RotationComponent>(player);    

	TransformComponent tfm = { Vector2D { position.x, position.y } };
	ColliderComponent col = { { position.x, position.y, colDimensions.x, colDimensions.y } };

	SetEntityData<TransformComponent>(player, tfm);
	SetEntityData<VelocityComponent>(player, { Vector2D {0,0} });
	SetEntityData<ColliderComponent>(player, col);
    SetEntityData<PlayerComponent>(player, { true });
    SetEntityData<HealthComponent>(player, { 100 });
    SetEntityData<RotationComponent>(player, { 0.0f });

	ComponentBitSet newSig = GetEntitySignature(player);		// updating views
	UpdateViews(player, ComponentBitSet{}, newSig);

	return player;
}

Entity SceneInstance::CreateUIHealthBorder(const Vector2D position, const SpriteComponent sprite)
{
    Entity uiBorder = CreateEntity();

    AddComponent<TransformComponent>(uiBorder);
    AddComponent<SpriteComponent>(uiBorder);

    SetEntityData<TransformComponent>(uiBorder, { position });
    SetEntityData<SpriteComponent>(uiBorder, sprite);

    ComponentBitSet newSig = GetEntitySignature(uiBorder);		// updating views
    UpdateViews(uiBorder, ComponentBitSet{}, newSig);

    return uiBorder;
}

Entity SceneInstance::CreateUIHealthMeter(const Vector2D position, const SpriteComponent sprite)
{
    Entity uiMeter = CreateEntity();

    AddComponent<TransformComponent>(uiMeter);
    AddComponent<SpriteComponent>(uiMeter);

    SetEntityData<TransformComponent>(uiMeter, { position });
    SetEntityData<SpriteComponent>(uiMeter, sprite);

    ComponentBitSet newSig = GetEntitySignature(uiMeter);		// updating views
    UpdateViews(uiMeter, ComponentBitSet{}, newSig);

    return uiMeter;
}

Entity SceneInstance::CreateUITextField(const Vector2D position, std::string text, TTF_Font* font)
{
    Entity textField = CreateEntity();
    AddComponent<TransformComponent>(textField);
    AddComponent<LabelComponent>(textField);

    SetEntityData<TransformComponent>(textField, { position });
    SetEntityData<LabelComponent>(textField, LabelComponent(text, font));

    ComponentBitSet newSig = GetEntitySignature(textField);		// updating views
    UpdateViews(textField, ComponentBitSet{}, newSig);
    
    return textField;
}

Entity SceneInstance::CreateUIButton(const Vector2D position, const ClickableComponent click, const std::string text, TTF_Font* font, const SpriteComponent sprite)
{
    Entity button = CreateEntity();
    AddComponent<TransformComponent>(button);
    AddComponent<LabelComponent>(button);
    AddComponent<ClickableComponent>(button);
    AddComponent<SpriteComponent>(button);

    SetEntityData<TransformComponent>(button, { position });
    SetEntityData<LabelComponent>(button, LabelComponent(text, font));
    SetEntityData<ClickableComponent>(button, click);
    SetEntityData<SpriteComponent>(button, sprite);

    ComponentBitSet newSig = GetEntitySignature(button);		// updating views
    UpdateViews(button, ComponentBitSet{}, newSig);

    return button;
}

// adds entity to scene UI element map
void SceneInstance::AddUIElement(std::string id, Entity entity)
{
    uiElementMap.emplace(id, entity);
}

void SceneInstance::RemoveUIElement(std::string id)
{
    uiElementMap.erase(id);
}

bool SceneInstance::IsUIElement(std::string id)
{
    return uiElementMap.count(id);
}

// get entity by UI element map id
Entity SceneInstance::GetUIElement(std::string id)
{
    if (uiElementMap.find(id) != uiElementMap.end()) {
        return uiElementMap[id];
    }
    else {
        std::cerr << "[Client] UIElement does not exist" << std::endl;
        return 0;
    }
}

std::vector<Entity>& SceneInstance::GetDeletionQueue()
{
    return deletionQueue;
}

void SceneInstance::AppendDeletionQueue(Entity entity)
{
    if (std::find(deletionQueue.begin(), deletionQueue.end(), entity) != deletionQueue.end()) { std::cout << "duplicate rejected" << std::endl; return; }
    
    deletionQueue.emplace_back(entity);
}

void SceneInstance::ClearDeletionQueue()
{
    deletionQueue.clear();
}

