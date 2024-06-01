#include <iostream>
#include <vector>
#include <utility>
using namespace std;

#define DROPHEIGHT 20
#define DROPPOSITION 4

struct Shape {                 //O = center
    string color;              //X = position, offset from center
    pair<int, int> center = {DROPPOSITION, DROPHEIGHT};
    vector<pair<int, int>> offsetCells;
    void Rotate(bool CW){
        if(CW){
            for(pair<int, int> &eachOffset : offsetCells){ //to rotate clockwise, (x, y) = (y, -x)
                int temp = eachOffset.first;
                eachOffset.first = eachOffset.second;
                eachOffset.second = -temp;
            }
        }
        else if(!CW){ //CCW
            for(pair<int, int> &eachOffset : offsetCells){ //to rotate clockwise, (x, y) = (y, -x)
                int temp = eachOffset.second;
                eachOffset.second = eachOffset.first;
                eachOffset.first = -temp;
            }
        }
    }
    virtual void ResetOffsets() = 0;
};

struct Ghost : Shape {           // XX
    Ghost(string color, pair<int, int> center, vector<pair<int, int>> offsetCells){                     // OX
        this->color = color; //yellow
        this->center = center;
        this->offsetCells = offsetCells;
    }
    void ResetOffsets() override {}
};

struct Square : Shape {           // XX
    Square(){                     // OX
        this->color = "\033[38;5;226m"; //yellow
        offsetCells = { {0, 0}, {1, 0}, {0, 1}, {1, 1} };
    }
    void ResetOffsets() override { offsetCells = { {0, 0}, {1, 0}, {0, 1}, {1, 1} }; }
};

struct T : Shape {                //  X
    T(){                          // XOX
        this->color = "\033[35m"; //purple
        offsetCells = { {0, 0}, {-1, 0}, {1, 0}, {0, 1} };
    }
    void ResetOffsets() override { offsetCells = { {0, 0}, {-1, 0}, {1, 0}, {0, 1} }; }
};
                                   // X
struct L : Shape {                 // O
    L(){                           // XX
        this->color = "\033[38;5;130m"; //orange
        offsetCells = { {0, 0}, {0, 1}, {0, -1}, {1, -1} };
    }
    void ResetOffsets() override { offsetCells = { {0, 0}, {0, 1}, {0, -1}, {1, -1} }; }
};
                                    //  X
struct LR : Shape {                 //  O
    LR(){                           // XX
        this->color = "\033[34m"; //blue
        offsetCells = { {0, 0}, {0, 1}, {0, -1}, {-1, -1} };
    }
    void ResetOffsets() override { offsetCells = { {0, 0}, {0, 1}, {0, -1}, {-1, -1} }; }
};

struct I : Shape {
    I(){                          // literally just a line
        this->color = "\033[36m"; //cyan
        offsetCells = { {0, 0}, {0, 1}, {0, 2}, {0, -1}};
    }
    void ResetOffsets() override { offsetCells = { {0, 0}, {0, 1}, {0, 2}, {0, -1} }; }
};

struct Z : Shape {                // XX
    Z(){                          //  OX
        this->color = "\033[38;5;196m"; //red
        offsetCells = { {0, 0}, {0, 1}, {1, 0}, {-1, 1}};
    }
    void ResetOffsets() override { offsetCells = { {0, 0}, {0, 1}, {1, 0}, {-1, 1} }; }
};

struct S : Shape {                //  XX
    S(){                          // XO
        this->color = "\033[32m"; //green
        offsetCells = { {0, 0}, {-1, 0}, {0, 1}, {1, 1}};
    }
    void ResetOffsets() override { offsetCells = { {0, 0}, {-1, 0}, {0, 1}, {1, 1} }; }
};