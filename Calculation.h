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
    vector<string> convert_infix_to_postfix(string infix);
    int evaluate_postfix(string postfix);
    int precedence(string c);
};