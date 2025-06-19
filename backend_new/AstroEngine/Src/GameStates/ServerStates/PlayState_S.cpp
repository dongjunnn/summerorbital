#include "PlayState_S.h"
#include "../../Server.h"

void PlayState_S::onEnter(Server& server)
{
	map = new Map("terrain", 1, 32);
	map->LoadMap("assets/map.map", scene, 25, 20);

}

void PlayState_S::handleEnetEvent(Server& server, ENetEvent& event)
{
    switch (event.type)
    {
    case ENET_EVENT_TYPE_CONNECT:
    {
        std::cout << "[SERVER] A new client connected from "
            << event.peer->address.host << ":" << event.peer->address.port << std::endl;
    
        // realise this implementation loses global transform scaling, can do somthing about that if need be
        Entity newPlayer = scene.CreatePlayer({ Vector2D{400,320} }, { Vector2D {32,32} });

        event.peer->data = (void*)((uintptr_t) newPlayer );     // this could be a problem if a player dies
        std::cout << "[SERVER] Created player with ID " << newPlayer << " for the new client." << std::endl;

        break;
    }

    case ENET_EVENT_TYPE_RECEIVE:
    {
        std::cout << "[SERVER] Received a packet of length "
            << event.packet->dataLength << std::endl;
        // This is where you will process player input
        Entity playerID = (uintptr_t)event.peer->data;

        PlayerInputPacket input;
        memcpy(&input, event.packet->data, sizeof(PlayerInputPacket));

        // Update the player's velocity based on the input
        VelocityComponent& vel = scene.GetEntityData<VelocityComponent>(playerID); 
   
        vel.velocity = Vector2D(0, 0); // Reset velocity; rip syntax, maybe velocity component could have just been
        if (input.up) vel.velocity.y = -1;      // x,y instead of a Vec2D
        if (input.down) vel.velocity.y = 1;
        if (input.left) vel.velocity.x = -1;
        if (input.right) vel.velocity.x = 1;

        enet_packet_destroy(event.packet);
        break;
    }

    case ENET_EVENT_TYPE_DISCONNECT:
    {
        std::cout << "[SERVER] A client disconnected." << std::endl;
        // Later, this is where you will destroy the player entity for this client
        if (event.peer->data != NULL) {
            Entity playerID = (uintptr_t)event.peer->data;
            scene.DestroyEntity(playerID); // Remove the player entity from the ECS
            std::cout << "[SERVER] Destroyed player with ID " << playerID << "." << std::endl;
        }
        break;
    }
    }
}

void PlayState_S::update(Server& server)
{
    movementSystem.update();		// movement system updates the position of all entities with a transform and velocity
    collisionSystem.updateColliderPositions();
    collisionSystem.checkCollision();
    collisionSystem.resolveCollision();		// resolve collisions pushes back entities if they overlap in movementsystem
    entityCleanSystem.cleanProjectiles();
}

void PlayState_S::broadcastStates(Server& server)
{
    // This function will eventually package up the game state (like entity positions)
    // and send it to all clients. For now, we can leave it empty.
    std::vector<PlayerState> playerStates;

    auto& players = scene.GetView<PlayerComponent>();
    for (Entity player : players)
    {
        PlayerState state;
        state.entityID = player;
        state.x = scene.GetEntityData<TransformComponent>(player).position.x;   // rip 
        state.y = scene.GetEntityData<TransformComponent>(player).position.y;   // its cache miss time
        playerStates.push_back(state);                                          // damn this ecs needs work
    }

    // Only send a packet if there are players to update
    if (!playerStates.empty())
    {
        // Create an ENet packet.
        // The data is the start of our vector, and the length is the total size in bytes.
        // ENET_PACKET_FLAG_RELIABLE means the packet is guaranteed to arrive.
        ENetPacket* packet = enet_packet_create(playerStates.data(),
            playerStates.size() * sizeof(PlayerState),
            ENET_PACKET_FLAG_RELIABLE);

        // Send the packet to all connected clients on channel 0.
        enet_host_broadcast(server.getServerHost(), 0, packet);
    }
}

void PlayState_S::onExit(Server& server)
{
    delete map;
}


