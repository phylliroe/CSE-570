// Name: Andrew Loop-Perez
// 006198799
// Class: CSE 570
// Term: Fall 2019
// Assignment: Lab 1
// Filename: lab1.cpp
// Description: The program simulates the DFA found on page 151 of the textbook using the transition table method.
// The acceptable input for the DFA is: (a|b) *abb. The halt state for the DFA is the state labeled "3"
// If the correct state has been reached after the input string has been processed, print "Yes". Otherwise, print "No"

#include <iostream>

using namespace std;

int dfa(const int [4][2]);
int check_state(char);
int change_sate(int, char, const int [4][2]);
void reject(char);
bool verify(const int [1], const int size, int);

// Check if a valid character has been entered
// Valid characters are 'a' and 'b'
// If the input is valid, return a number corresponding to a column in the transition table
// If the input is invalid, return -1
int check_state(char c)
{
    switch (c)
    {
    case 'a':
        return 0;
    case 'b':
        return 1;
    default:
        return -1;
    }
}

// Change the state of the DFA
// Returns the new state
int change_state(int s, char c, const int table[4][2])
{
    // Get position of the next state in the transition table
    int next_state = check_state(c);
    
    // If the next state is valid, update the state of DFA using the table
    // If not, then reject that character and end the program
    if (next_state != -1)
        s = table[s][next_state];
    else
        reject(c);
    
    return s;
}

// Simulate the DFA
// Takes the transition table as input
// Returns the final state of the DFA
int dfa(const int table [4][2])
{
    int s = 0; // Initial state of the DFA

    // Prompt the user for an input string
    cout << "Please enter a string from the following alphabet: (a|b)*abb" << endl << endl;
    cout << "Enter: ";
    char c = getchar();

    // Read each character of the input string. Stop when the terminating character is reached
    while(c != '\n')
    {
        s = change_state(s, c, table); // Update the state
        c = getchar(); // Read the next character
    } 

    return s;
}

// Check if the DFA is in an acceptable halt state
// If the current value of s is found in the list of acceptable states, return TRUE. Ottherwise, return FALSE
bool verify(const int end [1], const int size, int s)
{
    // Check if the last state of the DFA is one of the accepted end states
    for (int i = 0; i < size; i++)
        if(end[i] == s)
            return true;

    return false;
}

// If an invalid character is found, print a message and terminate the program
void reject(char c)
{
    cout << c << " is not a valid character! This string has been rejected! Try again." << endl;
    exit(1);
}

// Main function
int main() 
{
    // Tranisition table for the DFA
    // Rows = current state (row 0 = state 0, row 1 = state 1, and so on)
    // Columns = next states (col 0 = next state when input is a, col 1 = next state when input is b)
    const int tranistion_table [4][2] = {
        {1, 0},
        {1, 2},
        {1, 3},
        {1, 0}
    };

    const int end_states [1] = {3}; // List of acceptable halt states
    const int end_states_size = 1; // Number of acceptable halt states

    // Simulate the DFA
    // The value of "state" will be the number of the final state of the DFA
    const int state = dfa(tranistion_table);

    // If the end state of the DFA is valid, print "YES"
    // If not, print "NO"
    if (verify(end_states, end_states_size, state))
        cout << "YES" << endl;
    else
        cout << "NO" << endl;

    return 0;
}
