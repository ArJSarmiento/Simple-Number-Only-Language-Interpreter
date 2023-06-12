#include <iostream>
#include <string>
#include "Utils.h"
#include "Usecase.h"

using namespace std;

void Usecase::BEG(string command)
{
    string var_name = command.erase(0, 4); // Remove "BEG "
    string store;

    cout << "SNOL> Please enter value for [" << var_name << "]: ";
    cin >> store;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (is_digit(store))
        storage->store_var(var_name, store);
    else
        cout << "SNOL> Error! Invalid value." << endl;
}

void Usecase::PRINT(string command)
{
    string temp = command.erase(0, 6); // Remove "PRINT  "

    // guard clause for digit PRINT
    if (is_digit(temp))
    {
        cout << "SNOL> [" << temp << "] = " << temp << endl;
        return;
    }
    // retrieve varible then print value
    string var_value = storage->get_var(temp);
    if (var_value == "")
    {
        cout << "SNOL> Error! [" << temp << "] is not defined!" << endl;
        return;
    }
    cout << "SNOL> [" << temp << "] = " << var_value << endl;
}

void Usecase::ASSIGN(string command)
{
    // remove spaces
    string var_name = strip_spaces(command.substr(0, command.find("=")));
    if (!is_variable(var_name))
    {
        cout << "SNOL> Error! [" << var_name << "] is not a valid variable name!" << endl;
        return;
    }

    // get expression an convert to postfix
    string expression = remove_space(command.substr(command.find("=") + 1));
    string result = CALCULATE(expression);
    if (result == "")
        return;

    storage->store_var(var_name, result);
}

string Usecase::CALCULATE(string expression)
{
    // get expression an convert to postfix
    vector<string> postfix = calculation->convert_infix_to_postfix(expression);
    if (postfix.size() == 0)
        return "";

    // evaluate postfix and store result
    string result = calculation->evaluate_postfix(postfix);
    return result;
}