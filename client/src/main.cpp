#include <enet/enet.h>
#include <stdio.h>
#include <string.h>
#include "Game.h"
#include "raylib.h"

int main(int argc, char **argv)
{
    Game game(650,600);

    ENetAddress address;
    ENetHost *client;
    ENetPeer *peer;

    ENetEvent event;
    int eventStatus;

    // a. Initialize enet
    if (enet_initialize() != 0) {
        printf("An error occured while initializing ENet.\n");
        return EXIT_FAILURE;
    }

    atexit(enet_deinitialize);

    // b. Create a host using enet_host_create
    client = enet_host_create(NULL, 1, 2, 57600/8, 14400/8);

    if (client == NULL) {
        printf("An error occured while trying to create an ENet server host\n");
        exit(EXIT_FAILURE);
    }

    enet_address_set_host(&address, "localhost");
    address.port = 1234;

    // c. Connect and user service
    peer = enet_host_connect(client, &address, 2, 0);

    if (peer == NULL) {
        printf("No available peers for initializing an ENet connection");
        exit(EXIT_FAILURE);
    }

    eventStatus = 1;

    while (!WindowShouldClose()) {
        game.draw();

        /**Might have to change the time*/

        eventStatus = enet_host_service(client, &event, 10);

        // If we had some event that interested us
        if (eventStatus > 0) {
            switch(event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    printf("(Client) We got a new connection from %x\n",
                            event.peer->address.host);
                    break;

                case ENET_EVENT_TYPE_RECEIVE:
                    printf("(Client) Message from server : %s\n", event.packet->data);
                    // Lets broadcast this message to all

                    switch (event.packet->data[0]){
                        case Playing: {
                            game.gameState = Playing;

                            int currentIdx = 1;
                            if (game.allWords.empty()){                                
                                while (currentIdx < event.packet->dataLength){
                                    int temp = (int)event.packet->data[currentIdx]; 
                                    
                                    currentIdx += 1; 

                                    if (currentIdx >= event.packet->dataLength){
                                        //std::cout << "Crash before at " << currentIdx << std::endl;
                                        break;
                                    }
                                    std::string tempString = "";
                                    for (int i = 0; i < temp; i++){

                                        tempString += event.packet->data[currentIdx];

                                        currentIdx += 1;
                                    }
                                    game.allWords.push_back(tempString);
                                    }
                            }
                            
                            enet_packet_destroy(event.packet);
                            break;
                        }
                        case Refreshing:
                            game.gameState = Refreshing;
                            break;
                    }

                case ENET_EVENT_TYPE_DISCONNECT:
                    printf("(Client) %s disconnected.\n", event.peer->data);

                    // Reset client's information
                    event.peer->data = NULL;
                    break;
            }
        }
        char buffer[1]; 

        buffer[0] = game.gameState;

        std::cout << game.gameState <<  std::endl;

        game.packet = enet_packet_create(buffer,1,ENET_PACKET_FLAG_RELIABLE);
        
        if (game.packet != nullptr){
            enet_peer_send(peer,0,game.packet);
            game.packet = nullptr;
        }
        
    }
}
