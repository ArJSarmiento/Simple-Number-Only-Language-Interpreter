#include <iostream>
#include <stack>
#include <string>
#include <algorithm>
#include <cctype>
#include <cstring>
#include "Utils.h"
#include "Calculation.h"
#include <sstream> // Required for converting string to float
#include <cmath>   // Required for handling float comparison

using namespace std;


/*
    This function converts an infix expression to its equivalent postfix expression and evaluates the postfix if valid
    Input: infix expression
    Output: postfix expression
*/
vector<string> Calculation::calculate(string infix)
{
    stack<char> operators;  // Stack to store operators
    vector<string> tokens;  // Vector to store tokens (operands and operators)
    string currentNumber;   // Temporary string to store the current number being parsed
    bool parsingNumber = false;  // Flag to indicate if currently parsing a number

    // Iterate through each character in the infix expression
    for (char& ch : infix) {
        if (isspace(ch)) {
            continue;  // Skip whitespace characters
        }
        else if (isdigit(ch)) {
            // If the character is a digit, append it to the currentNumber string
            currentNumber += ch;
            parsingNumber = true;
        }
        else {
            if (parsingNumber) {
                // If parsing a number, add the currentNumber as a token
                tokens.push_back(currentNumber);
                currentNumber.clear();
                parsingNumber = false;
            }

            if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
                // If the character is an operator (+, -, *, /)
                while (!operators.empty() && operators.top() != '(' && operators.top() != ')') {
                    // Pop operators from the stack and add them as tokens until the top of the stack is '(' or ')'
                    tokens.push_back(string(1, operators.top()));
                    operators.pop();
                }
                operators.push(ch);  // Push the current operator onto the stack
            }
            else if (ch == '(') {
                operators.push(ch);  // Push '(' onto the stack
            }
            else if (ch == ')') {
                // If the character is ')', pop operators from the stack and add them as tokens until '(' is found
                while (!operators.empty() && operators.top() != '(') {
                    tokens.push_back(string(1, operators.top()));
                    operators.pop();
                }
                if (!operators.empty() && operators.top() == '(') {
                    operators.pop();  // Pop '(' from the stack
                }
            }
            else {
                // Invalid character detected, return "Invalid expression"
                return {"Invalid expression"};
            }
        }
    }

    if (parsingNumber) {
        // If parsing a number at the end, add it as a token
        tokens.push_back(currentNumber);
    }

    while (!operators.empty()) {
        // Pop remaining operators from the stack and add them as tokens
        tokens.push_back(string(1, operators.top()));
        operators.pop();
    }

    return tokens;  // Return the vector of tokens (postfix expression)
}

/*
    This function evaluates a postfix expression
    Input: postfix expression
    Output: result of the expression
*/

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