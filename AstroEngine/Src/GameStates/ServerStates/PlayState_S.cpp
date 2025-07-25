#include "PlayState_S.h"
#include "../../Server.h"
#include "../../Map.h"
#include <cstring>
#include <cmath> 

void PlayState_S::onEnter(Server& server)
{
	map = new Map("terrain", 1, 32);
	map->LoadMap("assets/map.map", scene, 25, 20);

    scene.hookEventSystem(server.getEvents());  // event system is not mandatory
    entityCleanSystem.hookEvents(); // maybe better system one day
    matchPhase = new MatchPhase_S(scene, clientData, server.getEvents());
}

void PlayState_S::handleEnetEvent(Server& server, ENetEvent& event)
{
    switch (event.type)
    {
    case ENET_EVENT_TYPE_CONNECT:
    {
        std::cout << "[SERVER] A new client connected from "
            << event.peer->address.host << ":" << event.peer->address.port << std::endl;

        // creating new player serverside
        Entity newPlayer = scene.CreatePlayer({ Vector2D{ 400,320} }, { Vector2D {32,32} });

        event.peer->data = (void*)((uintptr_t)newPlayer);     // this could be a problem if a player dies
        std::cout << "[SERVER] Created player with ID " << newPlayer << " for the new client." << std::endl;

        // Updating player reference (who is player #1 etc) 
        clientData.assignPlayer(newPlayer); // TODO connect client data to events
        server.getEvents()->broadcast<PlayerJoinedEvent>(newPlayer);

        // packaging new player data 
        PlayerState state;
        state.entityID = newPlayer;
        state.x = scene.GetEntityData<TransformComponent>(newPlayer).position.x;   // rip 
        state.y = scene.GetEntityData<TransformComponent>(newPlayer).position.y;   // its cache miss time
        state.angle = scene.GetEntityData<RotationComponent>(newPlayer).angle;

        state.health = scene.GetEntityData<HealthComponent>(newPlayer).hp;
        state.colour = clientData.getPlayerRefbyID(newPlayer);
        std::cout << state.colour << std::endl;

        ENetPacket* packet = enet_packet_create(&state,
            sizeof(PlayerState),
            ENET_PACKET_FLAG_RELIABLE);

        // Sending packet to new client on channel 4
        enet_peer_send(event.peer, 4, packet);

        break;
    }

    case ENET_EVENT_TYPE_RECEIVE:
    {
        switch (event.channelID)
        {
        case 0:
        {
            Entity playerID = (uintptr_t)event.peer->data;

            PlayerInputPacket input;
            memcpy(&input, event.packet->data, sizeof(PlayerInputPacket));

            bool& isAlive = scene.GetEntityData<PlayerComponent>(playerID).isAlive;

            if (isAlive)    // they can move and shoot only when they are alive
            {
                handleUserMovementInput(playerID, input);
                handleUserFiring(playerID, input, server);
            }

            enet_packet_destroy(event.packet);
            break;
        }
        case 4:
        {
            if (event.packet->dataLength == sizeof(PacketMatchRestart))
            {   
                Entity playerID = (uintptr_t)event.peer->data;

                PacketMatchRestart packet;
                memcpy(&packet, event.packet->data, sizeof(PacketMatchRestart));

                if (packet.code == 1) 
                { 
                    scene.events()->broadcast<MatchRestartEvent>(playerID); 
                }
            }
        }
        break;
        }
        break;
    }

    case ENET_EVENT_TYPE_DISCONNECT:
    {
        std::cout << "[SERVER] A client disconnected." << std::endl;
        
        if (event.peer->data != NULL) {
            Entity playerID = (uintptr_t)event.peer->data;
            scene.AppendDeletionQueue(playerID); // Tell other clients it is destroyed
            clientData.disassignPlayer(playerID);  // free up player reference (who is player #1 etc)
            scene.events()->broadcast<PlayerDisconnectEvent>(playerID);
            std::cout << "[SERVER] Destroyed player with ID " << playerID << "." << std::endl;
        }
        break;
    }
    }
}

void PlayState_S::update(Server& server)
{
    // processing game events
    server.getEvents()->processEvents();

    movementSystem.update();		// movement system updates the position of all entities with a transform and velocity
    collisionSystem.updateColliderPositions();
    collisionSystem.checkCollision();
    collisionSystem.resolveCollision();		// resolve collisions pushes back entities if they overlap in movementsystem

    // now clean systems return a list of what they delete, to make sending to client easier
    entityCleanSystem.cleanProjectiles();
    delProj = entityCleanSystem.clearDeletionQueue();     // this collates all deletions now

    // setting defeated players 
    entityCleanSystem.handleDefeatedPlayers();   

    // this is literally only for that one countdown timer
    matchPhase->updateCountdownTimer();
}

void PlayState_S::broadcastStates(Server& server)
{
    // This function will eventually package up the game state (like entity positions)
    // and send it to all clients. For now, we can leave it empty.

    std::vector<PlayerState> playerStates;
    std::vector<ProjectileState> projectileStates;

    auto& players = scene.GetView<PlayerComponent>();
    for (Entity player : players)
    {
        PlayerState state;
        state.entityID = player;
        state.x = scene.GetEntityData<TransformComponent>(player).position.x;   // rip 
        state.y = scene.GetEntityData<TransformComponent>(player).position.y;   // its cache miss time
        state.angle = scene.GetEntityData<RotationComponent>(player).angle;

        state.health = scene.GetEntityData<HealthComponent>(player).hp;
        state.colour = 0;

        playerStates.push_back(state);                                          // damn this ecs needs work
    }

    auto& projectiles = scene.GetView<ProjectileComponent>();
    for (Entity projectile : projectiles)
    {
        ProjectileState state;
        state.projectileID = projectile;
        state.x = scene.GetEntityData<TransformComponent>(projectile).position.x;   // rip 
        state.y = scene.GetEntityData<TransformComponent>(projectile).position.y;   // its cache miss time
        projectileStates.push_back(state);                                          // but really no feels to refactor
    }

    // Only send a packet if there are players to update
    if (!playerStates.empty())      // player states on channel 0
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

    if (!projectileStates.empty())  // projectile states on channel 1
    {
        // Create an ENet packet.
        // The data is the start of our vector, and the length is the total size in bytes.
        // ENET_PACKET_FLAG_RELIABLE means the packet is guaranteed to arrive.
        ENetPacket* packet = enet_packet_create(projectileStates.data(),  
            projectileStates.size() * sizeof(ProjectileState),
            ENET_PACKET_FLAG_RELIABLE);

        // std::cout << "Sending packet: " << projectileStates.size() << std::endl;
        enet_host_broadcast(server.getServerHost(), 1, packet);
    }

    // deletions on channel 3
    if (!(delProj.empty() && delMisc.empty()))
    {
        delProj.insert(delProj.end(), delMisc.begin(), delMisc.end());  // lol yuck 
        ENetPacket* packet = enet_packet_create(delProj.data(),
            delProj.size() * sizeof(PacketProjectileDestroyed),
            ENET_PACKET_FLAG_RELIABLE);
        
        // send the packet to all connected clients on channel 3
        std::cout << "sending deletion packet" << std::endl;
        enet_host_broadcast(server.getServerHost(), 3, packet);
    }

    // match phase communication on channel 4
    bool& broadcastNow = matchPhase->getBroadcast();
    if (broadcastNow)
    { 
        matchPhase->broadcastPhase(server.getServerHost());
        broadcastNow = false;
    }

}

void PlayState_S::onExit(Server& server)
{
    delete map;
}

void PlayState_S::handleUserMovementInput(Entity playerID, const PlayerInputPacket& input)
{
    // Update the player's velocity based on the input
    Vector2D& vel = scene.GetEntityData<VelocityComponent>(playerID).velocity;
    float& plyrAngle = scene.GetEntityData<RotationComponent>(playerID).angle;

    float accel = 0.2f;
    Vector2D normalised = vel.getNormalised();
    Vector2D bias = Vector2D(0.0f, -1.0f).scale(accel);
    float magnitude = vel.magnitude();
    float brake = 0.22f;
    float rot_speed = 0.05f;     // in radians

    // if w is pressed and the increase in velocity is less than the difference between current and max velocities, add velocity
    if (input.up && (MAX_VEL - magnitude >= bias.magnitude()))
    {
        vel = vel + bias.rotateACW(plyrAngle);
    }
    else if (input.up) { vel += bias.scale(MAX_VEL - bias.magnitude()).rotateACW(plyrAngle); }

    // if s is pressed and the decrease in velocity doesnt lead to negative magnitude, decrease velocity
    if (input.down && (magnitude - normalised.scale(brake).magnitude() >= 0.0f))
    {
        vel -= normalised.scale(brake);
    }
    else if (input.down) { vel.x = 0.0f; vel.y = 0.0f; }

    // if a is pressed, rotate clockwise
    if (input.left)
    {
        plyrAngle -= rot_speed;
    }
    if (input.right)
    {
        plyrAngle += rot_speed;
    }

    if (!(input.up))
    {
        Vector2D step = normalised.scale(VEL_DECAY);
        float stepsize = step.magnitude();

        vel -= step;
        if (std::abs(vel.x) < stepsize) vel.x = 0;
        if (std::abs(vel.y) < stepsize) vel.y = 0;
    }
}

void PlayState_S::handleUserFiring(Entity playerID, const PlayerInputPacket& input, Server& server)
{
    if (input.fireButtonPressed) {

        PlayerComponent& playerData = scene.GetEntityData<PlayerComponent>(playerID);
        
        // NOTE: In milliseconds
        const float fireRate = 500.0f; // 0.5 seconds CAN BE CHANGED TODO
        
        // Get the current time
        float currentTime = SDL_GetTicks();
        
        // Check if enough time has passed since the last shot
        if (currentTime > playerData.lastFireTime + fireRate)
        {
            
            playerData.lastFireTime = currentTime;

            TransformComponent& playerTransform = scene.GetEntityData<TransformComponent>(playerID);
            RotationComponent& playerRotation = scene.GetEntityData<RotationComponent>(playerID);
            
            float projectileSpeed = 5.0f;
            float spawnDistance = 40.0f;
            
            float shootingAngle = playerRotation.angle - (M_PI / 2.0f);

            Vector2D projectileVelocity = Vector2D(cos(shootingAngle), sin(shootingAngle)).scale(projectileSpeed);

            /*Vector2D spawnOffset = Vector2D(cos(shootingAngle), sin(shootingAngle)).scale(spawnDistance);
            Vector2D spawnPosition = playerTransform.position + spawnOffset;*/

            Vector2D spawnOffset = Vector2D(cos(shootingAngle), sin(shootingAngle)).scale(spawnDistance);
            Vector2D spawnPosition = (playerTransform.position + Vector2D(16, 16)) + spawnOffset + Vector2D(-3, -3);
            
            Entity newProjectileID = scene.CreateProjectile(spawnPosition, projectileVelocity, { 10, 10 });

            std::cout << "[DEBUG] new projectile id: " << newProjectileID << std::endl;

            PacketProjectileCreated projectilePacket;
            projectilePacket.entityID = newProjectileID;
            projectilePacket.position = spawnPosition;
            projectilePacket.velocity = projectileVelocity;

            ENetPacket* packet = enet_packet_create(&projectilePacket,
                sizeof(PacketProjectileCreated),
                ENET_PACKET_FLAG_RELIABLE);

            enet_host_broadcast(server.getServerHost(), 2, packet);
        }
    }
}
