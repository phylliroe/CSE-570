/*
Name: Andrew Loop-Perez
ID: 006198799
Course: CSE 570 Fall 2019
Filename: lab3.cpp
Assignment: Lab 3
Description: Main file for lab 3. Prompt the user to enter a grammar in the same way as in lab 2.
Then, generate the canonical LR(0) sets for that grammar. 
*/

#include <iostream>
#include "parse.h"

int main()
{
    vector<Production*> cfg; // Grammar
    set<State*> states; // Set of LR(0) items

    get_input(cfg); // Prompt the user to input the grammar

    cout << endl;

    states = canonical(cfg); // Get the LR(0) items

    // Print the LR(0) items
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" <<endl;
    cout << "The Canonical LR(0) Sets Are..." << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" <<endl;
    
    printstates(states);
    
    cout << endl;
    //cout << states.size() <<endl;

    return 0;
}