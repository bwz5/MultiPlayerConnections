#include <iostream>
#include <enet/enet.h>
#include <vector> 
#include "Game.h"

int main() {
    
    Game game;

    /*Initialize ENet*/

    if (enet_initialize () != 0)
    {
        printf("An error occurred while initializing ENET\n");
        return EXIT_FAILURE;
    } else {
        printf("Successfully initialized ENET\n");

    atexit (enet_deinitialize); // Cleanup 

    ENetAddress address;
    ENetHost * server;
    ENetEvent event;

    int eventStatus;

    /* Bind the server to the default localhost.     */
    /* A specific host address can be specified by   */
    /* enet_address_set_host (& address, "x.x.x.x"); */
    address.host = ENET_HOST_ANY;

    /* Bind the server to port 1234. */
    address.port = 1234;

    server = enet_host_create (& address /* the address to bind the server host to */, 
                                2      /* allow up to 32 clients and/or outgoing connections */,
                                2      /* allow up to 2 channels to be used, 0 and 1 */,
                                0      /* assume any amount of incoming bandwidth */,
                                0      /* assume any amount of outgoing bandwidth */);
    if (server == NULL)
    {
        printf ("An error occurred while trying to create an ENet server host.\n");
        exit (EXIT_FAILURE);
    }
  
    while (1) {
        eventStatus = enet_host_service(server, &event, 1000);

        int currentIdx = 1; 

        // If we had some event that interested us
        if (eventStatus > 0) {
            switch(event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    printf("(Server) We got a new connection from %x\n",
                            event.peer->address.host);
                    
                    game.addPlayer();
                    break;

                case ENET_EVENT_TYPE_RECEIVE:
                    printf("(Server) Message from client : %s\n", event.packet->data);

                    /*We will receive a game state change */

                    switch (event.packet->data[0]){
                        case PlayerWait:
                            game.gameState = PlayerWait;
                            break;
                        case Refreshing:
                            game.refreshingCount += 1; 
                            if (game.refreshingCount >= 1){
                                game.gameState = Refreshing;
                            }
                            break;
                    }
                    
                    break;

                case ENET_EVENT_TYPE_DISCONNECT:
                    game.removePlayer();

                    printf("%s disconnected.\n", event.peer->data);

                    // Reset client's information
                    event.peer->data = NULL;
                    break;

            }
        }

        if (game.gameState == Playing){
            char message[200];

            message[0] = Playing;
            
            for (auto i : game.words){
                message[currentIdx] = i.length() + 1;

                currentIdx += 1; 
                for (int j = 0; j < i.length(); j++){
                    message[currentIdx] = i[j];
                    
                    currentIdx += 1;
                }
                message[currentIdx] = '\0';
                currentIdx += 1;
            }
            game.packet = enet_packet_create(message,currentIdx+1,ENET_PACKET_FLAG_RELIABLE);
            enet_host_broadcast(server,0,game.packet);
        }
        if (game.gameState == Refreshing){
            char buffer[1];

            buffer[0] = Refreshing;

            game.packet = enet_packet_create(buffer,1,ENET_PACKET_FLAG_RELIABLE);

            enet_host_broadcast(server,0,game.packet);
        }
    }

    enet_host_destroy(server); // For the very end

    return 0;
    }
}