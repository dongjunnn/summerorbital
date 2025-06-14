#include "NetServer.h"
#include <cstring>

NetServer::NetServer() {
    enet_initialize();
}

NetServer::~NetServer() {
    if (server) enet_host_destroy(server);
    enet_deinitialize();
}

bool NetServer::start(uint16_t port) {
    ENetAddress address = {};
    address.host = ENET_HOST_ANY;
    address.port = port;

    server = enet_host_create(&address, 32, 2, 0, 0); // up to 32 clients
    return server != nullptr;
}

std::vector<std::pair<int, InputState>> NetServer::receiveInputs() {
    std::vector<std::pair<int, InputState>> result;
    ENetEvent event;
    while (enet_host_service(server, &event, 0) > 0) {
        if (event.type == ENET_EVENT_TYPE_CONNECT) {
            peerToID[event.peer] = nextID++;
        } else if (event.type == ENET_EVENT_TYPE_RECEIVE) {
            InputState input;
            memcpy(&input, event.packet->data, sizeof(InputState));
            result.emplace_back(peerToID[event.peer], input);
            enet_packet_destroy(event.packet);
        }
    }
    return result;
}

void NetServer::broadcastState(const GameState& state) {
    ENetPacket* packet = enet_packet_create(
        state.players.data(),
        state.players.size() * sizeof(PlayerState),
        ENET_PACKET_FLAG_RELIABLE
    );
    enet_host_broadcast(server, 0, packet);
}

void NetServer::service() {
    enet_host_service(server, nullptr, 0);
}
