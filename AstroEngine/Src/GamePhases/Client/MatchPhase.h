#pragma once
#include "../../SceneInstance.h"
#include "../../Components/Components.h"
#include <SDL.h>

class MatchPhase
{
public:
	MatchPhase(SceneInstance& sceneRef, EventManager* eventManager) 
		: scene(sceneRef), eventManager(eventManager) 
	{
		eventManager->connect<MatchRestartEvent, MatchPhase>(&MatchPhase::onPlayAgain, this);
	}

	void updateServerMsg(uint8_t code)
	{
		Entity serverMsg = scene.GetUIElement("ServerMsg");
		LabelComponent& label = scene.GetEntityData<LabelComponent>(serverMsg);

		Entity scoreCount = scene.GetUIElement("ScoreCounter");
		LabelComponent& scorelbl = scene.GetEntityData<LabelComponent>(scoreCount);
		Vector2D& scorePos = scene.GetEntityData<TransformComponent>(scoreCount).position;

		switch (code)
		{
		case 1:		// one player in room, waiting
		
			label.update(
				"Waiting for Player #2",
				{ 255, 255, 255, 255 }		// not so manual anymore
			);

			scorePos = { 352, 3 };
			scorelbl.update(
				"Lobby",
				{ 255, 255, 255, 255 }
			);

			setPlayAgainBtn(false);

			// to interrupt countdown timer if active
			if (countdownStarted) countdownStarted = false;	

			// if we reach here the scores should always be reset
			p1_score = p2_score = 0;

			break;
		case 2:		// both players here, countdown started
			label.update(
				"Starting in 3",
				{ 255, 255, 255, 255 }		
			);

			start = SDL_GetTicks();
			countdownStarted = true;

			break;
		case 3:
			label.update(
				"GLHF",
				{ 255, 255, 255, 255 }
			);
			
			scorePos = { 368, 3 };
			scorelbl.update(
				std::to_string(p1_score) + " : " + std::to_string(p2_score),
				{ 255, 255, 255, 255 }
			);

			break;
		case 4:
			label.update(
				"Player #2 wins!",
				{ 255, 71, 76, 255 }
			);

			p2_score += 1;

			scorelbl.update(
				std::to_string(p1_score) + " : " + std::to_string(p2_score),
				{ 255, 255, 255, 255 }
			);

			break;
		case 5:
			label.update(
				"Player #1 wins!",
				{ 173, 216, 230, 255 }
			);

			p1_score += 1;

			scorelbl.update(
				std::to_string(p1_score) + " : " + std::to_string(p2_score),
				{ 255, 255, 255, 255 }
			);
			break;
		case 6:
			scorePos = { 280, 3 };

			// turn on play again button
			setPlayAgainBtn(true);

			// update score label and turn off server msg
			if (p1_score > p2_score)
			{
				label.update(
					"",
					{ 173, 216, 230, 255 }
				);

				scorelbl.update(
					"Player 1 wins ",
					{ 173, 216, 230, 255 }
				);
			}
			else if (p2_score > p1_score)
			{
				label.update(
					"",
					{ 255, 71, 76, 255 }
				);

				scorelbl.update(
					"Player 2 wins ",
					{ 255, 71, 76, 255 }
				);
			}
			else { std::cerr << "[CLIENT WARN] p1 and p2 scores equal at match end" << std::endl; }
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

	void onPlayAgain(MatchRestartEvent& event)
	{
		playAgain = true;
		setPlayAgainBtn(false);

		p1_score = p2_score = 0;

		Entity serverMsg = scene.GetUIElement("ServerMsg");
		LabelComponent& label = scene.GetEntityData<LabelComponent>(serverMsg);

		label.update(
			"Waiting for players",
			{ 255, 255, 255, 255 }		
		);
	}

	bool& getPlayAgain()
	{
		return playAgain;
	}

private:
	SceneInstance& scene;
	EventManager* eventManager = nullptr;

	// timer stuff
	u_int timerDuration = 2900;

	bool countdownStarted = false;
	uint32_t start = 0;

	// an event handler would be less janky
	bool playAgain = false; 

	// lazy to add server score packet
	// so client infers the score
	// < win condition settings >
	
	int p1_score = 0;
	int p2_score = 0;

	void setPlayAgainBtn(bool state)
	{
		// true for on, false for off

		Entity playAgainBtn = scene.GetUIElement("PlayAgainBtn");
		ClickableComponent& click = scene.GetEntityData<ClickableComponent>(playAgainBtn);
		SpriteComponent& spr = scene.GetEntityData<SpriteComponent>(playAgainBtn);
		LabelComponent& lbl = scene.GetEntityData<LabelComponent>(playAgainBtn);

		if (state)
		{
			click.isActive = true;
			spr.alpha = 255;
			lbl.update("Play Again");
		}
		else
		{
			click.isActive = false;
			spr.alpha = 0;
			lbl.update("");
		}
	}
};