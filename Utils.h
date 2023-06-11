#pragma once
#include <iostream>
#include <stack>
#include <string>
#include <algorithm>
#include <regex>

using namespace std;

/*
	Header file for function prototypes of utility functions
*/

int analyze_command(string command);
bool check_syntax(string command, int type);

string remove_space(string str);

bool is_operator(char c);
bool is_operator_string(string c);
bool is_variable(string c);
bool is_digit(string c);
bool is_digit(string c);
int precedence(char c);
string strip_spaces(string str);