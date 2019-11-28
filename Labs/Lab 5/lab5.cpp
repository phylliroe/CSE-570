#include "parse.h"

typedef map< int, map<char, string> > actionmap; 

bool check_terminal(Production* p, char&c);
actionmap get_action(const set<State*> states);
bool check_at_end(Production *p);
actionmap get_action(const set<State*> states, mapset follow, State* s);
void print_action(const actionmap action, const set<State*> states, const vector<char>order);
void print_goto(const vector<char> order, const set<State*> states, int state_num);

bool check_terminal(Production* p, char& c)
{
    c = p->after_dot();

    if ( (c != '\0') && (!isupper(c)) )
    {
        return true;
    }

    return false;
}

bool check_at_end(Production* p)
{
    //char c = p->after_dot();
    return p->at_end();
}

void calc_action(map<char, string>& state_actions, vector<Production*> v, int i, mapset follow, State* s)
{
    int j;
    char j_char, current_char, prev, next;
    string shift, reduce;

    for (vector<Production*>::iterator itr = v.begin(); itr != v.end(); itr++)
    {
        // If A -> a.
        if (check_at_end( (*itr) ))
        {
            prev = (*itr)->before_dot();

            // If A->a. where a is terminal
            if ( !isupper(prev) )
            {
                current_char = prev;
                reduce = "reduce ";
                reduce += current_char;
                state_actions.insert( pair<char, string>(current_char, reduce) );
            }
            // If A-> a. where a is nonterminal
            else
            {
                if ((*itr)->left == 'S')
                {
                    state_actions.insert( pair<char, string>('$', "accept") );
                }
                else
                {
                    for (auto prod : follow)
                    {
                        if (prod.first == (*itr)->left)
                        {
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
        // If A -> ?.a?
        else
        {
            next = (*itr)->after_dot();

            if ( !isupper(next) )
            {
                j = s->get_goto(next);

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

actionmap get_action(const set<State*> states, mapset follow)
{
    actionmap actions;
    int i, j;
    char j_char, current_char, prev;
    map<char, string> state_actions;
    string shift, reduce;

    for (set<State*>::iterator state_itr = states.begin(); state_itr != states.end(); state_itr++)
    {
        i = (*state_itr)->get_state_number();

        /*
        // Loop through kernel
        for (vector<Production*>::iterator k_itr = (*state_itr)->get_kernel().begin(); k_itr != (*state_itr)->get_kernel().end(); k_itr++)
        {
            // If the dot is the last character in the RHS A -> a.
            if (check_at_end( (*k_itr) ) ) // SEGFAULT
            {
                cout << "AAAAA" << endl;
                prev = (*k_itr)->before_dot();

                // If A -> a., where a is terminal
                if ( !isupper(prev) )
                {
                    current_char = prev;
                    reduce = "r";
                    reduce += current_char;
                    state_actions.insert( pair<char, string>(current_char, reduce) );
                }
                // If A -> a. where a is nonterminal
                else
                {
                    for (auto prod : follow)
                    {
                        if (prod.first == (*k_itr)->left)
                        {
                            for (auto terminal : prod.second)
                            {
                                reduce = "r";
                                reduce += terminal;
                                state_actions.insert( pair<char, string>(terminal, reduce) );
                            }
                        }
                    }
                }
            }
        }
        */
        calc_action(state_actions, (*state_itr)->get_kernel(), i, follow, (*state_itr));
        calc_action(state_actions, (*state_itr)->get_closure(), i, follow, (*state_itr));
    
        // Loop through closure
        actions.insert( pair< int, map<char, string> >(i, state_actions) );
        state_actions.clear();
    }
   
    return actions;
}

void print_action(const actionmap action, const set<State*> states, const vector<char>order)
{
    for (auto elem : action)
    {
        cout << "State: " << elem.first << endl;

        cout << "ACTION: " << endl;

        for (auto a : elem.second)
        {
            cout << a.first << ": " << a.second << endl;
        }

        print_goto(order, states, elem.first);

        cout << endl;
    }
}

void print_goto(vector<char> order, const set<State*> states, int state_num)
{
    int next;

    cout << "GOTO: " << endl;

    for (auto i : states)
    {
        if (i->get_state_number() == state_num)
        {
            for (int j = 0; j < order.size(); j++)
            {
                cout << order[j] << ": ";
        
                if (i->check_goto_states(j))
                {
                    next = i->get_goto(order[j]);

                    if (next != '\0')
                    {
                        cout << next << endl;
                    }
                }
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

    cfg.push_back(new Production()); // s
    cfg.push_back(new Production()); // e
    cfg.push_back(new Production()); // e
    cfg.push_back(new Production()); // t
    cfg.push_back(new Production()); // t
    cfg.push_back(new Production()); // f
    cfg.push_back(new Production()); // f

    cfg[0]->left = 'S';
    cfg[1]->left = 'E';
    cfg[2]->left = 'E';
    cfg[3]->left = 'T';
    cfg[4]->left = 'T';
    cfg[5]->left = 'F';
    cfg[6]->left = 'F';
    
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


    while ( itr != states.end()) 
    {
        //cout << (*itr)->goto_size() << endl;
        (*itr)->printgoto();
        cout << endl;

        ++itr;
    }

    action = get_action(states, follow_sets);
    cout << action.size() << endl;

    for (auto elem : action)
    {
        cout << elem.first << ",";

        cout << elem.second.size();

        cout << endl;
    }

    cout << endl;

    print_action(action, states, map_order);

    return 0;
}