/*
Name: Andrew Loop-Perez
ID: 006198799
Course: CSE 570 Fall 2019
Filename: productions.cpp
Assignment: Lab 3
Description: Struct used to represent the productions of a grammar
*/

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Production
{
    // LHS -> RHS
    char left; // Left hand side of the production 
    vector<char> right; // Right hand side of the produiction

    Production()
    {}

    // Create a production with LHS = c and RHS = v
    Production(char c, vector<char> v)
    {
        this->left = c;
        this->right = v;
    }

    // Print the production in the format: A -> aBb
    void print() const
    {
        cout << left << " -> ";
        for (int i = 0; i < right.size(); i++)
            cout << right[i];

        cout << endl;
    }

    // Find the index of the character '.' in the RHS
    // Return the index of the dot
    int find_dot()
    {
        vector<char>::iterator itr = find(right.begin(), right.end(), '.');
        const int index = distance(right.begin(), itr);
        return index;
    }

    // Return the character that comes directly after the dot in the RHS
    // If the dot is at the end of the productions, returns '\0'
    char after_dot()
    {
        char char_after_dot;
        int index = find_dot();
        int next;

        if (index != (right.size() - 1))
        {
            next = index + 1;
            return right[next];
        }

        return '\0';
    }

    // Increment the dot's position by one
    // Swap the dot with the next character after it 
    void move_dot()
    {
        int dot_pos = find_dot();

        if (dot_pos != (right.size() - 1))
        {
            int next_pos = (dot_pos + 1);
            iter_swap(right.begin() + dot_pos, right.begin() + next_pos);
        }
    }
};