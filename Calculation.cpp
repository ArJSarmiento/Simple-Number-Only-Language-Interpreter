#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <cstdio>
#include "Utils.h"
#include "Calculation.h"
#include <stack>
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
    bool hasDot = false;          // Flag to indicate if dot is present

    if (infix.empty())
    {
        cout << "Invalid expression. Expression is empty." << endl;
        return {};
    }
    if (is_operator(infix[0]))
    {
        cout << "Invalid expression. Expression cannot start with an operator." << endl;
        return {};
    }

    // Iterate through each character in the infix expression
    for (int i = 0; i < infixStringLength; i++)
    {
        if (infix[i] == '.')
        {
            if (parsingVariable)
            {
                cout << "Invalid expression. Dot should not be in a variable." << endl;
                return {};
            }
            if (!parsingNumber)
            {
                cout << "Invalid expression. Variables should not start with a dot." << endl;
                return {};
            }
            if (hasDot)
            {
                cout << "Invalid expression. Number has more than one dot." << endl;
                return {};
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
                cout << "Invalid expression. Variables should not start with a number." << endl;   
                return {};
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
                    cout << "Invalid expression. Number has a dot at the end." << endl;
                    return {};
                }
                if (hasDot != isFloat)
                {
                    cout << "Invalid expression. Type mismatch." << endl;
                    return {};
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
                    cout << "Invalid expression" << endl;
                    return {};
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
                    cout << "Invalid expression" << endl;
                    return {};
                }

                operators.push(infix[i]); // Push '(' onto the stack
            }
            else if (infix[i] == ')')
            {
                if (!is_operator(infix[i + 1]) && infix[i + 1] != '\0')
                {
                    cout << "Invalid expression" << endl;
                    return {};
                }

                // If the character is ')', pop operators from the stack and add them as tokens until '(' is found
                while (!operators.empty() && operators.top() != '(')
                {
                    tokens.push_back(string(1, operators.top()));
                    operators.pop();
                }

                if (operators.empty())
                {
                    cout << "Mismatched parentheses in infix expression" << endl;
                    return {};
                }
            }
            else
            {
                // Invalid character detected, return "Invalid expression"
                cout << "Invalid expression." << endl;
                return {};
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
            cout << "Mismatched parentheses in infix expression" << endl;
            return {};
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
float Calculation::evaluate_postfix(vector<string> postfix)
{
    vector<int> intOperands; // Temporary vector to allow int calculations
    vector<float> floatOperands; // Temporary vector to allow float calculations
    vector<string> finalResult; // Final return vector for evaluation
    
    for (vector<string>::iterator c = postfix.begin(); c!=postfix.end(); ++c) {
        char s = std::stoi(*c);
        if (isdigit(s)) // Checks if digit
        {
            if (isFloat) // Checks if float
            {
                float floatOperand;
                stringstream ss;
                ss << s;
                ss >> floatOperand;
                floatOperands.push_back(floatOperand);
            }
            else // Else integer
            {
                int intOperand = s- '0';
                intOperands.push_back(intOperand);
            }
        } 

        else if (s== '+' || s== '-' || s== '*' || s== '/') // Checks if operator
        {
            if (isFloat) // Checks if float
            {
                float floatOperand2 = floatOperands.front();
                floatOperands.erase(floatOperands.begin());
                float floatOperand1 = floatOperands.front();
                floatOperands.erase(floatOperands.begin());

                float result;
                switch (s)
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

                floatOperands.push_back(result); // Push the result back to the stack
            }
            else
            {
                int intOperand2 = intOperands.front();
                intOperands.erase(intOperands.begin());
                int intOperand1 = intOperands.front();
                intOperands.erase(intOperands.begin());
                int result;
                switch (s)
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
                intOperands.push_back(result); // Push the result back to the vector
            }
        }
    }
    
    if (!intOperands.empty())
    {
        if (isFloat) // Checks if float
        {
            return floatOperands.front(); // Return the final result as a string
        }
        else 
        {

            return intOperands.front(); // Return the final result as a string
        }
    }
    else
    {
        // Handle error when no result is available
        // For example, when the postfix expression is invalid
        throw runtime_error("Invalid postfix expression");
    }
}

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


    // stack<int> intOperands;
    // stack<float> floatOperands;

    // for (char c : postfix)
    // {
    //     if (isdigit(c))
    //     {
    //         // Convert char digit to integer or float and push onto respective stacks
    //         if (isFloat)
    //         {
    //             float floatOperand;
    //             stringstream ss;
    //             ss << c;
    //             ss >> floatOperand;
    //             floatOperands.push_back(floatOperand);
    //             cout << floatOperand.begin();
    //         }
    //         else
    //         {
    //             int intOperand = c - '0';
    //             intOperands.push_back(intOperand);
    //         }
    //     }
    //     else if (c == '+' || c == '-' || c == '*' || c == '/')
    //     {
    //         // Perform arithmetic operation when an operator is encountered
    //         if (isFloat)
    //         {
    //             float floatOperand2 = floatOperands.top();
    //             floatOperands.pop();
    //             float floatOperand1 = floatOperands.top();
    //             floatOperands.pop();

    //             float result;
    //             switch (c)
    //             {
    //             case '+':
    //                 result = floatOperand1 + floatOperand2;
    //                 break;
    //             case '-':
    //                 result = floatOperand1 - floatOperand2;
    //                 break;
    //             case '*':
    //                 result = floatOperand1 * floatOperand2;
    //                 break;
    //             case '/':
    //                 result = floatOperand1 / floatOperand2;
    //                 break;
    //             }

    //             floatOperands.push(result); // Push the result back to the stack
    //         }
    //         else
    //         {
    //             int intOperand2 = intOperands.top();
    //             intOperands.pop();
    //             int intOperand1 = intOperands.top();
    //             intOperands.pop();

    //             int result;
    //             switch (c)
    //             {
    //             case '+':
    //                 result = intOperand1 + intOperand2;
    //                 break;
    //             case '-':
    //                 result = intOperand1 - intOperand2;
    //                 break;
    //             case '*':
    //                 result = intOperand1 * intOperand2;
    //                 break;
    //             case '/':
    //                 result = intOperand1 / intOperand2;
    //                 break;
    //             }

    //             intOperands.push(result); // Push the result back to the stack
    //         }
    //     }
    // }

    // if (isFloat && !floatOperands.empty())
    // {
    //     return floatOperands.top(); // Return the final float result
    // }
    // else if (!intOperands.empty())
    // {
    //     return intOperands.top(); // Return the final int result
    // }
    // else
    // {
    //     // Handle error when no result is available
    //     // For example, when the postfix expression is invalid
    //     throw runtime_error("Invalid postfix expression");
    // }

/*
    This function returns the precedence of an operator
    Input: char
    Output: int
*/
