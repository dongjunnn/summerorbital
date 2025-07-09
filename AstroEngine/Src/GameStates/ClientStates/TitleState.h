#pragma once
#include "../../Client.h"
#include "ClientState.h"

class TitleState : public ClientState
{
public:
	TitleState(const int statusMsg = 0) 
		: ClientState(), status(statusMsg) {};

	void onEnter(Client& client) override;
	void handleInput(Client& client) override;
	void handleEnetEvent(Client& client, ENetEvent& event) override;
	void update(Client& client) override;
	void render(Client& client) override;
	void onExit(Client& client) override;

private:
	int status;	// for connection feedback
};