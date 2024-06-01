#include <iostream>
#include <vector>
using namespace std;

struct Game{                   
    vector<vector<int>> grid = {{0, 0, 0, 0},
                                {0, 0, 0, 0},
                                {0, 0, 0, 0},
                                {0, 0, 0, 0}};
    vector<int> ShiftLeft(vector<int> v){
        vector<int> u, w;
        for(int eachInt : v){ (eachInt != 0) ? u.push_back(eachInt) : w.push_back(0); } //fill with integers first
        for(int eachInt : w){ u.push_back(eachInt); };                                  //fill rest in with zeros second
        return u;
    }
    vector<int> Compress(vector<int> v){ //default shift left. Avoids rewriting code
        for(int i = 0; i < v.size()-1; i++){
            if(v.at(i) == v.at(i+1)){
                v.at(i) *= 2;
                v.at(i+1) = 0;
                i++;
            }
        }
        return v;
    }
    void Play(char move){
        for(int i = 0; i <= 3; i++){
            vector<int> u;
            for(int j = 0; j <= 3; j++){ //get row/column
                if(move == 'L'){ u.push_back(grid.at(i).at(j)); }   //default xy
                if(move == 'R'){ u.push_back(grid.at(i).at(3-j)); } //default xy, reverse x
                if(move == 'U'){ u.push_back(grid.at(j).at(i)); }   //inversed xy
                if(move == 'D'){ u.push_back(grid.at(3-j).at(i)); } //inversed xy, reverse y
            }
            u = ShiftLeft(Compress(ShiftLeft(u))); //{0 2 2 4} -> {2 2 4 0} -> {4 0 4 0} -> {4 4 0 0}
            for(int j = 0; j <= 3; j++){ //store row/colum
                if(move == 'L'){ grid.at(i).at(j) = u.at(j); }   //default xy
                if(move == 'R'){ grid.at(i).at(3-j) = u.at(j); } //default xy, reverse x
                if(move == 'U'){ grid.at(j).at(i) = u.at(j); }   //inversed xy
                if(move == 'D'){ grid.at(3-j).at(i) = u.at(j); } //inversed xy, reverse y
            }
        }
    }
    bool ValidMove(char move){ //if move resulted in no change, invalid move
        Game testGame{this->grid};
        testGame.Play(move);
        return (testGame.grid == this->grid) ? false : true; //technically also validates only U/D/L/R inputs, since nothing happens under all other cases
    }
    void CheckLoss(){
        for(vector<int> eachVector : grid){ for(int eachInt : eachVector){ if(eachInt == 0){return;} }}
        cout << "No empty cells remaining. Game over."; system("Pause");
    }
    void RandomPlacement(){
        int x = rand() % 4, y = rand() % 4; //random x and y location, and keep rerolling until empty cell
        while( !((grid.at(y).at(x) == 0) && (grid.at(y).at(x) = (rand() % 10 > 0) ? 2 : 4)) && ((x = rand() % 4) && (y = rand() % 4)) ){} //abusing that second && arguments aren't evaluated until the first is true

    }
    void Print(){
        for(vector<int> eachVector : grid){
            for(int eachCell : eachVector){
                cout << eachCell << '\t';
            }
            cout << endl << endl << endl;
        }
    }
};

int main(){
    char move;
    Game game;
    game.RandomPlacement();
    while(true){
        system("Clear");
        game.Print();
        cout << "Move U/D/L/R: ";
        cin >> move;
        while(!game.ValidMove(move)){ cout << endl << "Invalid Move. Input valid move: "; cin >> move; }
        game.Play(move);
        game.CheckLoss();
        game.RandomPlacement();
    }
    return 0;
}