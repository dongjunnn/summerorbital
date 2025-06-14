#include <iostream>
#include <enet/enet.h>
#include <cstdio>
#include "chat_screen.hpp"
#include <string>
#include <pthread.h> 

static ChatScreen chatScreen;

void SendPacket(ENetPeer* peer, const char* data)
{
    ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
}

void* MsgLoop(void* arg)
{
    ENetHost* client = static_cast<ENetHost*>(arg);
    while (true) 
    {
        ENetEvent event;
        while (enet_host_service(client, &event, 0)>0)
        {
            switch (event.type)
            {
                case ENET_EVENT_TYPE_RECEIVE:
                    printf("A packet of length %zu containing %s was received from %x:%u on channel %u.\n",
                        event.packet->dataLength,
                        event.packet->data,
                        event.peer->address.host,
                        event.peer->address.port,
                        event.channelID);
                enet_packet_destroy(event.packet);
                break;
            }
        } 
    }
}

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

    if (client == NULL)
    {
        fprintf(stderr, "An error occurred while creating the client");
        return EXIT_FAILURE;
    }

    ENetAddress address;
    ENetEvent event;
    ENetPeer* peer;

    enet_address_set_host(&address, "127.0.0.1");
    address.port = 7777;

    peer = enet_host_connect(client, &address, 1, 0);
    if (peer == NULL) 
    {
        fprintf(stderr, "No available peers for initiating an ENet connection."); 
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

    SendPacket(peer, "this is some test data");

    chatScreen.Init();
    // create thread for receiving data
    pthread_t thread;
    pthread_create(&thread, NULL, MsgLoop, client);

    // GAME LOOP START
    // while (enet_host_service(client, &event, 100000) > 0)
    // {
    //     switch(event.type)
    //     {
    //         case ENET_EVENT_TYPE_RECEIVE:
    //             printf("A packet of length %zu containing %s was received from %x:%u on channel %u.\n",
    //                 event.packet->dataLength,
    //                 event.packet->data,
    //                 event.peer->address.host,
    //                 event.peer->address.port,
    //                 event.channelID);
    //             break;
    //     }
    // }
    // GAME LOOP END
    

    // while (enet_host_service(client, &event, 3000) > 0)
    // {
    //     switch (event.type)
    //     {
    //         case ENET_EVENT_TYPE_RECEIVE:
    //             enet_packet_destroy(event.packet);
    //             break;
        
    //         case ENET_EVENT_TYPE_DISCONNECT:
    //             puts("Disconnection succeeded");
    //             break;
    //     }
    // }



    while (true)
    {
        std::string msg = chatScreen.CheckBoxInput();
        chatScreen.PostMessage("username", msg.c_str());
    }

    pthread_join(thread, NULL);

    enet_peer_disconnect(peer, 0);  

    return EXIT_SUCCESS;
}
