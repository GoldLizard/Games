#include <iostream>
#include "game.h"
using namespace std;

int main(){
    Game currentGame;
    while(!currentGame.Lose()){
        currentGame.Print();
        currentGame.Input();
        currentGame.GameTick();
        if(currentGame.CheckApple()){
            currentGame.EatApple();
            currentGame.RandomApple();
        }
    }
    cout << "GAME OVER" << endl;
    system("Pause");
    return 0;
}