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
#include <sstream> // Required for converting string to float
#include <cmath>   // Required for handling float comparison

// ...

bool isFloatInput = false; // Flag to indicate if user input is float or int

int Calculation::evaluate_postfix(string postfix)
{
    stack<int> intOperands;
    stack<float> floatOperands;

    for (char c : postfix)
    {
        if (isdigit(c))
        {
            // Convert char digit to integer or float and push onto respective stacks
            if (isFloatInput)
            {
                float floatOperand;
                stringstream ss;
                ss << c;
                ss >> floatOperand;
                floatOperands.push(floatOperand);
            }
            else
            {
                int intOperand = c - '0';
                intOperands.push(intOperand);
            }
        }
        else if (c == '+' || c == '-' || c == '*' || c == '/')
        {
            // Perform arithmetic operation when an operator is encountered
            if (isFloatInput)
            {
                float floatOperand2 = floatOperands.top();
                floatOperands.pop();
                float floatOperand1 = floatOperands.top();
                floatOperands.pop();

                float result;
                switch (c)
                {
                case '+':
                    result = floatOperand1 + floatOperand2;
                    break;
                case '-':
                    result = floatOperand1 - floatOperand2;
                    break;
                case '*':
                    result = floatOperand1 * floatOperand2;
                    break;
                case '/':
                    result = floatOperand1 / floatOperand2;
                    break;
                }

                floatOperands.push(result); // Push the result back to the stack
            }
            else
            {
                int intOperand2 = intOperands.top();
                intOperands.pop();
                int intOperand1 = intOperands.top();
                intOperands.pop();

                int result;
                switch (c)
                {
                case '+':
                    result = intOperand1 + intOperand2;
                    break;
                case '-':
                    result = intOperand1 - intOperand2;
                    break;
                case '*':
                    result = intOperand1 * intOperand2;
                    break;
                case '/':
                    result = intOperand1 / intOperand2;
                    break;
                }

                intOperands.push(result); // Push the result back to the stack
            }
        }
    }

    if (isFloatInput && !floatOperands.empty())
    {
        return floatOperands.top(); // Return the final float result
    }
    else if (!intOperands.empty())
    {
        return intOperands.top(); // Return the final int result
    }
    else
    {
        // Handle error when no result is available
        // For example, when the postfix expression is invalid
        throw runtime_error("Invalid postfix expression");
    }
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