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

void Calculation::error_reset(string message)
{
    cout << message << endl;
    isFloat = false;
    isAllVars = true;
}

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
        error_reset("SNOL> Error! Expression is empty.");
        return {};
    }
    if (is_operator(infix[0]) && infix[0] != '-')
    {
        error_reset("SNOL> Error! Expression cannot start with an operator.");
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
                error_reset("SNOL> Error! Dot should not be in a variable.");
                return {};
            }
            if (!parsingNumber)
            {
                error_reset("SNOL> Error! Variables should not start with a dot");
                return {};
            }
            if (hasDot)
            {
                error_reset("SNOL> Error! Number has more than one dot.");
                return {};
            }

            currentNumber += current_char;
            hasDot = true;
            isAllVars = false;
        }
        else if (isdigit(current_char) && !parsingVariable)
        {
            // If the character is a digit, append it to the currentNumber string
            currentNumber += current_char;
            parsingNumber = true;
            isAllVars = false;
        }
        else if (isalnum(current_char))
        {
            if ((isalpha(current_char) && parsingNumber) || hasDot)
            {
                error_reset("SNOL> Error! Variables should not start with a number.");
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
                    error_reset("SNOL> Error! Unknown command! Does not match any valid command of the language.");
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
                    error_reset("SNOL> Error! Number has a dot at the end.");
                    return {};
                }

                // If parsing a number, add the currentNumber as a token
                tokens.push_back(currentNumber);
                currentNumber.clear();

                if (hasDot != isFloat && !isFirstToken)
                {
                    error_reset("SNOL> Error! Operands must be of the same type in an arithmetic operation!");
                    return {};
                }
                if (hasDot && !isFloat)
                {
                    isFloat = true;
                }

                hasDot = false;
                parsingNumber = false;
                isFirstToken = false;
            }

            if (is_operator(current_char))
            {
                // handle negative
                if (current_char == '-' && (i == 0 || is_operator(infix[i - 1]) || infix[i - 1] == '('))
                {
                    tokens.push_back("-1");
                    tokens.push_back("*");
                    continue;
                }
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
                    error_reset("SNOL> Error! Mismatched parentheses in infix expression");
                    return {};
                }

                operators.pop(); // Pop '(' from the stack
            }
            else
            {
                // Invalid character detected, return "Error!"
                error_reset("SNOL> Error! Unknown command! Does not match any valid command of the language.");
                return {};
            }
        }
    }

    if (parsingNumber && !currentNumber.empty())
    {
        // check if currentNumber has Dot
        hasDot = currentNumber.find('.') != string::npos;

        if (currentNumber.back() == '.')
        {
            error_reset("SNOL> Error! Number has a dot at the end.");
            return {};
        }
        if (hasDot != isFloat && !isFirstToken)
        {
            error_reset("SNOL> Error! Operands must be of the same type in an arithmetic operation!");
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
            error_reset("SNOL> Error! Unknown command! Does not match any valid command of the language.");
            return {};
        }
        tokens.push_back(currentVariable);
    }

    while (!operators.empty())
    {
        if (operators.top() == '(' || operators.top() == ')')
        {
            error_reset("SNOL> Error! Mismatched parentheses in infix expression");
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
            string var_value = storage->get_var(token);

            if (var_value == "")
            {
                error_reset("SNOL> Error! [" + token + "] is not defined!");
                return "";
            }

            is_var_value_float = var_value.find('.') != string::npos;
            if ((is_var_value_float != isFloat && !isAllVars) || (isAllVars && i != 0 && is_var_value_float != isFloat))
            {
                error_reset("SNOL> Error! Operands must be of the same type in an arithmetic operation!");
                return "";
            }

            float floatOperand = stof(var_value);
            operands.push(floatOperand);

            if (is_var_value_float && !isFloat)
            {
                isFloat = true;
            }
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
                    error_reset("SNOL> Error! Division by zero!");
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
        isAllVars = true;
        return "";
    }

    isFloat = false;
    isAllVars = true;
    return remove_trailing_zeroes(operands.top());
}
