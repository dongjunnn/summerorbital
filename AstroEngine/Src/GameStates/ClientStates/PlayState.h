#pragma once
#include "../../Client.h"
#include "ClientState.h"
#include "../../Systems/Systems.h"
#include <map>   
#include "../../Map.h"
#include "../../ECS/ECS.h" 
#include <unordered_map>

class Map;

class PlayState : public ClientState
{
public:
	PlayState() : ClientState() {};

	void onEnter(Client& client) override;
	void handleInput(Client& client) override;
	void handleEnetEvent(Client& client, ENetEvent& event) override;
	void update(Client& client) override;
	void render(Client& client) override;
	void onExit(Client& client) override;

private:
	Map* map;

	std::unordered_map<std::uint32_t, Entity> serverToClientEntityMap; // to match server id to entity of client i think

};