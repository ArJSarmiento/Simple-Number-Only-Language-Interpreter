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
        cout << "SNOL> Error! Expression is empty." << endl;
        return {};
    }
    if (is_operator(infix[0]))
    {
        cout << "SNOL> Error! Expression cannot start with an operator." << endl;
        return {};
    }

    // Iterate through each character in the infix expression
    for (int i = 0; i < infixStringLength; i++)
    {
        if (infix[i] == '.')
        {
            if (parsingVariable)
            {
                cout << "SNOL> Error! Dot should not be in a variable." << endl;
                return {};
            }
            if (!parsingNumber)
            {
                cout << "SNOL> Error! Variables should not start with a dot." << endl;
                return {};
            }
            if (hasDot)
            {
                cout << "SNOL> Error! Number has more than one dot." << endl;
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
                cout << "SNOL> Error! Variables should not start with a number." << endl;
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
                    cout << "SNOL> Error! Number has a dot at the end." << endl;
                    return {};
                }
                if (hasDot != isFloat)
                {
                    cout << "SNOL> Error! Operands must be of the same type in an arithmetic operation!" << endl;
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
                    cout << "SNOL> Error!" << endl;
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
                    cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl;
                    return {};
                }

                operators.push(infix[i]); // Push '(' onto the stack
            }
            else if (infix[i] == ')')
            {
                if (!is_operator(infix[i + 1]) && infix[i + 1] != '\0')
                {
                    cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl;
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
                    cout << "SNOL> Error! Mismatched parentheses in infix expression" << endl;
                    return {};
                }
            }
            else
            {
                // Invalid character detected, return "Error!"
                cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl;
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
            cout << "SNOL> Error! Mismatched parentheses in infix expression" << endl;
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
string Calculation::evaluate_postfix(vector<string> postfix)
{
    stack<float> operands; // Stack to store operands
    Storage storage;

    for (int i = 0; i < postfix.size(); i++)
    {
        string token = postfix[i];
        if (is_digit(token))
        {
            float floatOperand = stof(token);
            operands.push(floatOperand);
        }
        else if (is_variable(token))
        {
            if (!storage.var_exists(token))
            {
                cerr << "SNOL> Error! [" << token << "] is not defined!" << endl;
                return "";
            }
            string var1 = storage.get_var(token);

            float floatOperand = stof(var1);
            operands.push(floatOperand);
        }
        else if (is_operator_string(token))
        {
            float floatOperand1 = operands.top();
            operands.pop();
            float floatOperand2 = operands.top();
            operands.pop();

            float result;
            char iter = token[0];
            switch (iter)
            {
            case '+':
                result = floatOperand1 + floatOperand2;
                break;
            case '-':
                result = floatOperand2 - floatOperand1;
                break;
            case '*':
                result = floatOperand1 * floatOperand2;
                break;
            case '/':
                result = floatOperand2 / floatOperand1;
                break;
            }

            operands.push(result);
        }
    }

    if (operands.empty())
    {
        return "";
    }

    return removeTrailingZeroes(operands.top());
}

string Calculation::removeTrailingZeroes(float num)
{
    string str = to_string(num);
    string whole_number = str.substr(0, str.find("."));
    string decimals = str.substr(str.find(".") + 1);
    string decimal_results = decimals;

    // iterates backwards and check for trailing zero
    for (int i = decimals.length() - 1; i >= 0; i--)
    {
        if (decimals[i] == '0')
        {
            decimal_results.pop_back();
        }
        else
        {
            break;
        }
    }

    // if the decimal results is empty, return the whole number
    if (decimal_results.empty())
    {
        return whole_number;
    }
    else
    {
        return whole_number + "." + decimal_results;
    }
}
