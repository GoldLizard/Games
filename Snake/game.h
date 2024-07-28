#include <iostream>
#include <conio.h>   //key inputs
#include <chrono>    //time
#include <vector>
using namespace std;

enum direction {UP, DOWN, LEFT, RIGHT};
enum pixel {EMPTY, SNAKE, APPLE};

struct Coordinates{
    int x, y;
    bool operator==(Coordinates &a){ return (this->x == a.x && this->y == a.y); }     
};

struct Game{
    Coordinates head, butt, apple;
    vector<Coordinates> tail;
    int loopTime = 500; //ms
    int xDim = 15, yDim = 15;
    vector<vector<int>> board;
    int direction = RIGHT;

    bool InTail(Coordinates coords){
        for(Coordinates eachCoord : tail){ if(eachCoord == coords){return 1;} }
        return 0;
    }

    bool Lose(){
        if(head.x < 0 || head.y < 0 || head.x >= xDim || head.y >= yDim){ return 1; } //Lose if head is out of bounds
        return InTail(head); //Lose if head hits self
        return 0;
    }

    void Input(){
        char input = ' ';
        auto start = chrono::high_resolution_clock::now();
        auto duration = chrono::milliseconds(loopTime);
        bool keyPressed = false;
        while(chrono::high_resolution_clock::now() - start < duration){
            if(kbhit()){
                input = _getch();
                if(     input == 'w' && direction != DOWN ){direction = UP;   }
                else if(input == 's' && direction != UP   ){direction = DOWN; }
                else if(input == 'a' && direction != RIGHT){direction = LEFT; }
                else if(input == 'd' && direction != LEFT ){direction = RIGHT;}
                while(kbhit()){getch();} //clear input buffer
                while(chrono::high_resolution_clock::now() - start < duration){} //just stall current loop to prevent double inputs
            }
        }
    }

    void GameTick(){
        Coordinates newHead = head;
        if(     direction == UP   ){newHead.y--;} //UP/DOWN inversed because of print for(eachRow : frame)
        else if(direction == DOWN ){newHead.y++;}
        else if(direction == LEFT ){newHead.x--;}
        else if(direction == RIGHT){newHead.x++;}
        butt = tail.back();                     //new invisible butt for when apple is eaten
        tail.insert(tail.begin(), head);        //move head to front of tail
        tail.pop_back();                        //remove end of tail to "move"
        head = newHead;                         //place new head
    }

    bool CheckApple(){ return (head == apple); }
    void EatApple(){ tail.push_back(butt); }
    void RandomApple(){
        apple = {rand()%xDim, rand()%yDim};
        while(InTail(apple) || (apple == head)){
            apple = {rand()%xDim, rand()%yDim};
        }
    }

    Game() : head(Coordinates{3, 3}), tail({Coordinates{2, 3}, Coordinates{1, 3}}), butt(Coordinates{0, 3}){
        vector<int> v;
        for(int i = 0; i < xDim; i++){ v.push_back(EMPTY); }
        for(int j = 0; j < yDim; j++){ board.push_back(v); } //generates board of specified dimensions
        RandomApple();
    }

    void Print(){
        system("Clear");
        vector<vector<int>> frame = board;                                                  //generate frame
        frame.at(head.y).at(head.x) = SNAKE;                                                //draw head
        for(Coordinates eachCoord : tail){ frame.at(eachCoord.y).at(eachCoord.x) = SNAKE; } //draw tail
        frame.at(apple.y).at(apple.x) = APPLE;                                              //draw apple
        for(int i = 0; i < xDim+2; i++){ cout << char(219) << char(219); }  //print top row, ██
        cout << endl;
        for(vector<int> eachRow : frame){                           //for eachRow in frame
            cout << char(219) << char(219);                             //print left column, ██
            for(int eachInt : eachRow){                                 //for each pixel in eachRow
                if(eachInt == EMPTY){ cout << "  "; }                       //print empty pixel
                else{
                    if(eachInt == SNAKE){ printf("\033[32m"); }             //print green pixel
                    if(eachInt == APPLE){ printf("\033[38;5;196m"); }       //print red pixel
                    cout << char(219) << char(219);
                }
            }
            printf("\033[1;0m");                                        //reset default color for next row
            cout << char(219) << char(219) << endl;                     //print right column, ██
        }
        for(int i = 0; i < xDim+2; i++){ cout << char(219) << char(219); } //print bottom row, ██
        cout << endl << endl;
    }
};