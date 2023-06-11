#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <cstdio>
#include <stack>
#include <sstream> // Required for converting string to float
#include <cmath>   // Required for handling float comparison
#include "Utils.h"
#include "Storage.h"
#include "Calculation.h"

using namespace std;

class Calculation
{
    public:
        bool isFloat = false;
        vector<string> convert_infix_to_postfix(string infix);
        string evaluate_postfix(vector<string> postfix);
        int precedence(string c);
        string removeTrailingZeroes(float num);
};