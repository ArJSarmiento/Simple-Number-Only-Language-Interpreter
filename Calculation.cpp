#include <iostream>
#include <stack>
#include <string>
#include <algorithm>
#include <cctype>
#include <cstring>
#include "Utils.h"
#include "Calculation.h"

using namespace std;

/*
    This function converts an infix expression to its equivalent postfix expression and evaluates the postfix if valid
    Input: infix expression
    Output: postfix expression
*/
string Calculation::calculate(string infix)
{
    return infix;
}

/*
    This function evaluates a postfix expression
    Input: postfix expression
    Output: result of the expression
*/
int Calculation::evaluate_postfix(string postfix)
{
    return 0;
}

/*
    This function returns the precedence of an operator
    Input: char
    Output: int
*/
int Calculation::precedence(string c)
{
    switch (c[0])
    {
    case '*':
    case '/':
    case '%':
        return 2;
    case '+':
    case '-':
        return 1;
    default:
        return 0;
    }
}