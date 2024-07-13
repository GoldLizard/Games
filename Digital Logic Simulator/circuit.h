#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include "gates.h"
using namespace std;

struct Event{
    int originTime, updateTime;
    Gate* gate;
};

struct Circuit{
    map<int, vector<Event>> queue;          //.at(time) pulls events scheduled for that tick
    map<char, Wire*> allWires;              //.at('A') pulls the corresponding wire*
    vector<Gate*> allGates;                 //same idea. Technically not needed, since its a circular dependancy and I already know allWires
    int maxTime = 61;
    void PlaceEvent(Event event){ //throws a fit if I dare pass {event} as a parameter. Also (void) to make the compiler shut up about different types in conditional operators
        (!queue.count(event.updateTime)) ? (void)queue.emplace(event.updateTime, vector<Event>(1, event)) : queue.at(event.updateTime).push_back(event);
    }
    Circuit(string circuitFile, string circuitConditions) {
        string line;                            //gets each line from TXT
        ifstream circuit, conditions;
        map<int, char> labels;                  //.at(4) pulls 'D'. Emplaced in wire condition, used in gate condition
        circuit.open(circuitFile);
        getline(circuit, line);                 //just trash first line
        while(!circuit.eof() && getline(circuit, line) && line != ""){
            try{
                stringstream TXTline(line);
                if(line.find("INPUT") < line.size() || line.find("OUTPUT") < line.size()){ //wire condition
                    string type; char symbol; int number;
                    TXTline >> type >> symbol >> number;
                    labels.emplace(number, symbol);
                    allWires.emplace(symbol, new Wire(maxTime));
                }
                else{                               //assuming gate condition
                    string type, delay; int input1, input2, output;
                    TXTline >> type >> delay >> input1 >> input2 >> output; //for not case, input2 becomes output.
                    Gate* newGate;
                    if(type == "AND"){ newGate = new AND; }
                    else if(type == "NAND"){ newGate = new NAND; }
                    else if(type == "OR"  ){ newGate = new OR;   }
                    else if(type == "NOR" ){ newGate = new NOR;  }
                    else if(type == "XOR" ){ newGate = new XOR;  }
                    else if(type == "XNOR"){ newGate = new XNOR; }
                    else if(type == "NOT" ){ newGate = new NOT;  }          //I am not rewriting a constructor for each subclass
                    newGate->delay = stoi(delay);                           //automatically filters 2ns to 2
                    if(!labels.count(input1)){ labels.emplace(input1, '0'+input1); allWires.emplace('0'+input1, new Wire(maxTime)); } //create input1 if it doesnt exist
                    newGate->input1 = allWires.at(labels.at(input1));                                                        //set input1
                    newGate->input1->connectedGates.push_back(newGate);                                                      //newGate dependant on input1
                    if(!labels.count(input2)){ labels.emplace(input2, '0'+input2); allWires.emplace('0'+input2, new Wire(maxTime)); } //create input2 if neither input/output
                    if(type == "NOT"){
                        newGate->input2 = nullptr;                              //null input2 for NOT, utilized in Wire::Evaluate()
                        newGate->outputWire = allWires.at(labels.at(input2));   //output wire dependant on newGate
                    }
                    else{
                        newGate->input2 = allWires.at(labels.at(input2));                                                       //set input2
                        newGate->input2->connectedGates.push_back(newGate);                                                     //newGate dependant on input2
                        if(!labels.count(output)){ labels.emplace(output, '0'+output); allWires.emplace('0'+output, new Wire(maxTime)); }//create wire if neither input/output
                        newGate->outputWire = allWires.at(labels.at(output));                                                   //set outputWire
                    }
                    allGates.push_back(newGate); //not really used for anything
                }
            }
            catch(const runtime_error& e){
                printf("\033[1;36m");
                cout << "Invalid line in " << circuitConditions << ", cannot parse. Line: " << line << endl;
            }
        }
        conditions.open(circuitConditions);
        getline(conditions, line);              //trash first line
        while(!conditions.eof() && getline(conditions, line) && line != ""){
            try{
                stringstream TXTline(line);
                string type; char symbol; int time; char state;
                TXTline >> type >> symbol >> time >> state;
                allWires.at(symbol)->history.at(time) = (state == 'X') ? 2 : state-'0';    //write new state at the specified time
                allWires.at(symbol)->PullForward(time, time);              //fill data between current time and last known value
                for(Gate* eachGate : allWires.at(symbol)->connectedGates){    //create new Events
                    PlaceEvent( Event{time, time + eachGate->delay, eachGate} );
                }
            }
            catch(const runtime_error& e){
                printf("\033[1;36m");
                cout << "Invalid line in " << circuitFile << ", cannot parse. Line: " << line << endl;
            }
        }
    }
    void SimulateTick(int time){
        if(!queue.count(time)){ return; }           //if no events are present at time, do nothing
        for(Event eachEvent : queue.at(time)){      //if events are present at time, evaluate every event at time
            eachEvent.gate->Evaluate(eachEvent.originTime, eachEvent.updateTime);
            for(Gate* eachGate : eachEvent.gate->outputWire->connectedGates){ //create new Events based on change
                PlaceEvent( Event{time, time + eachEvent.gate->delay, eachGate} );
            }
        }
    }
    bool EmptyQueue(int time){
        for(int i = time; i <= maxTime; i++){
            if(queue.count(i) && queue.at(i).size() > 0){ return 0; }
        }
        return 1;
    }
    void FinishGeneration(){
        for(pair<char, Wire*> eachPair : allWires){
            Wire* eachWire = eachPair.second;
            eachWire->PullForward(maxTime, maxTime);
        }
    }
    void Print(){
        for(pair<char, Wire*> eachPair : allWires){
            Wire* eachWire = eachPair.second;
            cout << " |" << endl << eachPair.first << '|';
            vector<char> symbols = {'_', '-', 'X', 'Z'};
            for(int i = 0; i < maxTime; i++){
                cout << symbols.at(eachWire->history.at(i));
            }
            cout << endl;
        }
        cout << " |";                               
        for(int i = 0; i < maxTime; i++){ cout << '_'; }                                   //|___________
        cout << endl << "  ";
        for(int i = 0; i < maxTime; i++){ cout << ( (i%5 == 0) ? to_string(i/10) : "-" ); } //0 0 1 1 2 2
        cout << endl << "  ";
        for(int i = 0; i < maxTime; i++){ cout << ( (i%5 == 0) ? to_string(i%10) : "-" ); } //0 5 0 5 0 5
        cout << endl;
        cout << "Elapsed Time: " << maxTime-1 << endl << endl;
    }
};