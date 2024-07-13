#ifndef WIRE
#define WIRE
#include <vector>
#include "gates.h"
using namespace std;

struct Gate; //forward declaration because Gate/Wire circular dependancy

struct Wire{
    vector<int> history;//size maxTime, with data 3, ungenerated
    vector<Gate*> connectedGates;
    Wire(int maxTime){ history = vector<int>(maxTime, 3); }
    int getLastValue(int time){
        for(int i = time; i >= 0; i--){ if(history.at(i) != 3){ return history.at(i); } }
        return 2;
    }
    void PullForward(int originTime, int updateTime){
        int oldData = getLastValue(originTime-1); 
        for(int i = updateTime-1; i >= 0 && history.at(i) == 3; i--){
            history.at(i) = oldData; //pull forward
        }
    }
};
#endif