#pragma once
#include "../../SceneInstance.h"
#include "../../Components/Components.h"
#include <SDL.h>

class MatchPhase
{
public:
	MatchPhase(SceneInstance& sceneRef) 
		: scene(sceneRef) {}

	void updateServerMsg(uint8_t code)
	{
		Entity serverMsg = scene.GetUIElement("ServerMsg");
		LabelComponent& label = scene.GetEntityData<LabelComponent>(serverMsg);

		switch (code)
		{
		case 1:		// one player in room, waiting
			label.text = "Waiting for Player #2";
			label.colour = { 255, 255, 255, 255 };
			label.toUpdate = true;	// yes this is extremely manual
			break;
		case 2:		// both players here, countdown started
			label.text = "Starting in 3";
			label.colour = { 255, 255, 255, 255 };
			label.toUpdate = true;

			start = SDL_GetTicks();
			countdownStarted = true;
			break;
		case 3:
			label.text = "GLHF";
			label.colour = { 255, 255, 255, 255 };
			label.toUpdate = true;
			break;
		case 4:
			label.text = "Player #2 wins!";
			label.colour = { 255, 71, 76, 255 };
			label.toUpdate = true;
			break;
		case 5:
			label.text = "Player #1 wins!";
			label.colour = { 173, 216, 230, 255 };
			label.toUpdate = true;
			break;
		}
	}

	void updateCountdownTimer()
	{
		if (countdownStarted)	// this part quite lazily put together
		{						// id know, cuz i did it
			uint32_t elapsed = SDL_GetTicks() - start;
			if (elapsed > timerDuration)
			{
				countdownStarted = false;
			} 
			else if (elapsed > 2000)
			{
				Entity serverMsg = scene.GetUIElement("ServerMsg");
				LabelComponent& label = scene.GetEntityData<LabelComponent>(serverMsg);

				label.text = "Starting in 1";
				label.toUpdate = true;
			}
			else if (elapsed > 1000)
			{
				Entity serverMsg = scene.GetUIElement("ServerMsg");
				LabelComponent& label = scene.GetEntityData<LabelComponent>(serverMsg);

				label.text = "Starting in 2";
				label.toUpdate = true;
			}
		}
	}

private:
	SceneInstance& scene;

	// timer stuff
	u_int timerDuration = 2900;

	bool countdownStarted = false;
	uint32_t start = 0;
};