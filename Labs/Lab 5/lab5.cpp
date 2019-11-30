/*
Name: Andrew Loop-Perez
ID: 006198799
Course: CSE 570 Fall 2019
Filename: lab5.cpp
Assignment: Lab 5
Description: Generate the ACTION and GOTO tables for the following grammar: 

S -> E
E -> E + T
E -> T
T -> T * F
T -> F
F -> ( E )
F -> id

*/

#include "parse.h"

typedef map< int, map<char, string> > actionmap; 

bool check_terminal(Production* p, char&c);
actionmap get_action(const set<State*> states);
bool check_at_end(Production *p);
actionmap get_action(const set<State*> states, mapset follow, State* s);
void print_action(const actionmap action, const set<State*> states, const vector<char>order);
void print_goto(const vector<char> order, const set<State*> states, int state_num);

// Check if the character after the dot in a production is terminal
bool check_terminal(Production* p, char& c)
{
    c = p->after_dot();

    if ( (c != '\0') && (!isupper(c)) )
    {
        return true;
    }

    return false;
}

// Return true if the dot in a proudction is at the end of the production
bool check_at_end(Production* p)
{
    //char c = p->after_dot();
    return p->at_end();
}

// Calculate the value for ACTION[i, c], where i is the number of a given state and c is a character
// from a production in that state
void calc_action(map<char, string>& state_actions, vector<Production*> v, int i, mapset follow, State* s)
{
    int j;
    char j_char, current_char, prev, next;
    string shift, reduce;

    // Loop through the productions in the state
    for (vector<Production*>::iterator itr = v.begin(); itr != v.end(); itr++)
    {
        // If A -> a.
        if (check_at_end( (*itr) ))
        {
            // Get the character that comes before the dot
            prev = (*itr)->before_dot();

            // Check if a is terminal
            if ( !isupper(prev) )
            {
                // ACTION[i, a] = reduce
                current_char = prev;
                reduce = "reduce ";
                reduce += current_char;
                state_actions.insert( pair<char, string>(current_char, reduce) );
            }
            // If A-> a. where a is nonterminal
            else
            {
                // Check if A is 'S'
                if ((*itr)->left == 'S')
                {
                    // ACTION[i, $] = accept
                    state_actions.insert( pair<char, string>('$', "accept") );
                }
                else
                {
                    // Loop through the FOLLOW sets
                    for (auto prod : follow)
                    {
                        // Find the set where the nonterminal character equals A
                        if (prod.first == (*itr)->left)
                        {
                            // Loop through the follow set of A
                            // ACTION[i, x] = reduce, for each x in FOLLOW(A)
                            for (auto terminal : prod.second)
                            {
                                reduce = "reduce ";
                                reduce += terminal;
                                state_actions.insert( pair<char, string>(terminal, reduce) );
                            }
                        }
                    }       
                }
            }
        }
        // If A -> x.ax, where the dot is not at the end of the production
        else
        {
            // Get the character that comes after the dof
            next = (*itr)->after_dot();

            // If the next character is terminal
            if ( !isupper(next) )
            {
                // Get the number of the state created by GOTO(i, next)
                j = s->get_goto(next);

                // If j is the number of an existing state and is not equal to i
                // ACTION[i, next] = shift
                if ( (j != -1) && (j != i) )
                {
                    current_char = next;
                    shift = "shift ";
                    j_char = (char)j;
                    shift += to_string(j);

                    state_actions.insert( pair<char, string>(current_char, shift) );
                }
            }
        }   
    }
}

// Calculate the ACTION values for the states
actionmap get_action(const set<State*> states, mapset follow)
{
    actionmap actions;
    int i, j;
    char j_char, current_char, prev;
    map<char, string> state_actions;
    string shift, reduce;

    // Loop through each state in the set
    for (set<State*>::iterator state_itr = states.begin(); state_itr != states.end(); state_itr++)
    {
        // get the number of the current state
        i = (*state_itr)->get_state_number();

        // get the ACTION values for the kernel and closure of the current state
        calc_action(state_actions, (*state_itr)->get_kernel(), i, follow, (*state_itr));
        calc_action(state_actions, (*state_itr)->get_closure(), i, follow, (*state_itr));
    
        // Insert the state number and the associated ACTION values into the action map
        actions.insert( pair< int, map<char, string> >(i, state_actions) );
        
        // Clear state_actions for the next iteration
        state_actions.clear();
    }
   
    // Return the ACTION map
    return actions;
}

// Print the ACTION values for a state
// Also calls the print_goto function to print the GOTO values
void print_action(const actionmap action, const set<State*> states, const vector<char>order)
{
    for (auto elem : action)
    {
        // Print the state number
        cout << "State: " << elem.first << endl;

        cout << "ACTION: " << endl;

        // Print the ACTION values
        for (auto a : elem.second)
        {
            cout << a.first << ": " << a.second << endl;
        }

        // Call print_goto to print the GOTO values
        print_goto(order, states, elem.first);

        cout << endl;
    }
}

// Print the GOTO values for a state
void print_goto(vector<char> order, const set<State*> states, int state_num)
{
    int next;

    cout << "GOTO: " << endl;

    // Find the current state in the ste
    for (auto i : states)
    {
        if (i->get_state_number() == state_num)
        {
            // Loop through the nonterminals in the grammar
            for (int j = 0; j < order.size(); j++)
            {
                cout << order[j] << ": ";
        
                // If there is a GOTO value for the nonterminal in the 
                // gotostates map for the current state print the value
                if (i->check_goto_states(j))
                {
                    next = i->get_goto(order[j]);

                    if (next != '\0')
                    {
                        cout << next;
                    }
                }

                cout << endl;
            }
        }
    }
    cout << endl;
}

int main()
{
    vector<Production*> cfg;
    set<State*> states;
    mapset first_sets;
    mapset follow_sets;
    vector<char> map_order;

    actionmap action;

    cfg.push_back(new Production()); // S
    cfg.push_back(new Production()); // E
    cfg.push_back(new Production()); // E
    cfg.push_back(new Production()); // T
    cfg.push_back(new Production()); // T
    cfg.push_back(new Production()); // F
    cfg.push_back(new Production()); // F

    // Add the values for the left hand sides of the productions
    cfg[0]->left = 'S';
    cfg[1]->left = 'E';
    cfg[2]->left = 'E';
    cfg[3]->left = 'T';
    cfg[4]->left = 'T';
    cfg[5]->left = 'F';
    cfg[6]->left = 'F';
    
    // Add the values for the right hand sides of the productions
    cfg[0]->right = {'E'};
    cfg[1]->right = {'E', '+', 'T'};
    cfg[2]->right = {'T'};
    cfg[3]->right = {'T', '*',  'F'};
    cfg[4]->right = {'F'};
    cfg[5]->right = {'(', 'E', ')'};
    cfg[6]->right = {'i'};


    map_order = get_map_order(cfg);
    
    first(cfg, first_sets);
    follow(cfg, follow_sets, first_sets, map_order);

    //print_sets(follow_sets, map_order, "FOLLOW");

    states = canonical(cfg);

    cout << endl;

    //printstates(states);

    set<State*>::iterator itr = states.begin();

    action = get_action(states, follow_sets);
    
    //cout << action.size() << endl;
    
    cout << endl;

    print_action(action, states, map_order);

    return 0;
}