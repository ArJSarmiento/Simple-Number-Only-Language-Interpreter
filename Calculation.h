#pragma once
#include <iostream>
#include <stack>
#include <string>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <vector>

using namespace std;

class Calculation
{
    public:
        bool isFloat = false;
        vector<string> convert_infix_to_postfix(string infix);
        float evaluate_postfix(vector<string> postfix);
        int precedence(string c);
};