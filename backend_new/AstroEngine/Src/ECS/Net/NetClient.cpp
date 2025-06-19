#include "NetClient.h"
#include <cstring>

NetClient::NetClient() {
    enet_initialize();
    client = enet_host_create(nullptr, 1, 2, 0, 0); // 1 peer, 2 channels
}

NetClient::~NetClient() {
    if (client) enet_host_destroy(client);
    enet_deinitialize();
}

bool NetClient::connectToServer(const char* ip, uint16_t port) {
    ENetAddress address;
    enet_address_set_host(&address, ip);
    address.port = port;

    server = enet_host_connect(client, &address, 2, 0);
    return server != nullptr;
}

void NetClient::sendInput(const InputState& input) {
    uint8_t data[5] = {input.up, input.down, input.left, input.right, input.shoot};
    ENetPacket* packet = enet_packet_create(data, 5, ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(server, 0, packet);
}

bool NetClient::receiveGameState(GameState& state) {
    ENetEvent event;
    while (enet_host_service(client, &event, 0) > 0) {
        if (event.type == ENET_EVENT_TYPE_RECEIVE) {
            size_t count = event.packet->dataLength / sizeof(PlayerState);
            PlayerState* players = reinterpret_cast<PlayerState*>(event.packet->data);
            state.players = std::vector<PlayerState>(players, players + count);
            enet_packet_destroy(event.packet);
            return true;
        }
    }
    return false;
}

void NetClient::service() {
    enet_host_service(client, nullptr, 0);
}
