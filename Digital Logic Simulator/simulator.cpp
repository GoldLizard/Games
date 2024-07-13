#include "circuit.h"
using namespace std;

int main(){
    string circuitFile;
    cout << "Input circuit file name: ";     cin >> circuitFile;         cout << endl;
    Circuit currentCircuit = Circuit(circuitFile + ".txt", circuitFile + "_v.txt");
    for(int time = 0; time < currentCircuit.maxTime; time++){
        currentCircuit.SimulateTick(time); //execute each Event for each time
        //currentCircuit.Print(); //for debugging
        if(currentCircuit.EmptyQueue(time)){
            currentCircuit.maxTime = time; //force quits this loop, also restricts time printed
        }
    }
    currentCircuit.FinishGeneration();
    currentCircuit.Print();
    system("Pause");
    return 0;
}