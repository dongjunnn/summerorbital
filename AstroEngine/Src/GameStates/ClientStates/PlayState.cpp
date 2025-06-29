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

    initUI(client);
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
                SpriteComponent projectileSprite{
                    client.assets->GetTexture("orb"),
                    {0,0,32,32},
                    {(int)projectilePacket.position.x, (int)projectilePacket.position.y, 32, 32}
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

                    RotationComponent& rot = scene.GetEntityData<RotationComponent>(clientID);
                    rot.angle = serverState.angle;

                    HealthComponent& health = scene.GetEntityData<HealthComponent>(clientID);
                    health.hp = serverState.health;

                }
                else {
                    // This is a new player we haven't seen. Create it and store the mapping.
                    SpriteComponent playerSprite{
                        client.assets->GetTexture("ship"),
                        {0,0,96,96},
                        {(int)serverState.x, (int)serverState.y, 32, 32}
                    };
                    Entity clientID = scene.CreatePlayer({ serverState.x, serverState.y }, playerSprite);
                    scene.AddUIElement("thisPlayer", clientID); // lol player is a UI element, this code is getting messy
                    std::cout << "[CLIENT] New player created with id " << clientID << std::endl;
                    serverToClientEntityMap[serverState.entityID] = clientID;
                }
            }
        }
            
        else if (event.packet->dataLength > 0 && event.packet->dataLength % sizeof(ProjectileState) == 0)
            {
                
                std::vector<ProjectileState> projectileStates(event.packet->dataLength / sizeof(ProjectileState));
                memcpy(projectileStates.data(), event.packet->data, event.packet->dataLength);

                for (const auto& serverState : projectileStates) {
                    // 3. Check if we have a mapping for this server ID
                    if (serverToClientEntityMap.count(serverState.projectileID)) {
                        Entity clientID = serverToClientEntityMap.at(serverState.projectileID);
                        TransformComponent& transform = scene.GetEntityData<TransformComponent>(clientID);
                        transform.position.x = serverState.x;
                        transform.position.y = serverState.y;
                    }
                    else {
                        // This is a new projectile we haven't seen. Create it and store the mapping.

                        SpriteComponent projSprite{
                            client.assets->GetTexture("orb"),
                            {0,0,32,32},
                            {(int)serverState.x, (int)serverState.y, 32, 32}
                        };
                        Entity clientID = scene.CreateProjectile({ serverState.x, serverState.y }, { 0.0f,0.0f }, projSprite);
                        serverToClientEntityMap[serverState.projectileID] = clientID;
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
    animationSystem.PlayerAnim();
    uiSystem.displayHP();
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

void PlayState::initUI(Client& client)
{
    Entity UIHealthBorder = scene.CreateUIHealthBorder({ 0,0 }, { client.assets->GetTexture("healthBorder"), {0,0,224,32}, {0,0,224,32} });
    Entity UIHealthBar = scene.CreateUIHealthMeter({ 0,0 }, { client.assets->GetTexture("healthMeter"), {0,0,224,32}, {0,0,224,32} });

    scene.AddUIElement("HealthBorder", UIHealthBorder);
    scene.AddUIElement("HealthBar", UIHealthBar);
}
