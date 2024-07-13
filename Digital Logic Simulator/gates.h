#ifndef GATE
#define GATE
#include <vector>
#include "wires.h"
using namespace std;

struct Gate{
    int delay;
    Wire* input1, *input2, *outputWire;
    vector<vector<int>> allSolutions;
    void Evaluate(int originTime, int updateTime){
        int A = input1->getLastValue(originTime), B = (input2 != nullptr) ? input2->getLastValue(originTime) : 2;
        //cout << "Event:" << '\t' << "A: " << A << " B: " << B << " output: " << allSolutions.at(A).at(B) << endl; 
        //cout << "originTime: " << originTime << "    updateTime: " << updateTime << endl;
        outputWire->history.at(updateTime) = allSolutions.at(A).at(B); //place
        outputWire->PullForward(originTime, updateTime);
    };
};

struct AND  : Gate{ AND()  { allSolutions = {{0, 0, 0}, {0, 1, 2}, {0, 2, 2}}; } };
struct NAND : Gate{ NAND() { allSolutions = {{1, 1, 1}, {1, 0, 2}, {1, 2, 2}}; } };
struct OR   : Gate{ OR()   { allSolutions = {{0, 1, 2}, {1, 1, 1}, {2, 1, 2}}; } };
struct NOR  : Gate{ NOR()  { allSolutions = {{1, 0, 2}, {0, 0, 0}, {2, 0, 2}}; } };
struct XOR  : Gate{ XOR()  { allSolutions = {{0, 1, 2}, {1, 0, 2}, {2, 2, 2}}; } };
struct XNOR : Gate{ XNOR() { allSolutions = {{1, 0, 2}, {0, 1, 2}, {2, 2, 2}}; } };
struct NOT  : Gate{ NOT()  { allSolutions = {{2, 2, 1}, {2, 2, 0}, {1, 0, 2}}; } };
#endif