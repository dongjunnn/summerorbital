#include <iostream>
#include <enet/enet.h>

int main(int argc, char ** argv) 
{
    if (enet_initialize() != 0) 
    {
        fprintf(stderr, "An error occurred while initializing ENet!\n");
        return EXIT_FAILURE;
    }
    atexit(enet_deinitialize);

    ENetHost* client;
    client = enet_host_create(NULL, 1, 1, 0, 0);

    if (clinet == NULL)
    {
        fprintf(stderr, "An error occured while creating the client");
        return EXIT_FAILURE;
    }

    ENetAddress address;
    ENetEvent event;
    ENetPeer* peer; // enet peer is the host i think

    enet_address_set_host(&address, "127.0.0.1");
    address.port = 7777;

    peer = enet_host_connect(client, &address, 1, 0);
    if (peer == NULL) 
    {
        fprintf(stderr, "No available peers for initiating an ENet connection."); 
        // This function sends formatted text to the given file stream ie the error stream which
        // is separated from the standard output stream
        return EXIT_FAILURE;
    }

    if (enet_host_service(client, &event, 5000) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT) 
    {
        puts("Connection to 127.0.0.1:7777 succeeded");
    } 
    else 
    {
        enet_peer_reset(peer);
        puts("Connection to 127.0.0.1:7777 failed");
        return EXIT_SUCCESS;
    }

    // GAME LOOP START

    while (enet_host_service(client, &event, 10) > 0) // pass by pointer with &
    {
        switch(event.type)
        {
            case ENET_EVENT_TYPE_RECEIVE:
                
        }
    }
    // GAME LOOP END


}