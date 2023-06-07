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
vector<string> Calculation::convert_infix_to_postfix(string infix)
{
    int infixStringLength = infix.length();
    stack<char> operators;        // Stack to store operator
    vector<string> tokens;        // Vector to store tokens (operands and operators)
    string currentNumber;         // Temporary string to store the current number being parsed
    string currentVariable;       // Temporary string to store the current variable being parsed
    bool parsingNumber = false;   // Flag to indicate if currently parsing a number
    bool parsingVariable = false; // Flag to indicate if currently parsing a variable
    bool isFloat = false;         // Flag to indicate if
    bool hasDot = false;          // Flag to indicate if dot is present

    if (infix.empty())
    {
        return {"Invalid expression. Expression is empty."};
    }
    if (is_operator(infix[0]))
    {
        return {"Invalid expression. Expression cannot start with an operator."};
    }

    // Iterate through each character in the infix expression
    for (int i = 0; i < infixStringLength; i++)
    {
        if (infix[i] == '.')
        {
            if (parsingVariable)
            {
                return {"Invalid expression. Dot should not be in a variable."};
            }
            if (!parsingNumber)
            {
                return {"Invalid expression. Variables should not start with a dot."};
            }
            if (hasDot)
            {
                return {"Invalid expression. Number has more than one dot."};
            }

            currentNumber += infix[i];
            hasDot = true;
        }
        else if (isdigit(infix[i]) && !parsingVariable)
        {
            // If the character is a digit, append it to the currentNumber string
            currentNumber += infix[i];
            parsingNumber = true;
        }
        else if (is_alpha_numeric(infix[i]))
        {
            if ((isalpha(infix[i]) && parsingNumber) || hasDot)
            {
                return {"Invalid expression. Variables should not start with a number."};
            }

            // If the character is a variable, append it to the currentVariable string
            currentVariable += infix[i];
            parsingVariable = true;
        }
        else
        {
            if (parsingVariable)
            {
                // If parsing a variable, add the currentVariable as a token
                tokens.push_back(currentVariable);
                currentVariable.clear();
                parsingVariable = false;
            }
            if (parsingNumber)
            {
                if (currentNumber.back() == '.')
                {
                    return {"Invalid expression. Number has a dot at the end."};
                }
                if (hasDot != isFloat)
                {
                    return {"Invalid expression. Type mismatch."};
                }

                // If parsing a number, add the currentNumber as a token
                tokens.push_back(currentNumber);
                currentNumber.clear();
                parsingNumber = false;

                if (hasDot && !isFloat)
                {
                    isFloat = true;
                }
                hasDot = false;
            }

            if (is_operator(infix[i]))
            {
                if (is_operator(infix[i + 1]) || infix[i + 1] == '\0')
                {
                    return {"Invalid expression"};
                }

                // If the character is an operator (+, -, *, /)
                while (!operators.empty() && operators.top() != '(' && operators.top() != ')')
                {
                    // Pop operators from the stack and add them as tokens until the top of the stack is '(' or ')'
                    tokens.push_back(string(1, operators.top()));
                    operators.pop();
                }
                operators.push(infix[i]); // Push the current operator onto the stack
            }
            else if (infix[i] == '(')
            {
                if (is_operator(infix[i + 1]))
                {
                    return {"Invalid expression"};
                }

                operators.push(infix[i]); // Push '(' onto the stack
            }
            else if (infix[i] == ')')
            {
                if (!is_operator(infix[i + 1]) && infix[i + 1] != '\0')
                {
                    return {"Invalid expression"};
                }

                // If the character is ')', pop operators from the stack and add them as tokens until '(' is found
                while (!operators.empty() && operators.top() != '(')
                {
                    tokens.push_back(string(1, operators.top()));
                    operators.pop();
                }

                if (operators.empty())
                {
                    return {"Mismatched parentheses in infix expression"};
                }
            }
            else
            {
                // Invalid character detected, return "Invalid expression"
                return {"Invalid expression"};
            }
        }
    }

    if (parsingNumber)
    {
        // If parsing a number at the end, add it as a token
        tokens.push_back(currentNumber);
    }

    while (!operators.empty())
    {
        if (operators.top() == '(' || operators.top() == ')')
        {
            return {"Mismatched parentheses in infix expression"};
        }
        // Pop remaining operators from the stack and add them as tokens
        tokens.push_back(string(1, operators.top()));
        operators.pop();
    }

    return tokens; // Return the vector of tokens (postfix expression)
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