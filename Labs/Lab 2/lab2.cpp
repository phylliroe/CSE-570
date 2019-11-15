/*
Name: Andrew Loop-Perez
ID: 006198799
Course: CSE 570 Fall 2019
Filename: lab2.cpp
Assignment: Lab 2
Description: File for Lab #2. Prompts the user to input a grammar. Then calculates the FIRST and FOLLOW sets of that grammar
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <string>

using namespace std;

// First and follow sets will be represented using a map of sets
// Each key is a unqiue terminal or nonterminal character
// The value for each key is a set containing the elements of either that character's first or follow set
typedef map< char, set<char> > mapset;

// Each production in the grammar is represented by a struct
// left is the left hand side of the production
// right is a vector of each character on the right hand side of the production
struct Production
{
    char left;
    vector<char> right;
};

bool find_epsilon(vector<char>);
void first(vector<Production*>, mapset&);
void insert_if_not_found(mapset&, char, char, bool&);
void follow(vector<Production*>, mapset&, mapset, vector<char>);
bool check_if_contains_epsilon(mapset, char);
void add_sets(mapset, char, mapset&, char, bool&);
void print_sets(mapset, vector<char>, string);
void print(vector<Production*>);
void get_input(vector<Production*>&);
vector<char> get_map_order(vector<Production*>);
void lhs_instructions();
void rhs_instructions();

// Calculate the Frist sets
void first(vector<Production*> grammar, mapset& firstsets)
{
    bool changed;
    char x, a;
    mapset terminal_firsts;

    // Rule 1. if X is terminal, FIRST(X) = {X}
    for (vector<Production*>::iterator terminals = grammar.begin(); terminals != grammar.end(); terminals++)
    {
        for (vector<char>::iterator itr = (*terminals)->right.begin(); itr != (*terminals)->right.end(); itr++)
        {
            if ( !isupper( (*itr) ) && terminal_firsts.find((*itr)) == terminal_firsts.end())
            {
                //terminal_firsts.insert(pair< char, set<char> > ( (*itr), set<char>() ));
                //terminal_firsts[(*itr)].insert((*itr));
                firstsets.insert(pair< char, set<char> > ( (*itr), set<char>() ));
                firstsets[(*itr)].insert((*itr));
            }
        }
    }

    // Rule 2: If X->e is a production, then add e to FIRST(X)
    for (vector<Production*>::iterator epsilon_itr = grammar.begin(); epsilon_itr != grammar.end(); epsilon_itr++)
    {
        if (find_epsilon((*epsilon_itr)->right))
        {
            firstsets[(*epsilon_itr)->left].insert('e');
        }
    }

    // Rule 3
    do
    {
        changed = false;

        for(vector<Production*>::iterator cfg_itr = grammar.begin(); cfg_itr != grammar.end(); cfg_itr++)
        {
            a = (*cfg_itr)->left;
            x = (*cfg_itr)->right.front();
            //cout << "---" << x << "---" << endl;
            if (isupper(x))
            {
                //cout << x << " IS NONTERMINAL!!!" << endl;
                for (set<char>::iterator nt = firstsets[x].begin(); nt != firstsets[x].end(); nt++)
                {
                    //cout << (*nt) << "AAAA" << endl;
                    insert_if_not_found(firstsets, a, (*nt), changed);
                }
            }
            else
            {
                //for (set<char>::iterator t = terminal_firsts[x].begin(); t != terminal_firsts[x].end(); t++)
                for (set<char>::iterator t = firstsets[x].begin(); t != firstsets[x].end(); t++)
                {
                    insert_if_not_found(firstsets, a, (*t), changed);
                }
            }
        }        
    }
    while(changed);
}

// Check if a production contains epsilon
// Returns TRUE if epsilon is found. Returns FALSE otherwise
bool find_epsilon(vector<char>v)
{
    if (find(v.begin(), v.end(), 'e') != v.end())
        return true;
    else
        return false;
}

// Checks if the character c is present in the set s belonging to key
// If c is not found, c is inserted into the set and the value changed is set to true
void insert_if_not_found(mapset& s, char key, char c, bool& changed)
{
    if (s[key].count(c))
    {}
    else 
    {
        s[key].insert(c);
        changed = true;
    }
}

// Calculate the Follow sets
void follow(vector<Production*> grammar, mapset& followsets, mapset firstsets, vector<char> map_order)
{
    bool contains_epsilon;
    bool changed;
    int nt_index, next_index;
    char start = map_order.front();
    followsets[start].insert('$');

    do {
        changed = false;

        // For every production A
        for (vector<Production*>::iterator cfg_itr = grammar.begin(); cfg_itr != grammar.end(); cfg_itr++)
        {
            // A->left
            char lhs = (*cfg_itr)->left;

            // For each element X in A->right
            for (vector<char>::iterator rhs_itr = (*cfg_itr)->right.begin(); rhs_itr != (*cfg_itr)->right.end(); rhs_itr++)
            {
                char current_nt;
                // If X is NonTerminal
                if (isupper((*rhs_itr)) )
                {
                    // If X is not the last element in A->right
                    // A->aXb
                    if ( (*rhs_itr) != (*cfg_itr)->right.back() )
                    {
                        vector<char>::iterator next_elem = rhs_itr;
                        ++next_elem; // b
                        char b = (*next_elem);

                        // Check if FIRST(b) contains epsilon
                        contains_epsilon = check_if_contains_epsilon(firstsets, b);
                        
                        // Add everything in FIRST(b) into FOLLOW(A)
                        add_sets(firstsets, b, followsets, (*rhs_itr), changed);

                        // if FIRST(b) contains epsilon, add everything in FOLLOW(A)
                        // to FOLLOW(B)
                        if (contains_epsilon)
                        {
                            add_sets(followsets, lhs, followsets, (*rhs_itr), changed);
                        }
                    }
                    // If A->aB, add everything in FOLLOW(A) into FOLLOW(B)
                    else if ( (*rhs_itr) == (*cfg_itr)->right.back() )
                    {
                        add_sets(followsets, lhs, followsets, (*rhs_itr), changed);
                    }
                }
            }
        }
    }
    while(changed);
}

// Checks if a set s belonging to key contains epsilon
// Returns true if epsilon is found. Returns false otherwise. 
bool check_if_contains_epsilon(mapset s, char key)
{
    if (s[key].count('e'))
        return true;
    else
        return false;
}

// Copies elements from the set a[a_key] into the set b[b_key]
// Loops through a[a_key]. Only adds the element to b[b_key] if the element 
// is NOT epsilon and is not already present in b[b_key]
void add_sets(mapset a, char a_key, mapset& b, char b_key, bool& changed)
{
    for (set<char>::iterator a_itr = a[a_key].begin(); a_itr != a[a_key].end(); a_itr++)
    {
        if ((*a_itr) != 'e')
        {
            if (b[b_key].count( (*a_itr) ))
            {}
            else 
            {
                b[b_key].insert((*a_itr));
                changed = true;
            }
        }
    }
}

// Print the productions of the grammar
// Loops through the vector containing the productions
// Prints them in the format: left -> right
void print(vector<Production*> v)
{
    for (int i = 0; i < v.size(); i++)
    {
        cout << v[i]->left << " -> "; 
        for (int j = 0; j < v[i]->right.size(); j++)
        {
            cout << v[i]->right[j];
        }

        cout << endl;
    }
}

void print_sets(mapset fs, vector<char> map_order, string set_type)
{
    for (vector<char>::iterator itr = map_order.begin(); itr != map_order.end(); itr++)
    {
        cout << set_type << "(" << (*itr) << ") = {";
        for (set<char>::iterator itr2 = fs[(*itr)].begin(); itr2 != fs[(*itr)].end(); itr2++)
        {
            cout << (*itr2) << ", ";
        }

        cout << "}" << endl;
    }
}

// Prompt the user to input a grammar
void get_input(vector<Production*>& v)
{
    char lhs;
    string rhs;
    int count = 0;

    lhs_instructions();

    while (true)
    {
        cin >> lhs;

        if (lhs == '$')
            break;

        v.push_back(new Production());
        v[count]->left = lhs;
        count++;
    }

    count = 0;

    rhs_instructions();

    while (count < v.size())
    {
        cout << v[count]->left << "->";
        cin >> rhs;

        if (rhs == "$")
            break;

        for (char const &c: rhs)
            v[count]->right.push_back(c);

        count++;
    }

}

void lhs_instructions()
{
    cout << "Please input the grammar that will be used for this program." << endl;
    
    cout << "First, enter the non-terminals on the left had side" 
    << "of each production in your grammar" << endl;
    
    cout << "Enter the non-terminals as follows:" << endl;
    cout << "non-terminal" << endl << "non-terminal" << endl << "..." 
    << endl << "non-terminal" << endl << "$" << endl;
    cout << "(Enter i for id and e for epsilon)" << endl;

    cout << "End the input by entering $" << endl;


}

void rhs_instructions()
{
    cout << endl;
    cout << "Now, enter the right hand side of each production." << endl;
    cout << "(Enter the characters as a single string, NO WHITESPACE)" << endl
    << "(Example: Enter aBb for A to get production...A -> aBb)" << endl;
}

// Get the order that of the non-terminals on the LHS of the grammar from top to bottom
vector<char> get_map_order(vector<Production*> cfg)
{
    vector<char>map_order;

    for (vector<Production*>::iterator order_itr = cfg.begin(); order_itr != cfg.end(); order_itr++)
    {
        if ( find(map_order.begin(), map_order.end(), (*order_itr)->left) == map_order.end() )
        {
            map_order.push_back((*order_itr)->left);
        }
    }

    return map_order;
}

// Main functions
int main()
{
    // Prompt the user for the grammar
    vector<Production*>cfg;
    get_input(cfg);
 
    mapset first_sets; // First sets of the grammar
    mapset follow_sets; // Follow sets of the grammar
    vector<char> map_order = get_map_order(cfg); // Get the ordr of the non-terminals
    
    cout << endl;

    // Print the grammar
    cout << "The Grammar is: " << endl;
    print(cfg);

    cout << endl;
    
    first(cfg, first_sets); // Get the first sets
  
    // Print the first sets
    cout << "=======================" << endl;
    cout << "The FIRST Sets Are..." << endl;
    cout << "=======================" << endl;
    print_sets(first_sets, map_order, "FIRST");

    cout << endl;

    follow(cfg, follow_sets, first_sets, map_order); // Get the follow sets
    
    // Print the follow sets
    cout << "=======================" << endl;
    cout << "The FOLLOW Sets Are..." << endl;
    cout << "=======================" << endl;
    print_sets(follow_sets, map_order, "FOLLOW");

    return 0;
}