#include "../../Components/Components.h"
#include "PlayState.h"
#include "../../Map.h"

void PlayState::onEnter(Client& client)
{
    std::cout << "[CLIENT] Loading level map" << std::endl;
	map = new Map("terrain", 1, 32);
	map->LoadMap("assets/map.map", scene, *client.assets, 25, 20);
}

void PlayState::handleInput(Client& client)
{
	// --- NEWLY IMPLEMENTED ---
	SDL_PollEvent(&client.event);		

	if (client.event.type == SDL_QUIT)
		client.isRunning = false;
	
	// Create an input packet to send to the server
	PlayerInputPacket input; // From Packet.h

	// Get a snapshot of the current state of all keys
	const Uint8* keystates = SDL_GetKeyboardState(NULL);

	// Populate the packet based on which keys are currently held down
	if (keystates[SDL_SCANCODE_W]) input.up = true;
	if (keystates[SDL_SCANCODE_S]) input.down = true;
	if (keystates[SDL_SCANCODE_A]) input.left = true;
	if (keystates[SDL_SCANCODE_D]) input.right = true;

	// Create an ENet packet to send the input state.
	// The flag '0' (or ENET_PACKET_FLAG_UNSEQUENCED) means this is an unreliable packet.
	// This is good for movement data that is sent every frame.
	ENetPacket* packet = enet_packet_create(&input,
		sizeof(PlayerInputPacket),
		0);

	// Send the input packet to the server on channel 0
	enet_peer_send(client.getServerPeer(), 0, packet);
	
}

void PlayState::handleEnetEvent(Client& client, ENetEvent& event)
{
    switch (event.type) {
    case ENET_EVENT_TYPE_CONNECT:
        std::cout << "[CLIENT] Connection to server game level succeeded." << std::endl;
        break;
    case ENET_EVENT_TYPE_RECEIVE:
    {
        // This is the most important part of the client.
        // We've received a game state packet from the server.

        // 1. Create a vector to hold the list of player states from the packet.
        // We calculate the size of the vector by dividing the total packet size
        // by the size of a single PlayerState object.
        std::vector<PlayerState> playerStates(event.packet->dataLength / sizeof(PlayerState));

        // 2. Copy the raw data from the packet into our vector.
        memcpy(playerStates.data(), event.packet->data, event.packet->dataLength);

        // 3. Process each state update.
        for (const auto& serverState : playerStates) {  // this part is abit iffy
            // Check if we already have a local entity for this player
            if (scene.isActive(serverState.entityID)) {
                Entity localPlayer = serverState.entityID;

                // --- START OF FIX ---
                // Check if the entity has the TransformComponent before trying to get it.
                // This prevents accessing invalid memory if the component is somehow missing.
                if (scene.HasComponent<TransformComponent>(localPlayer)) { //
                    TransformComponent& transform = scene.GetEntityData<TransformComponent>(localPlayer); //
                    transform.position.x = serverState.x; //
                    transform.position.y = serverState.y; //
                }
                else {
                    // This scenario indicates an unexpected state where an existing entity
                    // is missing a critical component. For robustness, we re-add it.
                    std::cerr << "[CLIENT] Warning: Entity ID " << serverState.entityID
                        << " found, but missing TransformComponent. Re-adding it." << std::endl;
                    ComponentBitSet oldSig = scene.GetEntitySignature(localPlayer);
                    scene.AddComponent<TransformComponent>(localPlayer);
                    scene.SetEntityData<TransformComponent>(localPlayer, { Vector2D{serverState.x, serverState.y} } );
                    scene.UpdateViews(localPlayer, oldSig, scene.GetEntitySignature(localPlayer));

                    // Also ensure SpriteComponent is there if it's expected for players
                    if (!scene.HasComponent<SpriteComponent>(localPlayer)) { //
                        ComponentBitSet oldSig = scene.GetEntitySignature(localPlayer);
                        scene.AddComponent<SpriteComponent>(localPlayer);
                        scene.SetEntityData<SpriteComponent>(localPlayer, SpriteComponent
                                                            { client.assets->GetTexture("player"),
                                                              { 0,0,32,32 },
                                                              { (int)serverState.x, (int)serverState.y, 32, 32 }
                                                            });     // sry for syntax, helper function soon
                        scene.UpdateViews(localPlayer, oldSig, scene.GetEntitySignature(localPlayer));
                    }
                }
                // --- END OF FIX ---
            }
            else {
                // If no, this is a new player we haven't seen before. Create a local
                // representation of it.
                std::cout << "[CLIENT] New player detected with ID " << serverState.entityID << ". Creating local entity." << std::endl;

                // Create the entity with the SAME ID as the one on the server
                Entity localPlayer = scene.CreatePlayer({serverState.x, serverState.y}, SpriteComponent
                                                        { client.assets->GetTexture("player"),
                                                          { 0,0,32,32 },
                                                          { (int)serverState.x, (int)serverState.y, 32, 32}
                                                        });  // createPlayer calls update views by itself
            }
        }

        // 4. Destroy the packet when we're done with it.
        enet_packet_destroy(event.packet);
        break;
    }
    case ENET_EVENT_TYPE_DISCONNECT:
        std::cout << "[CLIENT] Disconnected from server." << std::endl;
        // Here you could set a flag to take the player back to a main menu
        break;
    }
}

void PlayState::update(Client& client)	// updating internal state of client
{
	movementSystem.update();		// movement system updates the position of all entities with a transform and velocity
	collisionSystem.updateColliderPositions();
	collisionSystem.checkCollision();
	collisionSystem.resolveCollision();		// resolve collisions pushes back entities if they overlap in movementsystem
	entityCleanSystem.cleanProjectiles();
}

void PlayState::render(Client& client)
{
	SDL_Renderer* renderer = client.renderer;
	SDL_RenderClear(renderer);

	SDL_RenderCopy(renderer, client.assets->GetTexture("gameBackground"), NULL, NULL);	// for background
	renderSystem.render(renderer);

	SDL_RenderPresent(renderer);
}

void PlayState::onExit(Client& client)
{
	delete map;
}
