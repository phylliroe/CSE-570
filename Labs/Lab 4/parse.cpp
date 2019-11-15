/*
Name: Andrew Loop-Perez
ID: 006198799
Course: CSE 570 Fall 2019
Filename: parse.cpp
Assignment: Lab 3
Description: Definitions for the parsing functions.
*/

#include "parse.h"


/*
=========================================================================
First and Follow Functions
=========================================================================
*/
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

    rhs_instructions();

    count = 0;

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

/*
======================================================================================================
LR(0) Functions
========================================================================================================
*/
// Turn a given production into an augmented production
// Add the '.' character to the beginning of the RHS
// Return the new production
Production* augment(Production* &p)
{
    p->right.insert(p->right.begin(), '.');
    return p;
}

// Calculate the canonical LR(0) sets of the given grammar, v
// Create a new State object for each LR(0) item add add it to the set
// Return the set of LR(0) items
set<State*> canonical(const vector<Production*> v)
{
    set<State*> items; // Set containing the state
    vector<Production*> new_kernel;
    vector<char> char_list;
    int state_num = 0; // Number of a state. Starts at zero for State I0
    char current_char;
    bool changed;

    // Create the kernel of the intital State I0 and add it to the set
    new_kernel.push_back( get_initial(v) );
    items.insert(new State(state_num, new_kernel));

    // Clear new_kernel and increate the state counter by 1
    new_kernel.clear();
    state_num++;

    // Iterators pointing to the first and last States in the set
    set<State*>::iterator itr = items.begin();
    set<State*>::iterator last = items.end();

    // Get the closure for State I0
    (*itr)->calc_closure(v);

    // Calculate the other States
    // Loop until no changes have made (all States caluclated)
    do
    {
        changed = false;

        // Loop through the set
        // For each State I
        while ( itr != items.end() )
        {
            // Get the list of characters that come after the dot
            char_list = getcharlist( (*itr) );

            // For each character in char_list
            for (int i = 0; i < char_list.size(); i++)
            {
                current_char = char_list[i];

                // Get the kernel generated by GOTO(I, char)
                new_kernel = do_goto( (*itr), current_char );

                // If a State with a kernel that matches new_kernel does not exist in the set
                if (!state_exists(new_kernel, items))
                {
                    // Add a new State to the set with new_kernel as the kernel
                    items.insert( new State(state_num, new_kernel) ); 
                    
                    // Point last to the newly added State 
                    last = items.end(); 
                    --last;

                    // Calculate the closure of the new State
                    (*last)->calc_closure(v);
                    
                    state_num++; // Increment the state number
                    new_kernel.clear(); // Clear the values from new_kernel
                    changed = true; // Changes were made
                }
            }

            ++itr; // Move to next State in the set
        }
    } 
    while (changed);
    
    return items;
}

// Augment the starting production in the original grammar
// S -> E becomes S -> .E
Production* get_initial(const vector<Production*> v)
{
    vector<Production*>::const_iterator itr = v.begin();
    Production* p = new Production( (*itr)->left, (*itr)->right );
    p = augment(p);
    return p;
}

// Perform the GOTO action
// Calculate the kernel for the State that results from processing char x in State s
// Return the new kernel
vector<Production*> do_goto(State* s, char x)
{
    vector<Production*> new_kernel; // Kernel of the new state
    char next_char;

    // If there are any productions in the kernel or closure of s 
    // where x comes directly after the dot, add those productions to new_kernel
    get_char_after_dot(s->get_kernel(), new_kernel, x);
    get_char_after_dot(s->get_closure(), new_kernel, x);

    // Increment the dot in each production in new_kernel
    for (int i = 0; i < new_kernel.size(); i++)
        new_kernel[i]->move_dot();

    return new_kernel;
}

// For each production in the vector v, if there is a production such that the character
// that comes after the dot is the same as char x, add that production to new_kernel
void get_char_after_dot(const vector<Production*> v, vector<Production*> &new_kernel, const char x)
{
    char next_char;

    // Loop through each production in v
    for (int i = 0; i < v.size(); i++)
    {
        next_char = v[i]->after_dot(); // Get the character after the dot

        // Check the character
        if (next_char == x)
            new_kernel.push_back( new Production(v[i]->left, v[i]->right) );
    }
}

// Print the States in the set
void printstates(set<State*> states)
{
    set<State*>::const_iterator itr = states.begin();
    
    while (itr != states.end())
    {
        (*itr)->print_state();
        ++itr;
    }
}

// Create a vector of the characters that occur after the dot in the given State
// Return the vector of characters
vector<char> getcharlist(State* s)
{
    vector<char> char_list;
    char c;

    // Check the kernel
    for (int i = 0; i < s->get_kernel().size(); i++)
    {
        // Get the character that comes after the dot
        c = s->get_kernel()[i]->after_dot();

        // If the character is not in the list and is not '\0', add it to the list
        if ( find(char_list.begin(), char_list.end(), c) == char_list.end() )
        {
            if (c != '\0')
                char_list.push_back(c);
        }
    }

    // Check the closure
    for (int j = 0; j < s->get_closure().size(); j++)
    {
        c = s->get_closure()[j]->after_dot();

        if ( find(char_list.begin(), char_list.end(), c) == char_list.end() )
        {
            if (c != '\0')
                char_list.push_back(c);
        }
    }

    return char_list;
}

// Check if a State with a given kernel already exists in the set
// Return TRUE if such a State already exists in the set. Return FALSE is no such State exists
bool state_exists(const vector<Production*> new_kernel, const set<State*> states)
{
    /* 
    Loop through each State in the set. 
    Compare the productions in the kernel of the State and in new_kernel.
    For each comparision, add a TRUE or FALSE to the bool_result vector.
    If all of the elements in bool_result are TRUE, then the State kernel and new_kernel contain the 
    same productions in the same order 
    */

    vector<Production*> v; // Contains the kernel of a given State
    vector<bool> bool_result; // Contains the results of the check
    int size; // Contains the size of the kernels

    // Loop through each State in the set
    for(set<State*>::iterator itr = states.begin(); itr != states.end(); itr++)
    {
        v = (*itr)->get_kernel(); // Kernel of the current State

        // Check if the kernel of the state and new_kernel contain the same number of proudctions
        // If they are not, add a FALSE to the bool vector
        if ( v.size() == new_kernel.size() )
        {
            size = v.size(); // Number of productions in the two kernels

            // For each production in the State and in new_kernel
            // Add a TRUE to the bool vector if, and only if, the productions at indexi in the State kernel 
            // and index i in new_kernel have identical LHS and RHS. Otheriwse add a FALSE to the bool vector
            for (int i = 0; i < size; i++)
            {
                if ( v[i]->left == new_kernel[i]->left )
                {
                    if ( v[i]->right == new_kernel[i]->right )
                        bool_result.push_back(true);
                    else
                        bool_result.push_back(false);
                }
                else
                    bool_result.push_back(false);
            }
        }
        else
            bool_result.push_back(false);

        // If all of the elements in the bool vector are TRUE, return TRUE
        if ( find(bool_result.begin(), bool_result.end(), false) == bool_result.end() )
            return true;

        bool_result.clear(); // Clear the bool vector for the next iteration
    }

    return false; // State with given kernel does not exist
}