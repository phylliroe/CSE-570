/*
Name: Andrew Loop-Perez
ID: 006198799
Course: CSE 570 Fall 2019
Filename: state.h
Assignment: Lab 4
Description: Header file for the State class. State object is used to represent the various LR(0) items generated
for a given grammar
*/

#ifndef STATE_H
#define STATE_H

#include <set>
#include <map>
#include "productions.cpp"

class State
{
    private:
        // Vector of the productions within the state
        vector<Production*> kernel;
        vector<Production*> closure;
    
        // Contains the number of the state
        // State I0, I1, I2, etc
        int state_number;

        // Contains the next GOTO state for each non-terminal character after the dot
        // in this state.
        map<char, int> gotostates;

        // Change any productions in the state that are in the form: A->.e into A->.
        void epsilon()
        {
            const vector<char> epsilon_only = {'.', 'e'};

            for (int i = 0; i < closure.size(); i++)
            {
                if (closure[i]->right == epsilon_only)
                {
                    closure[i]->right = {'.'};
                }
            }
        }

    public:
        // Default constructor
        State()
        {}

        // Initialize the state number to "num" and the kernel to "new_kernel"
        State( int num, vector<Production*> new_kernel )
        {
            state_number = num;
            kernel = new_kernel;
        }

        // Return the number of this state;
        int get_state_number() const
        {
            return state_number;
        }

        // Return the kernel vector
        vector<Production*> get_kernel() const
        {
            return this->kernel;
        }

        // Return the closure vector
        vector<Production*> get_closure() const
        {
            return this->closure;
        }

        // Print the state
        void print_state() const 
        {
            // Print the number of the state
            cout << "==========" << endl;
            cout << "State " << state_number << ":" << endl;
            cout << "==========" << endl;

            // Print the productions in the kernel of the state
            if (!kernel.empty())
            {
                for (int i = 0; i < kernel.size(); i++)
                    kernel[i]->print();
            }
           
            // Print the productions in the closure of the state
            if (!closure.empty())
            {
                for (int j = 0; j < closure.size(); j++)
                    closure[j]->print();
            }

            cout << endl;
        }

        // Find the productions in the closure of the state
        void calc_closure(const vector<Production*> v)
        {
            int dot_index; 
            int next;
            char next_char;
            Production* new_prod;

            // If the kernel is not empty
            if (!kernel.empty())
            {
                // For Production* in the kernel, get the index of the dot in the right hand side
                for (int i = 0; i < kernel.size(); i++)
                {
                    dot_index = kernel[i]->find_dot();

                    // If the dot is not at the end of the production.
                    // Get the index of the element after the dot and the character that is at that index (next_char).
                    // Add any productions from the original CFG where the left hand side is euqal to next_char to the closure of this state.
                    if (dot_index != (kernel[i]->right.size() - 1))
                    {
                        next = (dot_index + 1);
                        next_char = kernel[i]->right[next];
                        find_productions(v, next_char);
                    }
                }
            }

            // Loop through the closure of the state. Check each production.
            // In each production, if the character that directly follows that dot
            // is a non-terminal, add the productions of that non-terminal from State 0
            // into the closure
            if (!closure.empty())
            {
                bool changed;

                // Repeat the steps until no more changes are made to the closure
                do
                {
                    changed = false;

                    // Loop through the productions in the closure and get the character that comes after the dot
                    for (int i = 0; i < closure.size(); i++)
                    {
                        dot_index = closure[i]->find_dot();

                        if ( dot_index != (closure[i]->right.size() - 1) )
                        {
                            next = (dot_index + 1);
                            next_char = closure[i]->right[next];

                            // Check if the character after the dot is a non-terminal (uppercase)
                            if (isupper(next_char))
                            {
                                // Get the productions from the initial State
                                for (int j = 0; j < v.size(); j++)
                                {
                                    if (v[j]->left == next_char)
                                    {
                                        new_prod = v[j];
                                        new_prod = augment(new_prod); // Augment the production

                                        // Check if the production already exist in the kernel or the closure of this State
                                        if ( !check_if_exists(new_prod, this->kernel) && !check_if_exists(new_prod, this->closure) )
                                        {
                                            closure.push_back(new_prod); // Add production to closure
                                            changed = true; // Changes have been made 
                                        }
                                    }
                                }
                            }
                        }
                    }

                }
                while (changed);
            }

            // Rewrite any productions in the closure that contain only epsilon on the RHS
            epsilon();
        }
        
        // Take a vector of Production pointers v (CFG) and a character c as input
        // If there is a production in v such that the left hand side equals c
        // Create a copy of that production p, augmented p, and add p to the closure 
        void find_productions(const vector<Production*> v, const char c)
        {
            Production* p;
            for (int i = 0; i < v.size(); i++)
            {
                if (v[i]->left == c)
                {
                    p = v[i];
                    p = augment(p);
                    closure.push_back( p );
                }
            }
        }

        // Take a production p as input
        // Augment the production by add a '.' to the beginning of the right hand side
        // Return the newly augmented production
        Production* augment(Production* &p)
        {
            if (p->right[0] != '.')
                p->right.insert(p->right.begin(), '.');
           
            return p;
        }

        // Take a Production p and a vector of productions v as input
        // If there is a production in v such that the left and right hand sides of that production
        // are equal to the left and right hand sides of p, return true. Otherwise, return false.
        // TRUE = A production that matches p already exists in v
        // FALSE = There is no production in v that matches p
        bool check_if_exists(Production* p, const vector<Production*> v)
        {
            Production* aug = p;
            //augment(aug);

            for (int i = 0; i < v.size(); i++)
            {
                if ( (aug->left == v[i]->left) && (aug->right == v[i]->right) )
                    return true;
            }

            return false;
        }

        void add_goto_state(char c, int i)
        {
            if (!check_goto_states(c))
                gotostates.insert( pair<char, int>(c,i) );
        }

        int goto_size() const
        {
            return gotostates.size();
        }

        void printgoto() const
        {
            if (gotostates.empty())
            {
                cout << state_number << endl;
            }
            else
            {
                for (auto elem : gotostates)
                {
                    cout << state_number << ", " << elem.first << ", " << elem.second << endl;
                }
            }
        }

        bool check_goto_states(const int j) const
        {
            for (auto elem : gotostates)
            {
                if (elem.second == j)
                {
                    return true;
                }
            }

            return false;
        }

        int get_goto(char c) 
        {
            for (auto elem : gotostates)
            {
                if (elem.first == c)
                {
                    return elem.second;
                }
            }

            return -1;
        }

        char goto_char(char c)
        {
            for (auto elem : gotostates)
            {
                if (elem.first == c)
                    return elem.first;
            }

            return '\0';
        }
};

#endif







