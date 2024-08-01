#include "game.h"
using namespace std;

int main(){
    Game currentGame;
    currentGame.Input();
    for(int i = 0; true; i++){
        system("Clear");
        cout << "Tick: " << i << endl;
        currentGame.Print();
        system("Pause");
        currentGame.Tick();
    }
    return 0;
}