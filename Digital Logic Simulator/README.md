Got bored and reworked my C++ Object Oriented final project from scratch.
Cut it down from 658 lines to 199.
Most of this was programmed in a day. Day 2 was spent bug fixing. Day 3 was refactoring and input validation.
Also got previous circuits that didnt behave properly to work.

To use, you need 2 files in the same directory, in the format exampleCircuit.txt and exampleCircuit_v.txt.
To input in program, input "exampleCircuit" and both will be read.

exampleCircuit.txt contains input/output wires and gates.
exampleCircuit_v.txt contains changes to input/output wires over time, which will cascade to the connected gates

0, _     = false
1, -     = true
2, X     = unknown. Ex) 1 AND 2 results in 2, since the final output could be 0 or 1 depending on 2's observed state
3, Z     = ungenerated. This should never be printed, unless you uncomment the debug print in main()

Simulator was built using a circular dependancy between gates.h and wires.h.
Each wire has a vector<Gate*> to make events based on changes
Each gate has an input1/input2 to read and output to update

This program does not simulate each second. It uses an event queue, only records changes, then fills in the gaps. 
map<int, vector<Event>> queue allows for pairs {time, eventsAtTime}, in case multiple events happen at the same time. 

Im aware that a lot of this isnt normal whitespacing.
If Im using a stringstream to     TXTline >> type >> symbol >> time >> state;
I might as well                  string type; char symbol; int time; char state; 
Some short if/foor loops are better on one line, so that comments for final print output make sense

just compile simulator.cpp. The other 3 files are included
