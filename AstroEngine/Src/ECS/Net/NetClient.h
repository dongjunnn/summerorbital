#pragma once
#include <enet/enet.h>
#include "Protocol.h"

class NetClient {
public:
    NetClient();
    ~NetClient();

    bool connectToServer(const char* ip, uint16_t port);
    void sendInput(const InputState& input);
    bool receiveGameState(GameState& state); // returns false if no state yet
    void service();

private:
    ENetHost* client = nullptr;
    ENetPeer* server = nullptr;
};
