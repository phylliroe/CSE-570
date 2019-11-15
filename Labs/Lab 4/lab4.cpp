/*
Name: Andrew Loop-Perez
ID: 006198799
Class: CSE 570 Fall 2019
Assignment: Lab 4
Filename: lab4.cpp
Description: Create three different grammars. Then use the program from Lab 3 to generate the canonical LR(0) sets for each grammar.
Look at each grammar and determine if they are SLR. If they are not, state where the conflicts are. 
*/

#include "parse.h"

int main()
{
    // Prompt the user to enter the grammar
    vector<Production*> grammar;
    get_input(grammar);

    // Calculate the canonical LR(0) sets
    set<State*> states = canonical(grammar);

    cout << endl;

     // Print the LR(0) items
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" <<endl;
    cout << "The Canonical LR(0) Sets Are..." << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" <<endl;
    
    printstates(states);
    
    cout << endl;
    
    return 0;
}