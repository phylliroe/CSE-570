#include "parse.h"

typedef map< int, map<char, string> > actionmap; 

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

    return 0;
}