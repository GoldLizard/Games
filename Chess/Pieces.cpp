#include <iostream>
#include <vector>
#include "Pieces.h"
using namespace std;

bool Castle::CheckIfLegal(int x1, int y1, int x2, int y2) { //add castling eventually
    if((x2-x1)*(y2-y1) != 0){ //one axis must be 0, therefore product of movement must be 0 
        return 0; //restricts further logic to 1D movement
    }
    else{
        int unitX, unitY;
        (y2-y1 == 0) ? unitX = (x2-x1)/abs(x2-x1) : unitX = 0;
        (x2-x1 == 0) ? unitY = (y2-y1)/abs(y2-y1) : unitY = 0;
        int x = x1 + unitX, y = y1 + unitY;
        while( !(x == x2 && y == y2) ){
            if(grid.at(y).at(x) != nullptr){         //invalid move if a piece is blocking path
                return 0;
            }
            x += unitX; y += unitY;
        }
    }
    return 1; //valid move if 1D movement and no obstructing pieces
}

bool Knight::CheckIfLegal(int x1, int y1, int x2, int y2)  {
    if(abs((x2-x1)*(y2-y1)) == 2){ //only allows <+-1, +-2> , <+-2, +-1>
        return 1;
    }
    return 0;
}

bool Bishop::CheckIfLegal(int x1, int y1, int x2, int y2) {
    int unitX = (x2-x1)/abs(x2-x1), unitY = (y2-y1)/abs(y2-y1);
    if(abs(x2-x1) != abs(y2-y1)){ //if not diagonal, invalid move
        cout << "not a diagonal move" << endl;
        return 0;
    }
    int i = x1 + unitX, j = y1 + unitY;
    while(i != x2 && j != y2){ //for loop with 2 arguments at once. Cannot nest loops, else it searches a large square area
        if(grid.at(j).at(i) != nullptr){
            cout << "diagonal path blocked by piece" << endl;
            return 0;
        }
        i += unitX; j += unitY;
    }
    return 1;
}

bool King::CheckIfLegal(int x1, int y1, int x2, int y2) {
    if(abs(x2-x1) <= 1 && abs(y2-y1) <= 1){ //if king moved 1 in any direction
        return 1;
    }
    return 0;
}

bool Queen::CheckIfLegal(int x1, int y1, int x2, int y2) {
    if(abs(x2-x1) == abs(y2-y1)){ //if diagonal move, reuse Bishop logic
        Bishop tempBishop = Bishop(this->side);
        return tempBishop.CheckIfLegal(x1, y1, x2, y2);
    }
    else if((x2-x1)*(y2-y1) == 0){ //if 1D, reuse Castle logic
        Castle tempCastle = Castle(this->side);
        return tempCastle.CheckIfLegal(x1, y1, x2, y2);
    }
    return 0;
}

bool Pawn::CheckIfLegal(int x1, int y1, int x2, int y2) {                                  //google en passant
    if((abs(x2-x1) == 1) && (y2-y1 == ((this->side == WHITE) - (this->side == BLACK)))     //if pawn is moving diagonal
    && (grid.at(y1).at(x2)->symbol == "P") && (grid.at(y1).at(x2)->side != this->side)){   //and side piece is opponent's pawn
        return 1;
    }
    if(x2-x1 != 0){
        return 0;   //restricts next logic to 1D movement
    }
    else if((this->side == WHITE) && ((y2-y1 == 1) || (y1 == 1 && y2-y1 == 2)) //if white moving up 1 or !hasMoved and moving up 2
    || (this->side == BLACK) && ((y2-y1 == -1) || (y1 == 6 && y2-y1 == -2))){ //if black moving down 1 or !hasMoved and moving down 2
        return 1;
    }
    return 0;
}