#include "raylib.h"
#include <iostream>
#include <vector>
#include <enet/enet.h>

typedef enum GameState  {
            PlayerWait, 
            Playing, 
            Refreshing
        } GameState; 

class GuessBox{
    private:
    
    public:
        int width; 
        int height; 

        int positionX; 
        int positionY;

        std::string word; 
        bool clicked;

        bool visible;

        GuessBox(int w, int h, int posX, int posY, std::string wo);

        void drawBox();

        bool getClicked();

        std::string getWord();
};

class SubmitBar {
    private:
        int width; 
        int height; 

        int positionX; 
        int positionY;

        double startTime;
    public:
        bool gameStarted;
        
        int guessesLeft; 

        SubmitBar(int w, int h, int posX, int posY);
        void drawBar();
}; 

class Game {
private:

    bool guessRight;

    int screenWidth; 
    int screenHeight; 

    int numGuessRemaining; /*The numer of WORDS left to guess*/

    SubmitBar submitBar;

    bool boxUpdated;
    
public:
    bool won;

     ENetPacket * packet;
     
    std::vector<std::string> allWords;

    std::vector<std::string> correct1; 
    std::vector<std::string> correct2; 
    std::vector<std::string> correct3; 
    std::vector<std::string> correct4; 



    std::vector<GuessBox*> guessBoxes; 

    //The words that you have guessed
    std::vector<std::string> guessWords; 

    GameState gameState;

    Game(int w, int h);
    ~Game();

    void draw();

    void drawLogin();

    void drawPlay();

    void drawRefresh();

    void updateBoxes();

    void reset();
};

