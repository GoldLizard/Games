#include <iostream>
#include <ctime>
#include <chrono>
#include "game.cpp"
using namespace std;

int main(){
    //initial setup
    srand(time(0));
    Game game; 

    //game loop
    while(!game.Lose()){
        system("Clear");
        game.GenerateGhostBlock();
        game.Print();
        game.CheckAndExecuteInput(); //for a set amount of time, constantly read input and modify data
        if(game.AboveGround()){
            game.LowerBlock();
        } 
        else{                        //an english major could read this pseudocode
            game.PlaceBlock();
            game.ClearLines();
            game.GetNextBlock();
            game.UpdateInfoTable();
        }
    }
    
    //final clear and print upon loss
    system("Clear");
    game.Print();
    system("Pause");
    return 0;
}

/*
TODO:
    1. when storing a rotated block, rotation is not reset
    5. maybe refactor Cell code since its just a string
    7. phantom block to show final location below
*/