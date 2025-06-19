// #include "Game.h"

// Game *game = nullptr;

// int main(int argc, char *argv[])
// {

// 	const int FPS = 60;
// 	const int frameDelay = 1000 / FPS;

// 	Uint32 frameStart;
// 	int frameTime;

// 	game = new Game();
// 	game->init("GameWindow", 800, 640, false);

// 	while (game->running())
// 	{

// 		frameStart = SDL_GetTicks();

// 		game->handleEvents();
// 		game->update();
// 		game->render();

// 		frameTime = SDL_GetTicks() - frameStart;
// 		if (frameDelay > frameTime) 
// 		{
// 			SDL_Delay(frameDelay - frameTime);
// 		}

// 	}

// 	game->clean();
// 	return 0;
// }

#include <iostream>
#include "Server.h" 
#include "Client.h" 
#include <cstring>

#include <enet/enet.h>

// We no longer need to include Game.h here, as the
// Client and Server classes manage their own needs.

int main(int argc, char* argv[]) {
    // 1. Check for command-line arguments
    bool isServer = false;
    if (argc > 1 && strcmp(argv[1], "server") == 0) {
        isServer = true;
    }

    // 2. Conditionally launch the correct application
    if (isServer) {
        std::cout << "Starting application in SERVER mode..." << std::endl;
        Server server;
        if (server.init()) {
            server.run(); // The server has its own internal loop
        }
    } else {
        std::cout << "Starting application in CLIENT mode..." << std::endl;
        Client client;
        if (client.init()) {
            client.run(); // The client has its own internal loop
        }
    }

    // Notice the old game loop is gone from main. It now lives
    // inside the run() methods of the Server and Client classes.
    return 0;
}