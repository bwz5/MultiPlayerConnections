#include "Game.h"
#include <random>
#include <set>

bool isClicked(int positionX, int positionY, int width, int height){
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        if (GetMouseX() >= positionX && GetMouseX() <= positionX + width){
            if (GetMouseY() >= positionY && GetMouseY() <= positionY + height){
                return true;
            }
        }
    }
    return false;
}

Game::Game(int w, int h) : submitBar(100,50,200,500){
    screenHeight = h;
    screenWidth = w;

    InitWindow(screenWidth, screenHeight, "Maddie's Game");

    SetTargetFPS(60);   

    gameState = PlayerWait;

    numGuessRemaining = 0;

    guessRight = false;

    boxUpdated = false;

    packet = nullptr;

    won = false;
}

Game::~Game(){
    CloseWindow(); 
}

void Game::draw(){
    BeginDrawing();

        ClearBackground(WHITE);

        switch(gameState){
            case PlayerWait:
                drawLogin();
                break;
            case Playing:
                drawPlay();
                 break;
            case Refreshing:
                drawRefresh();
                 break;
            default:
        }

        if (IsKeyPressed(KEY_SPACE)){
            if (gameState == PlayerWait){
                gameState = Playing;
            }
        }

    EndDrawing();
}

void Game::drawLogin(){
    DrawRectangle(50,220,500,100,PURPLE);
    DrawText("Waiting for Other Player to Join...",100,250,25,WHITE);
}

void GuessBox::drawBox(){
    if (visible){ // Becomes not visible if it has been correctly guessed 
        if (clicked){
            DrawRectangle(positionX,positionY,width,height,DARKGRAY); 
            DrawText(word.c_str(), positionX + 25, positionY + 25, 15,WHITE);
        } else {
            DrawRectangle(positionX,positionY,width,height,LIGHTGRAY); 
            DrawText(word.c_str(), positionX + 25, positionY + 25, 15,BLACK);
        }
    }


}

GuessBox::GuessBox(int w, int h, int posX, int posY, std::string wo){
    width = w; 
    height = h; 
    positionX = posX;
    positionY = posY; 

    word = wo;

    clicked = false; 

    visible = true;
}

bool GuessBox::getClicked(){
    return clicked;
}

std::string GuessBox::getWord(){
    return word;
}

SubmitBar::SubmitBar(int w, int h, int posX, int posY){
    width = w; 
    height = h; 
    positionX = posX; 
    positionY = posY;

    guessesLeft = 4; 

    gameStarted = false; 
}

void SubmitBar::drawBar(){
    if (!gameStarted ){
        startTime = GetTime();
        gameStarted = true;
    }

    for (int i = 0; i < guessesLeft; i++){
        DrawCircle(positionX + i * 30 + 15,positionY, 10,DARKGRAY);
    }

    DrawRectangle(positionX, positionY + 30,width+100,height, PURPLE);
    DrawText("Submit Guess",positionX + 20, positionY + 50, 25,WHITE);

    double drawTime = GetTime() - startTime;
    std::string tempString = std::to_string(drawTime);

    DrawText(tempString.c_str(),  positionX + 250, positionY + 20, 30, BLACK);
}

int generateRandom(){
    return std::rand() % 16;
}

void Game::updateBoxes(){
        
    if (!allWords.empty()){

        correct1.push_back(allWords[0]);
        correct1.push_back(allWords[1]);
        correct1.push_back(allWords[2]);
        correct1.push_back(allWords[3]);
        correct2.push_back(allWords[4]);
        correct2.push_back(allWords[5]);
        correct2.push_back(allWords[6]);
        correct2.push_back(allWords[7]);
        correct3.push_back(allWords[8]);
        correct3.push_back(allWords[9]);
        correct3.push_back(allWords[10]);
        correct3.push_back(allWords[11]);
        correct4.push_back(allWords[12]);
        correct4.push_back(allWords[13]);
        correct4.push_back(allWords[14]);
        correct4.push_back(allWords[15]);
        
        /*TODO: Randomly assign the word to a guess Box*/

        int i = 0; 

        std::set<int> tempSet; 

        while (i < 16){
            int tempIdx = generateRandom();

            if (tempSet.find(tempIdx) == tempSet.end()){
                // This means that the generated number was unique 
                tempSet.emplace(tempIdx);

                guessBoxes[i]->word = allWords[tempIdx];

                i += 1; 
            }

        }

        // for (int i = 0; i < allWords.size(); i++){
        //     guessBoxes[i]->word = allWords[i];
        // }
        boxUpdated = true;
    }
}

void Game::drawPlay(){

    if (guessBoxes.empty()){
        guessBoxes.push_back(new GuessBox(100,50,50,50, "hello"));
        guessBoxes.push_back(new GuessBox(100,50,200,50, "hello"));
        guessBoxes.push_back(new GuessBox(100,50,350,50, "hello"));
        guessBoxes.push_back(new GuessBox(100,50,500,50, "hello"));

        guessBoxes.push_back(new GuessBox(100,50,50,150, "hello"));
        guessBoxes.push_back(new GuessBox(100,50,200,150, "hello"));
        guessBoxes.push_back(new GuessBox(100,50,350,150, "hello"));
        guessBoxes.push_back(new GuessBox(100,50,500,150, "hello"));

        guessBoxes.push_back(new GuessBox(100,50,50,250, "hello"));
        guessBoxes.push_back(new GuessBox(100,50,200,250, "hello"));
        guessBoxes.push_back(new GuessBox(100,50,350,250, "hello"));
        guessBoxes.push_back(new GuessBox(100,50,500,250, "hello"));

        guessBoxes.push_back(new GuessBox(100,50,50,350, "hello"));
        guessBoxes.push_back(new GuessBox(100,50,200,350, "hello"));
        guessBoxes.push_back(new GuessBox(100,50,350,350, "hello"));
        guessBoxes.push_back(new GuessBox(100,50,500,350, "hello"));
    }

    if (!boxUpdated){
        updateBoxes();
    }

    /*Draw the boxes based on whether or not they have the right position*/

    int pX = 50; 
    int pY = 50;

    int idx = 0;
    for (auto i : guessBoxes){
        if (i->visible){
            i->positionX = pX; 
            pX += 150; 
            if (pX > 500){
                pX = 50;
            }
            i->positionY = pY;

            idx += 1; 

            if (idx%4 == 0){
                pY += 100;
            }
            
        }
        else if (!i->visible){
            i->positionX = 900; 
            i->positionY = 900;
        }

        i->drawBox();
    }

    submitBar.drawBar();

    /*Change the color of the guessBox it has been pressed */
    for (auto i : guessBoxes){
        if (isClicked(i->positionX,i->positionY, i->width, i->height)){

            // if (!i->visible){
            //     return;
            // }

            if (!i->getClicked()){
                if (numGuessRemaining < 4){
                    i->clicked = true; 
                    numGuessRemaining += 1;

                    guessWords.push_back(i->word);

                }
            } else { // HAS already been clicked, so unclick it and reduce numGuessREmaining
                i->clicked = false;
                numGuessRemaining -= 1;

                int tempIdx = 0;
                for (int j = 0; j < guessWords.size(); j++){
                    if (guessWords[j] == i->word){
                        tempIdx = j;
                    }
                }
                guessWords.erase(guessWords.begin() + tempIdx);
            }
        }
    }

    /*Check to see if submit has been pressed*/

    //100,50,200,500 -> submit box width, height, positionX, positionY
    //positionX, positionY + 30,width+100,height -> where the rectangle is drawn
    if (isClicked(200, 500+30,100+100, 50)){
        if (numGuessRemaining != 4){
            return;
        }
        for (auto i : guessBoxes){
            if (i->getClicked()){
                i->clicked = false;
            }
        }
        numGuessRemaining = 0; /*No more words have been chosen*/

        /*Now wait to see if the guess was right */

        std::string tempString = guessWords[0];

        int rightVec = -1; 

        if (std::count(correct1.begin(), correct1.end(),tempString) != 0){
            rightVec = 1; 
        } else if (std::count(correct2.begin(), correct2.end(),tempString) != 0){
            rightVec = 2; 
        } else if (std::count(correct3.begin(), correct3.end(),tempString) != 0){
            rightVec = 3; 
        } else if (std::count(correct4.begin(), correct4.end(),tempString) != 0){
            rightVec = 4; 
        } 

        int counter = 0;
        switch(rightVec){
            case 1:
                for (int i = 0; i <  4; i++){
                    for (int j = 0; j < 4; j++){
                        if (guessWords[j] == correct1[i]){
                            counter += 1;
                        }
                    }
                }
                break; 
            case 2:
                for (int i = 0; i <  4; i++){
                    for (int j = 0; j < 4; j++){
                        if (guessWords[j] == correct2[i]){
                            counter += 1;
                        }
                    }
                }
                break; 
            case 3:
                for (int i = 0; i <  4; i++){
                    for (int j = 0; j < 4; j++){
                        if (guessWords[j] == correct3[i]){
                            counter += 1;
                        }
                    }
                }
                break; 
            case 4:
                for (int i = 0; i <  4; i++){
                    for (int j = 0; j < 4; j++){
                        if (guessWords[j] == correct4[i]){
                            counter += 1;
                        }
                    }
                }
                break;
            default:
                break;
        }

        /*Makes those boxes invisible */
        if (counter == 4){
            guessRight = true; 

            for (int i = 0; i < guessWords.size(); i++){
                for (int j = 0; j < guessBoxes.size(); j++){
                    if (guessWords[i] == guessBoxes[j]->word){
                        guessBoxes[j]->visible = false;
                    }
                }
            }

        }
        counter = 0; // Check to see if the player has won yet 
        for (int i = 0; i < guessBoxes.size(); i++){
            if (!guessBoxes[i]->visible){
                counter += 1;
            }
        }
        if (counter == 16){
            // Player has won 
            won  = true;
            gameState = Refreshing;
        }

        /*IF THEY GET THE GUESS WRONG*/

        if (!guessRight){
            submitBar.guessesLeft -= 1; 
        }
        if (submitBar.guessesLeft <= 0){
            gameState = Refreshing;
        }

        guessRight = false; 
        guessWords.clear();
    }
}

void Game::drawRefresh(){
    if (won){
        DrawText("Congratulations! You won!",250,250,25,BLACK);
    } else {
        DrawText("You lost this time",250,250,25,BLACK);
    }

    //reset();

}

void Game::reset(){
    won = false; 
    submitBar.gameStarted = false ;

    int startingX = 50; 

    int startingY = 50;

    for (auto i : guessBoxes){
        i->positionX = startingX; 
        startingX += 150; 

        if (startingX > 500){
            startingX = 50;
            startingY += 100;
        }
        i->positionY = startingY;

        i->visible = true; 
    }

    guessRight = false; 
    boxUpdated = false; 

    correct1.clear();
    correct2.clear();
    correct3.clear();
    correct4.clear(); 

    allWords.clear();

    guessWords.clear();

    gameState = Playing;

}

