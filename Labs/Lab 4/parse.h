/*
Name: Andrew Loop-Perez
ID: 006198799
Course: CSE 570 Fall 2019
Filename: parse.h
Assignment: Lab 4
Description: Header file for the various parsing functions. Contains the functions used for generating the 
First and Follow sets and the canonical LR(0) sets of a given grammar
*/

#ifndef PARSE_H
#define PARSE_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <string>
#include "state.h"

using namespace std;


// First and follow sets will be represented using a map of sets
// Each key is a unqiue terminal or nonterminal character
// The value for each key is a set containing the elements of either that character's first or follow set
typedef map< char, set<char> > mapset;


/*
==========================================================
User Input Functions
========================================================== */
void get_input(vector<Production*>&);
void lhs_instructions();
void rhs_instructions();

/*
==========================================================
First and Follow Functions
========================================================== */
bool find_epsilon(vector<char>);
void first(vector<Production*>, mapset&);
void insert_if_not_found(mapset&, char, char, bool&);
void follow(vector<Production*>, mapset&, mapset, vector<char>);
bool check_if_contains_epsilon(mapset, char);
void add_sets(mapset, char, mapset&, char, bool&);
void print_sets(mapset, vector<char>, string);
void print(vector<Production*>);
vector<char> get_map_order(vector<Production*>);


/*
============================================================
LR(0) Functions
============================================================= */
vector<Production*> augment(const vector<Production*> v);
set<State*> canonical(const vector<Production*> v);
Production* get_initial(const vector<Production*> v);
vector<Production*> do_goto(State* s, char x);
void get_char_after_dot(const vector<Production*> v, vector<Production*> &new_kernel, const char x);
void printstates(set<State*> states);
vector<char> getcharlist(State* s);
bool state_exists(const vector<Production*> new_kernel, const set<State*> states);

#endif 