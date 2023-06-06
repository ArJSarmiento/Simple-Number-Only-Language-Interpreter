#pragma once
#include <iostream>
#include <stack>
#include <string>
#include <algorithm>
#include <cctype>
#include <cstring>

using namespace std;

class Calculation
{

public:
    vector<string> calculate(string infix);
    int evaluate_postfix(string postfix);
    int precedence(string c);
    bool isValidVariableName(const string& name); // Added for infix to postfix input validation
};