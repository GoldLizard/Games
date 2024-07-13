# Games
If I'm bored, I might challenge myself to rewrite a game in cmd in as few lines as possible. 

2048 - requires text input. U, D, L, R
       Missing feature - arrow or WASD input. 
       Done with ~80 lines

Chess - text input like "B2 to B4". Includes move validation and en passant. 
        Unfortunately, the chess characters are actually multicharacters, which do not output correctly in cmd.
        Missing features - castling
        Done with ~260 lines

Tetris - WASD input. q/e to rotate, p for pause, x to instant drop, and r to swap with inventory slot
         Level scales every 10 lines cleared.
         Gravity depends on level.
         Score depends on both level and lines cleared at once.
         Player can see the next 3 blocks, including an inventory slot to swap pieces between.
         Ghost block below to show where the current block will fall to. 
         Missing feature - stop player stalling with infinite swaps
         Done with ~420 lines

Digital Logic Simulator - reworked C++ object oriented final project
                          Event queue instead of simulating each second
                          Original program of 583 lines completely rewritten in 199.
