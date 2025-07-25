#pragma once
#include "../../Events/EventManager.h"
#include "../../SceneInstance.h"
#include "../../Components/Components.h"
#include "../../ClientData.h"
#include "../../Packet.h"
#include "../../ECS/ECS.h"
#include <enet/enet.h>

enum class Phase
{
	Empty,
	Waiting, Lobby,
	Countdown,
	Match,
	MatchOver
};

struct MatchData
{
	using Checklist = std::array<bool, MAX_ACTIVE_PLAYERS>;

	PlayerList players;		// player 0 has entity id at 0, player 1 has entity id at 1, etc
	ScoreList scores;		// player 0 has score at index 0, player 1 has score at index 1, etc
	Checklist signalRestarted;	// player 0 signaled restart at id 0 etc


	// returns array of ids for all players
	PlayerList& getCurrentPlayers()
	{
		return players;
	}

	// returns number of players in position 0 and 1 of players
 	int getActivePlayerCount()
	{
		int count = 0;
		for (int i = 0; i < scores.size(); i++)
		{
			if (players[i]) count += 1;
		}
		return count;
	}


	// registers player id in players
	void addPlayer(Entity id)
	{
		for (int i = 0; i < MAX_PLAYERS; i++)
		{
			if (!players[i])
			{
				players[i] = id;
				std::cout << "adding player" << std::endl;
				return;
			}
		}
	}
	// removes player id in players, also resets scores
	void removePlayer(Entity id)
	{
		auto it = std::find(players.begin(), players.end(), id);
		size_t idx = it - players.begin();

		if (it == players.end())
		{
			std::cerr << "[SERVER WARN] Invalid id to remove from player list" << std::endl;
		}
		else if (idx < scores.size())
		{
			players[idx] = 0;
			resetScores();
			resetRestartSignal();
		}
		else
		{
			players[idx] = 0;
		}
	}

	int getScore(Entity id)
	{
		auto it = std::find(players.begin(), players.end(), id);
		size_t idx = it - players.begin();

		if (it == players.end())
		{
			std::cerr << "[SERVER WARN] Invalid id to increment score" << std::endl;
		}
		else if (idx < scores.size())
		{
			return scores[idx];
		}
		else
		{
			std::cerr << "[SERVER WARN] Trying to get score for spectator" << std::endl;
		}

		return -1;
	}

	// increments score by entity id
	void incrementScore(Entity id)
	{
		auto it = std::find(players.begin(), players.end(), id);
		size_t idx = it - players.begin();

		if (it == players.end())
		{
			std::cerr << "[SERVER WARN] Invalid id to increment score" << std::endl;
		}
		else if (idx < scores.size())
		{
			scores[idx] += 1;
		}
		else
		{
			std::cerr << "[SERVER WARN] Trying to increment score for spectator" << std::endl;
		}
	}

	// increments score by player reference
	void incrementScore(int ref, ClientData& clientData)
	{
		Entity id = clientData.getPlayerIDbyRef(ref);
		if (id)
		{
			incrementScore(id);
		}
	}

	// resets scores to 0 for all active players
	void resetScores()
	{
		std::fill(scores.begin(), scores.end(), 0);
		std::cout << "[SERVER] Scores reset" << std::endl;
	}

	// signals restart intentions, returns true if all players restart
	bool signalRestart(Entity id)
	{
		auto it = std::find(players.begin(), players.end(), id);
		size_t idx = it - players.begin();

		if (it == players.end())
		{
			std::cerr << "[SERVER WARN] Invalid id signaling restart" << std::endl;
		}
		else if (idx < signalRestarted.size())
		{
			signalRestarted[idx] = true;
		}
		else
		{
			std::cerr << "[SERVER WARN] Spectator signaling restart" << std::endl;
		}

		for (int i = 0; i < signalRestarted.size(); i++)
		{
			std::cout << signalRestarted[i] << std::endl;
		}

		if (std::all_of(signalRestarted.begin(), signalRestarted.end(), [](bool b) {return b;}))	// just c++ things 
		{
			resetRestartSignal();
			return true;
		}

		return false;
	}

	// resets signal restart
	void resetRestartSignal()
	{
		std::fill(signalRestarted.begin(), signalRestarted.end(), false);
	}
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
		eventManager->connect<MatchRestartEvent, MatchPhase_S>(&MatchPhase_S::onMatchRestart, this);
	}

	~MatchPhase_S() {}

	void changePhase(Phase gamePhase)
	{
		currentPhase = gamePhase;
		broadcast = true;
	}

	void onPlayerConnect(PlayerJoinedEvent& event)	// only call this when players join
	{
		matchData.addPlayer(event.id);

		switch (matchData.getActivePlayerCount())
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
		matchData.removePlayer(event.id);

		switch (matchData.getActivePlayerCount())
		{
		case 0:
			changePhase(Phase::Empty);
			break;
		case 1:
			changePhase(Phase::Waiting);
			break;
		}

		// to revive dead players on disconnect
		PlayerList& playerlist = matchData.getCurrentPlayers();
		for (int i = 0; i < MAX_ACTIVE_PLAYERS; i++)
		{
			Entity player = playerlist[i];
			if (player)
			{
				scene.GetEntityData<HealthComponent>(player).hp = 100;
				scene.GetEntityData<PlayerComponent>(player).isAlive = true;
			}
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

				if (!matchEnd)	// very very lazy
				{
					std::cout << "[SERVER] Restarting countdown" << std::endl;

					changePhase(Phase::Countdown);
					startCountdown(); // very lazy
				}
				else
				{
					std::cout << "[SERVER] Match Over" << std::endl;

					changePhase(Phase::Lobby);
					matchEnd = false;
				}
				
			}
		}
	}

	// connected to the player died event
	void onPlayerDefeated(PlayerDiedEvent& event)	
	{								
		defeated = clientData.getPlayerRefbyID(event.id);
	
		if (currentPhase == Phase::Match)
		{
			changePhase(Phase::MatchOver);
			startCountdown();
		}
	}
	
	void onMatchRestart(MatchRestartEvent& event)
	{	
		std::cout << "Match Restart event" << std::endl;

		bool restart = matchData.signalRestart(event.id);

		if (restart)
		{
			changePhase(Phase::Countdown);
			startCountdown();
		}
	}

	void broadcastPhase(ENetHost* serverHost)
	{
		uint8_t code = 0;

		Entity p1 = clientData.getPlayerIDbyRef(1);
		Entity p2 = clientData.getPlayerIDbyRef(2);

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
				matchData.incrementScore(p2);
			}
			else if (defeated == 2)
			{
				code = 5;
				std::cout << "[SERVER] Player #1 won" << std::endl;
				matchData.incrementScore(p1);
			}
			defeated = 0;

			if (matchData.getScore(p1) >= winScore || matchData.getScore(p2) >= winScore)
			{
				matchEnd = true;
				std::cout << "[DEBUG] someone won" << std::endl;
			}

			break;
		}
		case Phase::Lobby:
		{
			code = 6;

			if      (matchData.getScore(p2) >= winScore) { std::cout << "[SERVER] Player #2 wins match" << std::endl; }
			else if (matchData.getScore(p1) >= winScore) { std::cout << "[SERVER] Player #1 wins match" << std::endl; }
			
			matchData.resetScores();
			break;
		}
		}

		PacketMatchPhase data;
		data.code = code;

		ENetPacket* packet = enet_packet_create(&data, sizeof(PacketMatchPhase), ENET_PACKET_FLAG_RELIABLE);

		// sending match state packet to all connected clients on channel 4
		std::cout << "sending match state packet " << int(code) << std::endl;
		std::cout << "p1 score: " << matchData.getScore(p1) << " p2 score: " << matchData.getScore(p2) << std::endl;
		enet_host_broadcast(serverHost, 4, packet);
	}

	void broadcastMatchEnd(ENetHost* serverHost)	// this is not a good implementation, but it works enough for now
	{												// this triggers the client to compare the scores locally
		PacketMatchPhase data;					
		uint8_t code = 6;

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

	MatchData matchData;

	Phase currentPhase = Phase::Empty;
	int defeated = 0;

	bool broadcast = false;

	// match settings
	bool matchEnd = false;
	int winScore = 2;

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