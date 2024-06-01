#ifndef PIECES
#define PIECES
using namespace std;

#define WHITE 0
#define BLACK 1

struct Piece {
    bool side;
    string symbol;
    virtual bool CheckIfLegal(int x1, int y1, int x2, int y2) = 0;
};

struct Castle : Piece {
    Castle(bool side){
        this->side = side;
        this->symbol = "C"; //♖
    }
    bool CheckIfLegal(int x1, int y1, int x2, int y2) override;
};

struct Knight : Piece {
    Knight(bool side){
        this->side = side;
        this->symbol = "H"; //♘
    }
    bool CheckIfLegal(int x1, int y1, int x2, int y2) override;
};

struct Bishop : Piece {
    Bishop(bool side){
        this->side = side;
        this->symbol = "B"; //♗
    }
    bool CheckIfLegal(int x1, int y1, int x2, int y2) override;
};

struct King : Piece {
    King(bool side){
        this->side = side;
        this->symbol = "K"; //♔
    }
    bool CheckIfLegal(int x1, int y1, int x2, int y2) override;
};

struct Queen : Piece {
    Queen(bool side){
        this->side = side;
        this->symbol = "Q"; //♕
    }
    bool CheckIfLegal(int x1, int y1, int x2, int y2) override;
};

struct Pawn : Piece {
    Pawn(bool side){ 
        this->side = side;
        this->symbol = "P"; //
    }
    bool CheckIfLegal(int x1, int y1, int x2, int y2) override;
};

#endif