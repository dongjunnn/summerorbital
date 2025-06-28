#pragma once
#include <enet/enet.h>
#include <unordered_map>
#include "Protocol.h"

class NetServer {
public:
    NetServer();
    ~NetServer();

    bool start(uint16_t port);
    std::vector<std::pair<int, InputState>> receiveInputs();
    void broadcastState(const GameState& state);
    void service();

private:
    ENetHost* server = nullptr;
    std::unordered_map<ENetPeer*, int> peerToID;
    int nextID = 1;
};
