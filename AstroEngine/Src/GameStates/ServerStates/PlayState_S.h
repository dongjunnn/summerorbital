#pragma once
#include "ServerState.h"
#include "../../Map.h"


class PlayState_S : public ServerState
{
public:
	void onEnter(Server& server) override;
	void handleEnetEvent(Server& server, ENetEvent& event) override;
	void update(Server& server) override;
	void broadcastStates(Server& server) override;
	void onExit(Server& server) override;

	void handleUserMovementInput(Entity playerID, const PlayerInputPacket& input);
	void handleUserFiring(Entity playerID, const PlayerInputPacket& input, Server& server);

private:
	Map* map;
	std::vector<Entity> delProj;
	std::vector<Entity> delPlyr;
	std::vector<Entity> delMisc;
};