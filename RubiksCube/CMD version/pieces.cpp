using namespace std;

#define WHITE "\033[1;0m" //changing colors by printing this as .c_str()
#define YELLOW "\033[38;5;226m"
#define ORANGE "\033[38;5;130m"
#define BLUE "\033[34m"
#define RED "\033[38;5;196m"
#define GREEN "\033[32m"

struct Piece {
    string colorX, colorY, colorZ;
    void SwapYZ(){ string temp = colorY; colorY = colorZ; colorZ = temp; }
    void SwapXZ(){ string temp = colorX; colorX = colorZ; colorZ = temp; }
    void SwapXY(){ string temp = colorX; colorX = colorY; colorY = temp; }
};
//below functionally do nothing. They exist to show what cube.grid is
struct Corner : Piece {}; //all 3 active
struct Edge : Piece {}; //only 2 active
struct Center : Piece {}; //only 1 active, never moves
struct Core : Piece {}; //exists on the off chance its needed to not crash