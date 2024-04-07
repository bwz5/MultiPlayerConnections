#include <iostream>
#include <enet/enet.h>
#include <vector>

typedef enum GameState  {
            PlayerWait, 
            Playing, 
            Refreshing
        } GameState; 

class Game {
private:

    int numPlayers;
public:
     int refreshingCount;
     
    GameState gameState;

    /*The vector of current words in the game. Each group of 4 is a connection.*/
    std::vector<std::string> words; 

    ENetPacket* packet; 

    Game();

    void addPlayer();

    void removePlayer();

    void updateWordsList(std::vector<std::string> w);

    int getNumPlayers();

    int getGameState();

};