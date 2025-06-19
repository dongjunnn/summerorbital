#include "TitleState.h"
#include "PlayState.h"
#include "../../Components/Components.h"


void TitleState::onEnter(Client& client)
{
	Entity playButton = scene.CreateEntity();		
	scene.AddComponent<TransformComponent>(playButton);
	scene.AddComponent<SpriteComponent>(playButton);
	scene.AddComponent<ClickableComponent>(playButton);

	TransformComponent playbtnloc = { Vector2D(280, 320) };
	SpriteComponent playbtnspr = { client.assets->GetTexture("playButton"), {0,0, 480, 160}, {280, 320, 240, 80} };
	ClickableComponent playbtnclk = { playbtnspr.dstRect, [&client]() 
		{ 
			client.changeState(new PlayState()); 
		} };

	scene.SetEntityData<TransformComponent>(playButton, playbtnloc);
	scene.SetEntityData<SpriteComponent>(playButton, playbtnspr);
	scene.SetEntityData<ClickableComponent>(playButton, playbtnclk);

}

void TitleState::handleInput(Client& client)
{
	while (SDL_PollEvent(&client.event))		// checking for input events
	{
		if (client.event.type == SDL_QUIT)
			client.isRunning = false;

		clickableSystem.ProcessClicks(client.event);	// processing them yay
	}
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
	// nothing for now
}

void TitleState::render(Client& client)
{
	SDL_Renderer* renderer = client.renderer;
	SDL_RenderClear(renderer);

	SDL_RenderCopy(renderer, client.assets->GetTexture("titleScreen"), NULL, NULL);	// for background

	renderSystem.render(renderer);													// renders entities 

	SDL_RenderPresent(renderer);
}

void TitleState::onExit(Client& client)
{

}