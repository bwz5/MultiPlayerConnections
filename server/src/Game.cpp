#include "Game.h"

Game::Game(){
    gameState = PlayerWait; 

    numPlayers = 0;

    packet = nullptr;

    words.push_back("DKE\0");
    words.push_back("Museum\0");
    words.push_back("BossBaby\0");
    words.push_back("Spy\0");
    words.push_back("Stacks\0");
    words.push_back("Crane\0");
    words.push_back("Car\0");
    words.push_back("ROR\0");
    words.push_back("Dog Bite\0");
    words.push_back("Marauder\0");
    words.push_back("HomeAlone\0");
    words.push_back("Legos\0");
    words.push_back("Hikes\0");
    words.push_back("Garage\0");
    words.push_back("Observatory\0");
    words.push_back("McDonalds\0");

    refreshingCount = 0;

}

void Game::addPlayer(){
    numPlayers += 1; 

    if (numPlayers ==  2){
        gameState = Playing;
    }
}

void Game::removePlayer(){
    numPlayers -= 1;
}

void Game::updateWordsList(std::vector<std::string> w){
    words = w;
}

int Game::getNumPlayers(){
    return numPlayers;
}

int Game::getGameState(){
    return gameState;
}