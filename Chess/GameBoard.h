#include <iostream>
#include <vector>
#include <map>
#include "Pieces.h"
using namespace std;

//I dont like this being a global variable, but otherwise there would be a circular dependancy between Pieces and GameBoard
vector<vector<Piece*>> grid = 
    {{new Castle(WHITE), new Knight(WHITE), new Bishop(WHITE), new Queen(WHITE), new King(WHITE), new Bishop(WHITE), new Knight(WHITE), new Castle(WHITE)},
    {new Pawn(WHITE), new Pawn(WHITE), new Pawn(WHITE), new Pawn(WHITE), new Pawn(WHITE), new Pawn(WHITE), new Pawn(WHITE), new Pawn(WHITE)},
    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
    {new Pawn(BLACK), new Pawn(BLACK), new Pawn(BLACK), new Pawn(BLACK), new Pawn(BLACK), new Pawn(BLACK), new Pawn(BLACK), new Pawn(BLACK)},
    {new Castle(BLACK), new Knight(BLACK), new Bishop(BLACK), new King(BLACK), new Queen(BLACK), new Bishop(BLACK), new Knight(BLACK), new Castle(BLACK)}};

struct Input {
    int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    bool PromotionCondition(){
        bool side = grid.at(y2).at(x2)->side; //set after move is played
        if(grid.at(y2).at(x2)->symbol == "P" && ((side == WHITE && y2 == 7) || (side == BLACK && y2 == 0))){ //if pawn and reached other side
            return 1;
        }
        return 0;
    }
    Input(string move){ //move == "B2 to B4", return {1, 1, 1, 3} = {x1, y1, x2, y2}
        if(move.size() != 8){return;} //return empty data if invalid format
        cout << move.at(0) - 'A' << " " << (move.at(1) - '0') - 1 << " " << move.at(6) - 'A' << " " << (move.at(7) - '0') - 1 << endl;
        this->x1 = move.at(0) - 'A'; this->y1 = (move.at(1) - '0') - 1;
        this->x2 = move.at(6) - 'A'; this->y2 = (move.at(7) - '0') - 1;
    }
    IsValid(bool side){
        if((x1 + x2 + y1 + y2 == 0)                              //if empty NULL case from invalid input
        || (x1 == 8 || x2 == 8 || y1 == 8 || y2 == 8)            //or outside the 8x8 grid
        || (grid.at(y1).at(x1) == nullptr)                       //or trying to move an empty position
        || (grid.at(y1).at(x1)->side != side)                    //or trying to move opponent's piece
        || (x1 == x2 && y1 == y2)                                //or same start and end points
        || (!grid.at(y1).at(x1)->CheckIfLegal(x1, y1, x2, y2))){ //finally check move legality
            return 0;
        }       
        return 1;
    }
};

struct GameBoard {
    Piece* at(int x, int y){ return grid.at(y).at(x); }
    void Print(){
        int k = 0; //used for alternating empty squares
        for(int i = 7; i >= 0; i--){
            printf("\033[1;0m"); //reset to white
            cout << i+1 << " ";
            for(int j = 0; j <= 7; j++){
                Piece* currentPiece = grid.at(i).at(j);
                if(currentPiece == nullptr){
                    (k%2 == WHITE) ? printf("\033[1;0m") : printf("\033[1;30m");
                    cout << char(219);
                }
                else if(currentPiece->side == WHITE){ printf("\033[1;0m"); cout << currentPiece->symbol;} //text color white
                else if(currentPiece->side == BLACK){ printf("\033[1;30m"); cout << currentPiece->symbol;} //text color black
                k++;
            }
            k++;
            cout << endl;
        }
        cout << endl << "  ABCDEFGH" << endl << endl;
    }

    void Promote(Input move){
        cout << "Promote Pawn to: ";
        map<string, Piece*> options = {{"Queen", new Queen(grid.at(move.y2).at(move.x2)->side)}, {"Bishop", new Bishop(grid.at(move.y2).at(move.x2)->side)}, {"Rook", new Castle(grid.at(move.y2).at(move.x2)->side)}, {"Knight", new Knight(grid.at(move.y2).at(move.x2)->side)}};
        string promotion;
        cin >> promotion;
        while(!options.count(promotion)){
            cout << "Invalid promotion. Must be Queen, Bishop, Rook, or Knight" << endl;
            cin >> promotion;
        }
        grid.at(move.y2).at(move.x2) = options.at(promotion); //PLEASE COMPILE
    }

    void Play(Input move){
        Piece** start = &grid.at(move.y1).at(move.x1);
        Piece** end = &grid.at(move.y2).at(move.x2);
        *end = *start; //move the piece
        *start = nullptr; //delete old location
        if((*end)->symbol == "P"){  //pawn cases
            if(((*end)->side == WHITE && move.y2 == 7) || ((*end)->side == BLACK && move.y2 == 0)){ //promote case
                Promote(move);
            }
        }
        if((abs(move.x2-move.x1) == 1) && (move.y2-move.y1 == (((*end)->side == WHITE) - ((*end)->side == BLACK))) //if pawn is moving diagonal
        && (grid.at(move.y1).at(move.x2)->symbol == "P") && (grid.at(move.y1).at(move.x2)->side != (*end)->side)){ //and side piece is opponent's pawn
            grid.at(move.y1).at(move.x2) = nullptr; //delete passed piece
        }
    }
};