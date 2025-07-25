#pragma once
#include "../../Events/EventManager.h"
#include "../../SceneInstance.h"
#include "../../Components/Components.h"
#include "../../ClientData.h"
#include "../../Packet.h"
#include <enet/enet.h>

enum class Phase
{
	Empty,
	Waiting,
	Countdown,
	Match,
	MatchOver
};

class MatchPhase_S
{
public:
	MatchPhase_S(SceneInstance& sceneRef, ClientData& clientData, EventManager* eventManager) 
		: scene(sceneRef), clientData(clientData), eventManager(eventManager) 
	{
		// guess we can connect it here, didn't really think about the integration part with the events lol
		eventManager->connect<PlayerDiedEvent, MatchPhase_S>(&MatchPhase_S::onPlayerDefeated, this);
		eventManager->connect<PlayerJoinedEvent, MatchPhase_S>(&MatchPhase_S::onPlayerConnect, this);
		eventManager->connect<PlayerDisconnectEvent, MatchPhase_S>(&MatchPhase_S::onPlayerDisconnect, this);
	}

	~MatchPhase_S() {}

	void changePhase(Phase gamePhase)
	{
		currentPhase = gamePhase;
		broadcast = true;
	}

	void onPlayerConnect(PlayerJoinedEvent& event)	// only call this when players join
	{
		playerCount += 1;

		switch (playerCount)
		{
		case 1:
			changePhase(Phase::Waiting);
			break;
		case 2:
			changePhase(Phase::Countdown);

			startCountdown();
			std::cout << "[SERVER] All players connected, game starting in 3 sec" << std::endl;
			break;
		}
	}

	void onPlayerDisconnect(PlayerDisconnectEvent& event)	// only call this when players leave or join
	{
		playerCount -= 1;

		switch (playerCount)
		{
		case 0:
			changePhase(Phase::Empty);
			break;
		case 1:
			changePhase(Phase::Waiting);
			break;
		}
	}

	void updateCountdownTimer()
	{
		// lazy
		if (countdownStarted && currentPhase == Phase::Countdown)
		{	
			uint32_t elapsed = SDL_GetTicks() - start;
			if (elapsed > timerDuration)
			{
				countdownStarted = false;

				std::cout << "[SERVER] Starting match" << std::endl;
				resetPlayingField();

				changePhase(Phase::Match);
			}
		}
		else if (countdownStarted && currentPhase == Phase::MatchOver)
		{
			uint32_t elapsed = SDL_GetTicks() - start;
			if (elapsed > timerDuration)
			{
				countdownStarted = false;

				std::cout << "[SERVER] Restarting countdown" << std::endl;

				changePhase(Phase::Countdown);
				startCountdown(); // very lazy
			}
		}
	}

	// connected to the player died event
	void onPlayerDefeated(PlayerDiedEvent& event)	
	{								
		defeated = clientData.getPlayerRefbyID(event.id);

		changePhase(Phase::MatchOver);
		startCountdown();
	}
	
	void broadcastPhase(ENetHost* serverHost)
	{
		uint8_t code = 0;
		switch (currentPhase)
		{
		case Phase::Empty:
		{
			return;
		}
		case Phase::Waiting:
		{
			code = 1;
			break;
		}
		case Phase::Countdown:
		{
			code = 2;
			break;
		}
		case Phase::Match:
		{
			code = 3;
			break;
		}
		case Phase::MatchOver:
		{
			if (defeated == 1)
			{
				code = 4;
				std::cout << "[SERVER] Player #2 won" << std::endl;
			}
			else if (defeated == 2)
			{
				code = 5;
				std::cout << "[SERVER] Player #1 won" << std::endl;
			}
			defeated = 0;
			break;
		}
		}

		PacketMatchPhase data;
		data.code = code;

		ENetPacket* packet = enet_packet_create(&data, sizeof(PacketMatchPhase), ENET_PACKET_FLAG_RELIABLE);

		// sending match state packet to all connected clients on channel 4
		std::cout << "sending match state packet" << std::endl;
		enet_host_broadcast(serverHost, 4, packet);
	}

	bool& getBroadcast()
	{
		return broadcast;
	}

	Phase getCurrentPhase() const
	{
		return currentPhase;
	}

private:
	SceneInstance& scene;
	ClientData& clientData;
	EventManager* eventManager = nullptr;

	Phase currentPhase = Phase::Empty;
	int playerCount = 0;
	int defeated = 0;

	bool broadcast = false;
	bool restartSequenceStarted = false;

	// countdown timer stuff
	u_int timerDuration = 3000;

	bool countdownStarted = false;
	uint32_t start;

	void startCountdown()
	{
		start = SDL_GetTicks();
		countdownStarted = true;
	}

	void resetPlayingField()
	{
		scene.events()->broadcast<CullProjectilesEvent>();

		Entity p1 = clientData.getPlayerIDbyRef(1);
		Entity p2 = clientData.getPlayerIDbyRef(2);

		scene.GetEntityData<TransformComponent>(p1).position = Vector2D(192.0f, 576.0f);
		scene.GetEntityData<TransformComponent>(p2).position = Vector2D(592.0f, 128.0f);

		scene.GetEntityData<VelocityComponent>(p1).velocity = Vector2D(0.0f, 0.0f);
		scene.GetEntityData<VelocityComponent>(p2).velocity = Vector2D(0.0f, 0.0f);

		scene.GetEntityData<HealthComponent>(p1).hp = 100;
		scene.GetEntityData<HealthComponent>(p2).hp = 100;

		scene.GetEntityData<PlayerComponent>(p1).isAlive = true;
		scene.GetEntityData<PlayerComponent>(p2).isAlive = true;

		scene.GetEntityData<RotationComponent>(p1).angle = 0;
		scene.GetEntityData<RotationComponent>(p2).angle = 180;
	}
};