This program uses a 3D vector of pieces, each with colors in XYZ directions.
The program parses user input and passes that side's center coordinates like BLUE's {0, 1, 0} with clockwise or counterclockwise to a turn function.
That function then uses the given origin, creates an iterable 1D vector around that side's perimeter, then shifts pieces and swaps colors to make the "turn" happen.
Initial scramble uses the previous turn function 20 times, since all combinations are possible with that number of turns. 
As usual, I try to have backend logic in as few lines as possible. This was done in ~170 lines, with a 2D print function being ~30 of them. 

Known issues:
  For whatever reason, inputting literally anything other an CW like "BLUE C" or "BLUE RED" will register as CCW
  Also mistyping a color crashes the program. Not a difficult fix, but that kind of input validation wont be needed once I get a 3D render working. 
  

Folder 1: The cmd version with a 2D print of all the sides. 
  To use - input color in caps + CW or CCW. Examples: "BLUE CW" "ORANGE CCW" or "WHITE CW"
  To scramble again - input "scramble"

Folder 2: If I finish it, this should be an opengl version with a 3D render. 
