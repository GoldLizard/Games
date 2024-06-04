#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <chrono>
#include "pieces.cpp"
using namespace std;

struct Cube{
    vector<vector<vector<Piece*>>> grid; //defined by 3D grid, centered at <1, 1, 1>. All pieces offset <+-1, +-1, +-1>
    vector<pair<string, vector<int>>> centers = { {RED, {1, 0, 0}}, {ORANGE, {-1, 0, 0}}, {BLUE, {0, 1, 0}}, {GREEN, {0, -1, 0}}, {WHITE, {0, 0, 1}}, {YELLOW, {0, 0, -1}} };
    Cube(){
        grid = { { {new Corner, new Edge, new Corner}, 
                   {new Edge, new Center, new Edge}, 
                   {new Corner, new Edge, new Corner} }, //layer 1

                 { {new Edge, new Center, new Edge}, 
                   {new Center, new Core, new Center}, 
                   {new Edge, new Center, new Edge} }, //layer 2

                 { {new Corner, new Edge, new Corner},
                   {new Edge, new Center, new Edge},
                   {new Corner, new Edge, new Corner} } }; //layer 3
        for(pair<string, vector<int>> eachCenter : centers){
            string color = eachCenter.first;
            int centerX = eachCenter.second.at(0), centerY = eachCenter.second.at(1), centerZ = eachCenter.second.at(2);
            for(int i = -1; i <= 1; i++){
                for(int j = -1; j <= 1; j++){
                    if(abs(centerX) == 1){ grid.at(1+centerX).at(1+i).at(1+j)->colorX = color; }
                    else if(abs(centerY) == 1){ grid.at(1+i).at(1+centerY).at(1+j)->colorY = color; }
                    else if(abs(centerZ) == 1){ grid.at(1+i).at(1+j).at(1+centerZ)->colorZ = color; }
                }
            }
        }
    }
    void Turn(vector<int> center, bool CW){
        int centerX = center.at(0), centerY = center.at(1), centerZ = center.at(2);
        vector<pair<int, int>> ringOffsets = {{-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}};
        vector<Piece**> perimeter = {};
        for(pair<int, int> eachOffset : ringOffsets){ //turn 2D side of 3D cube into a 1D iterable vector
            int iOffset = eachOffset.first, jOffset = eachOffset.second;
            if(abs(centerX) == 1){ //opposite sides have inverses turning directions, so (centerX == 1)-(centerX == -1) corrects that by adding/subtracting iOffset
                perimeter.push_back(&grid.at(1+centerX).at(1+iOffset*((centerX == 1)-(centerX == -1))).at(1+jOffset));
                (*perimeter.back())->SwapYZ();
            }
            else if(abs(centerY) == 1){
                perimeter.push_back(&grid.at(1+iOffset*((centerY == -1)-(centerY == 1))).at(1+centerY).at(1+jOffset));
                (*perimeter.back())->SwapXZ();
            }
            else if(abs(centerZ) == 1){
                perimeter.push_back(&grid.at(1+iOffset*((centerZ == 1)-(centerZ == -1))).at(1+jOffset).at(1+centerZ));
                (*perimeter.back())->SwapXY();
            }
        }
        vector<Piece*> v; //shift the pieces on the turned side
        int n = (CW) ? 6 : 2; //CW is shifted by 6, CCW is shifted by 2
        for(int i = 0; i < n; i++){ v.push_back(*(perimeter.at(i))); }           //store the first n pieces
        for(int i = 0; i < 8-n; i++){ *perimeter.at(i) = *(perimeter.at(i+n)); } //copy the piece n spaces to the right
        for(int i = 0; i < n; i++){ *perimeter.at((8-n)+i) = v.at(i); }          //append the stored n pieces to the end
    }
    void Scramble(){ //generate 20 random moves
        for(int i = 0; i < 20; i++){ Turn(centers.at(rand()%centers.size()).second, rand()%2); }
    }
    void InputTurn(string input){
        if(input == "scramble"){ Scramble(); return; }
        if(input.find("CW") < input.size() || input.find("CCW") < input.size()){ return; }
        stringstream s(input);
        string color, rotation;
        s >> color >> rotation;
        map<string, vector<int>> sides = { {"RED", {1, 0, 0}}, {"ORANGE", {-1, 0, 0}}, {"BLUE", {0, 1, 0}}, {"GREEN", {0, -1, 0}}, {"WHITE", {0, 0, 1}}, {"YELLOW", {0, 0, -1}} };
        Turn(sides.at(color), (rotation == "CW"));
    }
    bool Solved(){
        for(pair<string, vector<int>> eachCenter : centers){
            string color = eachCenter.first;
            int centerX = eachCenter.second.at(0), centerY = eachCenter.second.at(1), centerZ = eachCenter.second.at(2);
            for(int i = -1; i <= 1; i++){
                for(int j = -1; j <= 1; j++){
                    if( (abs(centerX) == 1 && grid.at(1+centerX).at(1+i).at(1+j)->colorX != color)
                     || (abs(centerY) == 1 && grid.at(1+i).at(1+centerY).at(1+j)->colorY != color)
                     || (abs(centerZ) == 1 && grid.at(1+i).at(1+j).at(1+centerZ)->colorZ != color) ){
                        return false;
                    }
                }
            }
        }
        return true;
    }
    void Print(){
        vector<vector<string*>> frame;
        vector<string*> v;
        for(int i = 0; i < 15; i++){ v.push_back(nullptr); }
        for(int j = 0; j < 11; j++){ frame.push_back(v); }
        for(int i = -1; i <= 1; i++){
            for(int j = -1; j <= 1; j++){
                frame.at(1+i).at(5+j) = new string{grid.at(1-j).at(1+i).at(2)->colorZ}; //White
                frame.at(5+i).at(1+j) = new string{grid.at(2).at(1+j).at(1-i)->colorX}; //Red
                frame.at(5+i).at(5+j) = new string{grid.at(1-j).at(2).at(1-i)->colorY}; //Blue, default
                frame.at(5+i).at(9+j) = new string{grid.at(0).at(1-j).at(1-i)->colorX}; //Orange
                frame.at(5+i).at(13+j) = new string{grid.at(1+j).at(0).at(1-i)->colorY}; //Green
                frame.at(9+i).at(5+j) = new string{grid.at(1-j).at(1-i).at(0)->colorZ}; //Yellow
            } 
        }
        for(vector<string*> eachVector : frame){
            for(string* eachString : eachVector){
                if(eachString == nullptr){
                    printf("\033[1;30m");
                    cout << char(219) << char(219);
                }
                else{ //eachString != nullptr
                    if(*eachString == ""){ printf("\033[1;30m"); }
                    else{ printf(eachString->c_str()); }
                    cout << char(219) << char(219);
                }
            }
            cout << endl;
        }
        cout << endl;
    }
};