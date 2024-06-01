#include <iostream>
#include <vector>
#include "Pieces.h"
#include "GameBoard.h"
#include "Pieces.cpp"
using namespace std;

int main(){
    GameBoard currentGame;
    string currentMove;

    for(int i = WHITE; true; i++){
        bool side = i%2; //will oscillate between WHITE and BLACK every other loop
        currentGame.Print();
        (side == WHITE) ? cout << "White's turn: " << endl : cout << "Black's turn: " << endl;
        cin>>ws; //for whatever reason, using cin in a function will not clear cin when jumping back to main
        getline(cin, currentMove); //input "B2 to B4"
        Input move = Input(currentMove);
        while(!move.IsValid(side)){
            cout << "Invalid Move. Input a valid move" << endl;
            getline(cin, currentMove);
            move = Input(currentMove);
        }
        currentGame.Play(move);
        system("Clear");
    }
    return 0;
}