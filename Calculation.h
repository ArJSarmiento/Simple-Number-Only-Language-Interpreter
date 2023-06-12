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
#include <vector>
#include "Utils.h"
#include "Storage.h"
#include "Calculation.h"

using namespace std;

class Calculation
{
    private:
        Storage *storage;
        bool isFloat = false;
        bool isAllVars = true;
    public:
        Calculation(Storage *storage) : storage(storage) {}

        string remove_trailing_zeroes(float num);
        vector<string> convert_infix_to_postfix(string infix);
        string evaluate_postfix(vector<string> postfix);
        void error_reset(string message);
};