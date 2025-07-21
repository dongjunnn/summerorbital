#include <cstdlib> // For std::getenv
#include "TitleState.h"
#include "PlayState.h"
#include "../../Components/Components.h"

void TitleState::onEnter(Client& client)
{
	Entity addressField = scene.CreateEntity();
	scene.AddComponent<TransformComponent>(addressField);
	scene.AddComponent<SpriteComponent>(addressField);
	scene.AddComponent<LabelComponent>(addressField);

	TransformComponent addrloc = { Vector2D(120 ,320) };
	SpriteComponent addrspr = { client.assets->GetTexture("addressField"), { 0, 0, 544, 96 }, {120, 320, 544, 96} };

	scene.SetEntityData<TransformComponent>(addressField, addrloc);
	scene.SetEntityData<SpriteComponent>(addressField, addrspr);
	
	const char* server_host_env = std::getenv("SERVER_HOST");
	std::string default_address = server_host_env ? std::string(server_host_env) : "127.0.0.1";

	scene.SetEntityData<LabelComponent>(addressField, { default_address,
														client.assets->GetFont("KennyFuture_48"),
														SDL_Color {255,255,255,255},
														false, 160, 20, true });

	scene.AddUIElement("addressField", addressField);


	Entity playButton = scene.CreateEntity();		
	scene.AddComponent<TransformComponent>(playButton);
	scene.AddComponent<SpriteComponent>(playButton);
	scene.AddComponent<ClickableComponent>(playButton);

	TransformComponent playbtnloc = { Vector2D(280, 448) };
	SpriteComponent playbtnspr = { client.assets->GetTexture("playButton"), {0, 0, 480, 160}, {280, 448, 240, 80} };
	ClickableComponent playbtnclk = { playbtnspr.dstRect, [&client, this, addressField]() 
		{ 
			SDL_StopTextInput();
			std::string address = scene.GetEntityData<LabelComponent>(addressField).text;
			client.changeState(new PlayState(address)); 
		} };

	scene.SetEntityData<TransformComponent>(playButton, playbtnloc);
	scene.SetEntityData<SpriteComponent>(playButton, playbtnspr);
	scene.SetEntityData<ClickableComponent>(playButton, playbtnclk);

	if (status == 1)
	{
		Entity connectFailedMsg = scene.CreateEntity();
		
		scene.AddComponent<TransformComponent>(connectFailedMsg);
		scene.AddComponent<SpriteComponent>(connectFailedMsg);

		TransformComponent connectionErrorloc = { Vector2D(180, 560) };
		SpriteComponent connectionErrorspr = { client.assets->GetTexture("connectionFailedMsg"), {0, 0, 432, 40}, { 180, 560, 432, 40 } };
	
		scene.SetEntityData<TransformComponent>(connectFailedMsg, connectionErrorloc);
		scene.SetEntityData<SpriteComponent>(connectFailedMsg, connectionErrorspr);

		scene.AddUIElement("connectFailed", connectFailedMsg);
	
	}

	SDL_StartTextInput();
}

void TitleState::handleInput(Client& client)
{
	Entity addressField = scene.GetUIElement("addressField");

	SDL_PollEvent(&client.event);
	if (client.event.type == SDL_QUIT)
	{
		client.isRunning = false;
	}

	clickableSystem.ProcessClicks(client.event);	// processing them yay
	clickableSystem.ProcessAddressInput(client.event);
}

void TitleState::handleEnetEvent(Client& client, ENetEvent& event)
{
    switch (event.type) {
    case ENET_EVENT_TYPE_CONNECT:
        std::cout << "[CLIENT] Connection to server succeeded." << std::endl;
        break;
    case ENET_EVENT_TYPE_RECEIVE:
	{ 
		// nothing for now
	}  

    case ENET_EVENT_TYPE_DISCONNECT:
	{
		// nothing for now
	}
    }
}

void TitleState::update(Client& client)
{
	if (status == 1)
	{
		uiSystem.displayConnectionErrorMsg();
	}
}

void TitleState::render(Client& client)
{
	SDL_Renderer* renderer = client.renderer;
	SDL_RenderClear(renderer);

	SDL_RenderCopy(renderer, client.assets->GetTexture("titleScreen"), NULL, NULL);	// for background

	renderSystem.render(renderer);													// renders entities 
	renderSystem.renderText(renderer);												// renders text on top

	SDL_RenderPresent(renderer);
}

void TitleState::onExit(Client& client)
{

}