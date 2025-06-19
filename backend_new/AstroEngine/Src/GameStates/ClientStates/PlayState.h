#pragma once
#include "../../Client.h"
#include "ClientState.h"
#include "../../Map.h"

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
};