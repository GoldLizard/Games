#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

enum pixel {DEAD, LIFE}; //DEAD == 0, LIFE == 1

struct Coordinates{
    int x, y;
    Coordinates operator+(Coordinates &c){ return Coordinates{this->x + c.x, this->y + c.y}; }
    bool operator<(vector<int> v){ return (x >= 0 && x < v.at(0) && y >= 0 && y < v.at(1)); }
};

struct Game{
    int xDim = 40, yDim = 40;
    vector<vector<bool>> board, emptyTemplate;
    bool ValueAt(Coordinates c){ return board.at(c.y).at(c.x); }
    void Set(vector<vector<bool>> &v, Coordinates c, bool b){ v.at(c.y).at(c.x) = b; }
    Game(){
        board = vector<vector<bool>>(yDim, vector<bool>(xDim, DEAD)); //generate empty 2D boards
        emptyTemplate = board;
    }
    void Input(){
        cout << "Conway's Game of Life. Fullscreen to avoid visual glitches" << endl;
        cout << "Center is \"20, 20\". Input coordinates one at a time to place living/dead cells" << endl;
        cout << "type \"RUN\" to run simulation" << endl;
        cout << "type \"PRE\" for predefined examples" << endl;
        cout << "Coordinates: ";
        string input = "";
        getline(cin, input);
        cout << endl;
        while(input != "RUN" && input != "PRE"){  
            stringstream line(input);
            string x, y;
            line >> x >> y;
            board.at(stoi(y)).at(stoi(x)) = !ValueAt(Coordinates{stoi(x), stoi(y)}); //toggle coordinate
            Print();
            cout << "Coordinates: ";
            getline(cin, input);
            cout << endl;
        }
        if(input == "PRE"){
            cout << "Options: \"glider\", \"block\", \"beehive\", \"explosion\", and \"space ship\"" << endl;
            getline(cin, input);
            cout << endl;
            Coordinates center = Coordinates{20, 20};
            vector<Coordinates> offsets;
            if(input == "glider"){ offsets = {{0, 0}, {1, 0}, {0, -1}, {-1, -1}, {-1, 1}}; } //glider offsets
            else if(input == "block"){ offsets = {{0, 0}, {-1, 0}, {0, 1}}; }
            else if(input == "blinker"){ offsets = {{0, 0}, {-1, 0}, {1, 0}}; }
            else if(input == "beehive"){ offsets = {{0, 0}, {1, 0}, {2, 0}, {0, 1}}; }
            else if(input == "explosion"){ offsets = {{-3, 1}, {-2, 1}, {-1, 1}, {-3, 0}, {-1, 0}, {-3, -1}, {-2, -1}, {-1, -1}, {3, 1}, {2, 1}, {1, 1}, {3, 0}, {1, 0}, {3, -1}, {2, -1}, {1, -1}}; }
            else if(input == "space ship"){ offsets = {{0, 0}, {0, -1}, {0, -2}, {1, 0}, {2, -1}, {-3, 5}, {-2, 5}, {-1, 5}, {-3, 4}, {0, 6}, {0, 7}, {-1, 7}, {-6, 4}, {-7, 4}, {-8, 4}}; }
            for(Coordinates eachOffset : offsets){ Set(board, center + eachOffset, LIFE); }
        }
    }
    void Tick(){
        vector<vector<bool>> nextBoard = emptyTemplate;
        vector<Coordinates> offsets = {{1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}};
        for(int i = 0; i < xDim; i++){
            for(int j = 0; j < yDim; j++){
                Coordinates eachPixel = Coordinates{i, j};
                int liveNeighbors = 0;
                for(Coordinates eachOffset : offsets){
                    if(eachPixel + eachOffset < vector{xDim, yDim} && ValueAt(eachPixel + eachOffset)){ liveNeighbors++; } //if side is within bounds and alive, count
                }
                Set(nextBoard, eachPixel, liveNeighbors == 3 || (ValueAt(eachPixel)&&(liveNeighbors == 2))); //game's 4 rules are redundant
            }
        }
        board = nextBoard;
    }
    void Print(){
        for(int i = 0; i < xDim+2; i++){ cout << char(219) << char(219); }      //top row
        cout << endl;
        for(int i = board.size()-1; i >= 0; i--){                               //for each row, top to bottom
            cout << char(219) << char(219);                                         //left column
            for(bool eachBool : board.at(i)){                                       //for each pixel, left to right
                cout << (eachBool ? string{char(219), char(219)} : "  ");               //print value
            } 
            cout << char(219) << char(219) << endl;                                 //right column + new line
        }
        for(int i = 0; i < xDim+2; i++){ cout << char(219) << char(219); }      //bottom row
        cout << endl;
    }
};
    /*Any live cell with fewer than two live neighbours dies, as if by underpopulation.
    Any live cell with two or three live neighbours lives on to the next generation.
    Any live cell with more than three live neighbours dies, as if by overpopulation.
    Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.*/