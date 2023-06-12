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
#include "Calculation.h"

using namespace std;

// Removes all trailing zeroes on a float
string Calculation::remove_trailing_zeroes(float num)
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
    return whole_number + "." + decimal_results;
}

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
    bool isFirstToken = true;     // Flag to indicate if the current token is the first token

    if (infix.empty())
    {
        cout << "SNOL> Error! Expression is empty." << endl;
        isFloat = false;
        return {};
    }
    if (is_operator(infix[0]))
    {
        cout << "SNOL> Error! Expression cannot start with an operator." << endl;
        isFloat = false;
        return {};
    }

    // Iterate through each character in the infix expression
    for (int i = 0; i < infixStringLength; i++)
    {
        char current_char = infix[i];
        // cout << current_char;
        if (current_char == '.')
        {
            if (parsingVariable)
            {
                cout << "SNOL> Error! Dot should not be in a variable." << endl;
                isFloat = false;
                return {};
            }
            if (!parsingNumber)
            {
                cout << "SNOL> Error! Variables should not start with a dot." << endl;
                isFloat = false;
                return {};
            }
            if (hasDot)
            {
                cout << "SNOL> Error! Number has more than one dot." << endl;
                isFloat = false;
                return {};
            }

            currentNumber += current_char;
            hasDot = true;
        }
        else if (isdigit(current_char) && !parsingVariable)
        {
            // If the character is a digit, append it to the currentNumber string
            currentNumber += current_char;
            parsingNumber = true;
        }
        else if (isalnum(current_char))
        {
            if ((isalpha(current_char) && parsingNumber) || hasDot)
            {
                cout << "SNOL> Error! Variables should not start with a number." << endl;
                isFloat = false;
                return {};
            }

            // If the character is a variable, append it to the currentVariable string
            currentVariable += current_char;
            parsingVariable = true;
        }
        else
        {
            if (parsingVariable)
            {
                // If parsing a variable, add the currentVariable as a token
                if (currentVariable.find("BEG") != string::npos || currentVariable.find("PRINT") != string::npos)
                {
                    cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl;
                    isFloat = false;
                    return {};
                }
                tokens.push_back(currentVariable);
                currentVariable.clear();
                parsingVariable = false;
            }
            else if (parsingNumber)
            {
                if (currentNumber.back() == '.')
                {
                    cout << "SNOL> Error! Number has a dot at the end." << endl;
                    isFloat = false;
                    return {};
                }

                // If parsing a number, add the currentNumber as a token
                tokens.push_back(currentNumber);
                currentNumber.clear();

                if (hasDot != isFloat && !isFirstToken)
                {
                    cout << "SNOL> Error! Operands must be of the same type in an arithmetic operation!" << endl;
                    isFloat = false;
                    return {};
                }
                if (hasDot && !isFloat)
                {
                    isFloat = true;
                }

                hasDot = false;
                parsingNumber = false;
            }

            if (is_operator(current_char))
            {
                // If the character is an operator (+, -, *, /)
                while (!operators.empty() && operators.top() != '(' && precedence(operators.top()) >= precedence(current_char))
                {
                    // Pop operators from the stack and add them as tokens until the top of the stack is '(' or ')'
                    tokens.push_back(string(1, operators.top()));
                    operators.pop();
                }
                operators.push(current_char); // Push the current operator onto the stack
            }
            else if (current_char == '(')
            {
                operators.push(current_char); // Push '(' onto the stack
            }
            else if (current_char == ')')
            {

                // If the character is ')', pop operators from the stack and add them as tokens until '(' is found
                while (!operators.empty() && operators.top() != '(')
                {
                    tokens.push_back(string(1, operators.top()));
                    operators.pop();
                }

                if (operators.empty())
                {
                    cout << "SNOL> Error! Mismatched parentheses in infix expression" << endl;
                    isFloat = false;
                    return {};
                }

                operators.pop(); // Pop '(' from the stack
            }
            else
            {
                // Invalid character detected, return "Error!"
                cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl;
                isFloat = false;
                return {};
            }

            isFirstToken = false;
        }
    }

    if (parsingNumber && !currentNumber.empty())
    {
        // check if currentNumber has Dot
        hasDot = currentNumber.find('.') != string::npos;

        if (currentNumber.back() == '.')
        {
            cout << "SNOL> Error! Number has a dot at the end." << endl;
            isFloat = false;
            return {};
        }
        if (hasDot != isFloat && !isFirstToken)
        {
            cout << "SNOL> Error! Operands must be of the same type in an arithmetic operation!" << endl;
            isFloat = false;
            return {};
        }

        // If parsing a number, add the currentNumber as a token
        tokens.push_back(currentNumber);
        currentNumber.clear();

        if (hasDot && !isFloat)
        {
            isFloat = true;
        }
        hasDot = false;
        parsingNumber = false;
    }
    if (parsingVariable && !currentVariable.empty())
    {
        // If parsing a variable at the end, add it as a token
        if (currentVariable.find("BEG") != string::npos || currentVariable.find("PRINT") != string::npos)
        {
            cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl;
            isFloat = false;
            return {};
        }
        tokens.push_back(currentVariable);
    }

    while (!operators.empty())
    {
        if (operators.top() == '(' || operators.top() == ')')
        {
            cout << "SNOL> Error! Mismatched parentheses in infix expression" << endl;
            isFloat = false;
            return {};
        }

        // Pop remaining operators from the stack and add them as tokens
        string o = string(1, operators.top());
        tokens.push_back(o);
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
    bool is_var_value_float = false;

    for (string token : postfix)
    {

        if (is_digit(token))
        {
            float floatOperand = stof(token);
            operands.push(floatOperand);
        }
        else if (is_variable(token))
        {
            string var_value = storage->get_var(token);

            if (var_value == "")
            {
                cout << "SNOL> Error! [" << token << "] is not defined!" << endl;
                isFloat = false;
                return "";
            }

            is_var_value_float = var_value.find('.') != string::npos;
            if (is_var_value_float != isFloat)
            {
                cout << "SNOL> Error! Operands must be of the same type in an arithmetic operation!" << endl;
                isFloat = false;
                return "";
            }

            float floatOperand = stof(var_value);
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
            {
                if (floatOperand1 == 0)
                {
                    cout << "SNOL> Error! Division by zero!" << endl;
                    isFloat = false;
                    return "";
                }
                result = floatOperand2 / floatOperand1;
                break;
            }
            }

            operands.push(result);
        }
    }

    if (operands.empty())
    {
        isFloat = false;
        return "";
    }

    isFloat = false;
    return remove_trailing_zeroes(operands.top());
}
