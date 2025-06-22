#include "../../Components/Components.h"
#include "PlayState.h"
#include "../../Packet.h" 
#include "../../Map.h"
#include <cstring>

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

    if (keystates[SDL_SCANCODE_SPACE]) input.fireButtonPressed = true;
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
        // Check for projectile creation
        if (event.packet->dataLength == sizeof(PacketProjectileCreated))
        {
            PacketProjectileCreated projectilePacket;
            memcpy(&projectilePacket, event.packet->data, sizeof(PacketProjectileCreated));

            // Create the sprite for the projectile
            SpriteComponent projectileSprite { 
                client.assets->GetTexture("orb"), 
                {0,0,16,16}, 
                {(int)projectilePacket.position.x, (int)projectilePacket.position.y, 16, 16} 
            };
            
            // 1. Create a projectile locally, which gives us a NEW client-side ID
            Entity clientID = scene.CreateProjectile(projectilePacket.position, projectilePacket.velocity, projectileSprite);

            // 2. Store the mapping from the server's ID to our new client ID
            serverToClientEntityMap[projectilePacket.entityID] = clientID;
        }
        // Check for game state updates
        else if (event.packet->dataLength > 0 && event.packet->dataLength % sizeof(PlayerState) == 0)
        {
            std::vector<PlayerState> playerStates(event.packet->dataLength / sizeof(PlayerState));
            memcpy(playerStates.data(), event.packet->data, event.packet->dataLength);

            for (const auto& serverState : playerStates) {
                // 3. Check if we have a mapping for this server ID
                if (serverToClientEntityMap.count(serverState.entityID)) {
                    Entity clientID = serverToClientEntityMap.at(serverState.entityID);
                    TransformComponent& transform = scene.GetEntityData<TransformComponent>(clientID);
                    transform.position.x = serverState.x;
                    transform.position.y = serverState.y;
                } else {
                    // This is a new player we haven't seen. Create it and store the mapping.
                    SpriteComponent playerSprite { 
                        client.assets->GetTexture("player"),
                        {0,0,32,32},
                        {(int)serverState.x, (int)serverState.y, 32, 32}
                    };
                    Entity clientID = scene.CreatePlayer({serverState.x, serverState.y}, playerSprite);
                    serverToClientEntityMap[serverState.entityID] = clientID;
                }
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
	// entityCleanSystem.cleanProjectiles();
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
