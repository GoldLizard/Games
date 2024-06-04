#include <iostream>
#include "cube.cpp"
using namespace std;

int main(){
    Cube cube;
    cube.Print();
    cout << "Press Enter to scramble. " << endl;
    system("Pause");
    cube.Scramble();
    cube.Print();
    srand(time(0));
    string input;
    while(!cube.Solved()){
        cout << "color: ";
        getline(cin, input);
        cout << endl;
        cube.InputTurn(input);
        //system("Clear");
        cube.Print();
    }
    cout << "Solved" << endl;
    system("Pause");
    return 0;
}