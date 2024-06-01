#include <iostream>
#include <vector>
#include <conio.h>   //key inputs
#include <chrono>
#include "shapes.cpp"
using namespace std;

#define LEFT 0
#define RIGHT 1
#define CCW 0
#define CW 1
#define WIDTH 10
#define HEIGHT 23
#define INFOWIDTH 15
#define INFOHEIGHT 6

struct Cell{
    string color;
};

struct Game{
    vector<vector<Cell*>> grid, infoTable; //for storage and nextBlocks, printed above
    int score, level, totalLines, ghostOffset, loopTime; //default game speed, milliseconds
    Shape* currentBlock, *storedBlock, *ghostBlock;


    vector<Shape*> nextBlocks; 

    void UpdateInfoTable(){
        int yLevel = 2; //print axis
        int xStart = 1;
        vector<vector<Cell*>> infoFrame;
        vector<Cell*> v;
        for(int i = 0; i < INFOWIDTH; i++){ v.push_back(nullptr); }
        for(int j = 0; j < INFOHEIGHT; j++){ infoFrame.push_back(v); }
        for(Shape* eachShape : nextBlocks){ //for each shape
            for(pair<int, int> eachOffset : eachShape->offsetCells){ //for each offset
                infoFrame.at(yLevel + eachOffset.second).at(xStart + eachOffset.first) = new Cell{eachShape->color};
            }
            xStart += 3;
        }
        if(storedBlock != nullptr){
            for(pair<int, int> eachOffset : storedBlock->offsetCells){
                infoFrame.at(yLevel + eachOffset.second).at(WIDTH + 2 + eachOffset.first) = new Cell{storedBlock->color};
            }
        }
        for(int i = 0; i < INFOHEIGHT; i++){
            infoFrame.at(i).at(WIDTH) = new Cell{"\033[1;30m"};
        }
        infoTable = infoFrame;
    }



    Shape* RandomBlock(){
        vector<Shape*> table = {new Square, new T, new L, new LR, new S, new Z, new I}; //probably memory leak here
        return table.at(rand() % table.size());
    }
    void GetNextBlock(){
        currentBlock = nextBlocks.at(0);
        nextBlocks.at(0) = nextBlocks.at(1);
        nextBlocks.at(1) = nextBlocks.at(2);
        nextBlocks.at(2) = RandomBlock();
    }
    void SwapBlock(){
        if(storedBlock == nullptr){
            storedBlock = currentBlock;
            GetNextBlock();
        }
        else{
            Shape* temp = currentBlock;
            currentBlock = storedBlock;
            storedBlock = temp;

        }
        currentBlock->center = {DROPPOSITION, DROPHEIGHT};
        storedBlock->ResetOffsets();
        UpdateInfoTable();
    }
    

    Game(){
        vector<Cell*> nullRow;
        for(int i = 0; i < WIDTH; i++){ nullRow.push_back(nullptr); } //fill nullRow {nullptr, nullptr, ...,}
        for(int i = 0; i < HEIGHT; i++){ grid.push_back(nullRow); } //empty 2D grid of nullptr
        nextBlocks = {RandomBlock(), RandomBlock(), RandomBlock()};
        GetNextBlock();
        storedBlock = nullptr;
        UpdateInfoTable();
        score = 0;
        loopTime = 500;
        level = 0;
        totalLines = 0;
    }

    bool Lose(){
        int xCenter = currentBlock->center.first, yCenter = currentBlock->center.second;
        for(pair<int, int> eachOffset : currentBlock->offsetCells){
            int x = eachOffset.first, y = eachOffset.second;
            if(grid.at(yCenter + y).at(xCenter + x) != nullptr){ return true; }
        }
        return false;
    }
    bool AboveGround(){
        int xCenter = currentBlock->center.first, yCenter = currentBlock->center.second;
        for(pair<int, int> eachOffset : currentBlock->offsetCells){
            int x = eachOffset.first, y = eachOffset.second;
            if(yCenter + y == 0){ return false; }
            if(grid.at(yCenter + y-1).at(xCenter + x) != nullptr){ return false; }
        }
        return true;
    }
    void LowerBlock(){
        currentBlock->center.second--;
    }

    bool CanMove(bool direction){ //0 == left, 1 == right
        int shift = (direction == RIGHT)-(direction == LEFT);
        for(pair<int, int> eachOffset : currentBlock->offsetCells){
            int x = currentBlock->center.first + eachOffset.first, y = currentBlock->center.second + eachOffset.second;
            if( !(x+shift >= 0 && x+shift < WIDTH) ){ return false; }
            if( (grid.at(y).at(x+shift) != nullptr) ){ return false; }
        }
        return true;
    }
    void Move(bool direction){
        int shift = (direction == RIGHT)-(direction == LEFT);
        currentBlock->center.first += shift;
    }
    void InstantDrop(){
        while(AboveGround()){
            LowerBlock();
        }
    }


    bool CanRotate(bool rotation){
        int xCenter = currentBlock->center.first, yCenter = currentBlock->center.second;
        for(pair<int, int> eachOffset : currentBlock->offsetCells){
            int x = eachOffset.first, y = eachOffset.second;
            if(rotation && grid.at(yCenter - x).at(xCenter + y) != nullptr){ return false; }
            if(!rotation && grid.at(yCenter + x).at(xCenter - y) != nullptr){ return false; }
        }
        return true;
    }

    void PlaceBlock(){
        int xCenter = currentBlock->center.first, yCenter = currentBlock->center.second;
        for(pair<int, int> eachOffset : currentBlock->offsetCells){
            int x = eachOffset.first, y = eachOffset.second;
            grid.at(yCenter + y).at(xCenter + x) = new Cell{currentBlock->color};
        }
    }

    void AddPoints(int linesCleared){
        if(linesCleared == 1){ score += 40*(level+1); }
        else if(linesCleared == 2){ score += 100*(level+1); }
        else if(linesCleared == 3){ score += 300*(level+1); }
        else if(linesCleared == 4){ score += 1200*(level+1); }
    }

    void ClearLines(){ //only check lines that PlaceBlock() modifies
        vector<int> modifiedLines;
        int linesCleared = 0;
        vector<Cell*> nullRow = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
        for(pair<int, int> eachOffset : currentBlock->offsetCells){
            int y = currentBlock->center.second + eachOffset.second;
            modifiedLines.push_back(y);
        }
        for(int eachLine : modifiedLines){ //check all modified lines
            int fullCells = 0;
            for(Cell* eachCell : grid.at(eachLine)){
                if(eachCell != nullptr){ fullCells++; }
            }
            if(fullCells == WIDTH){ grid.at(eachLine) = nullRow; linesCleared++; }
        }
        vector<vector<Cell*>> v;
        for(vector<Cell*> eachVector : grid){ //if empty lines are present, shift all lines down
            if(eachVector != nullRow){
                v.push_back(eachVector);
            }
        }
        for(int i = v.size(); i < grid.size(); i++){
            v.push_back(nullRow);
        }
        grid = v;

        AddPoints(linesCleared);
        totalLines += linesCleared;
        level = (int)(totalLines / 10); //starting at 0, scale difficulty every 10 lines cleared
    }

    void GenerateGhostBlock(){
        ghostBlock = new Ghost(currentBlock->color, currentBlock->center, currentBlock->offsetCells);

        bool aboveGround = true;
        //int xCenter = ghostBlock->center.first, yCenter = ghostBlock->center.second;
        while(aboveGround){
            for(pair<int, int> eachOffset : ghostBlock->offsetCells){
                int x = eachOffset.first, y = eachOffset.second;
                //cout << y << endl;
                if(ghostBlock->center.second + y == 0){ aboveGround = false; }
                else if(grid.at(ghostBlock->center.second + y-1).at(ghostBlock->center.first + x) != nullptr){ aboveGround = false; }
            }
            if(aboveGround){
                ghostBlock->center.second--;
            }
        }
    }

    void PrintSeveralPixels(int n){
        for(int i = 0; i < n; i++){
            cout << char(219) << char(219);
        }
        cout << endl;
    }

    void Print(){
        cout << "Next Blocks: " << '\t' << '\t' << "Stored Block: " << endl;
        //info bar
        printf("\033[1;30m");
        PrintSeveralPixels(INFOWIDTH + 2);

        for(int i = INFOHEIGHT-1; i >= 0; i--){
            printf("\033[1;30m");
            cout << char(219) << char(219);
            vector<Cell*> eachVector = infoTable.at(i);
            for(Cell* eachCell : eachVector){
                if(eachCell == nullptr){ cout << "  "; }
                else{ printf((eachCell->color).c_str()); cout << char(219) << char(219); }
            }
            printf("\033[1;30m");
            cout << char(219) << char(219) << endl;
        }
        
        //frame
        vector<vector<Cell*>> frame = grid; //every tick, a new frame is created. Both grid and currentblock draw to it
                                            //only when currentblock hits the bottom, does it permanently write to grid
        for(int i = 0; i < 4; i++){
            int xCenter = ghostBlock->center.first, yCenter = ghostBlock->center.second;
            int x = ghostBlock->offsetCells.at(i).first, y = ghostBlock->offsetCells.at(i).second;
            frame.at(yCenter + y).at(xCenter + x) = new Cell{"\033[1;0m"}; //write ghostBlock to frame
        }
        for(int i = 0; i < 4; i++){
            int xCenter = currentBlock->center.first, yCenter = currentBlock->center.second;
            int x = currentBlock->offsetCells.at(i).first, y = currentBlock->offsetCells.at(i).second;
            frame.at(yCenter + y).at(xCenter + x) = new Cell{currentBlock->color}; //write currentBlock to frame
        }
        printf("\033[1;30m");
        PrintSeveralPixels(INFOWIDTH + 2);
        for(int i = HEIGHT-1; i >= 0; i--){
            vector<Cell*> eachVector = frame.at(i);
            printf("\033[1;30m");
            cout << char(219) << char(219);
            for(Cell* eachCell : eachVector){
                if(eachCell == nullptr){ cout << "  "; }
                else{ printf((eachCell->color).c_str()); cout << char(219) << char(219); }
            }
            printf("\033[1;30m");
            cout << char(219) << char(219);
            cout << '\n' << flush;
        }
        printf("\033[1;30m");
        PrintSeveralPixels(12);
        cout << "Score: " << score << endl;
        cout << "Lines Cleared: " << totalLines << endl;
        cout << "Level: " << level << endl;
    }

    void CheckAndExecuteInput(){
        char input = ' ';
        bool gravityEvent = false;
        auto start = chrono::high_resolution_clock::now();
        auto duration = chrono::milliseconds(loopTime);
        bool keyPressedOld = false, keyPressedNew = false;
        while((chrono::high_resolution_clock::now() - start) * (4*(gravityEvent == true)+(level+1)) < duration){ //x4 gravity when holding down
            if(kbhit()){ keyPressedNew = true; input = _getch(); } //idk what happens when 2 inputs at once
            else{ keyPressedNew = false; }
            if(!keyPressedOld && keyPressedNew){ //if risingEdge
                if(input == 'p'){ cout << endl << "PAUSED" << endl; system("Pause"); } //WHY CANT SEVERAL HAPPEN AT ONCE
                else if(input == 'd'){ if(CanMove(RIGHT)){ Move(RIGHT); } }
                else if(input == 'a'){ if(CanMove(LEFT)){  Move(LEFT); } } //change move/rotate logic
                else if(input == 'e'){ if(CanRotate(CW)){  currentBlock->Rotate(CW); } }
                else if(input == 'q'){ if(CanRotate(CCW)){ currentBlock->Rotate(CCW); } }
                else if(input == 's'){ gravityEvent = true; }
                else if(input == 'x'){ InstantDrop(); }
                else if(input == 'r'){ SwapBlock(); }
                input = ' ';
                keyPressedOld = keyPressedNew;
            }
        }
        gravityEvent = false;
    }
};