#include <iostream>
#include <string>
#include "Storage.h"
#include "Usecase.h"
#include "Calculation.h"

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
        storage.store_var(var_name, store);
    else
        cout << "ERROR" << endl;
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
    //  guard clause for non-existance variable
    if (!storage.var_exists(temp))
    {
        cout << "SNOL> Error! [" << temp << "] is not defined!" << endl;
        return;
    }

    // retrieve varible then print value
    string var_value = storage.get_var(temp);
    cout << "SNOL> [" << temp << "] = " << var_value << endl;
}

void Usecase::ASSIGN(string command)
{
    // remove spaces
    string normalized_command = remove_space(command);
    string var_name = normalized_command.substr(0, normalized_command.find("="));

    // check if variable exists and initialize variable if not
    if (!storage.var_exists(var_name))
    {
        storage.store_var(var_name, "0");
    }

    // get expression an convert to postfix
    string expression = normalized_command.substr(normalized_command.find("=") + 1);
    vector<string> postfix = calculation.convert_infix_to_postfix(expression);
    if (postfix.size() == 0)
        return;

    // evaluate postfix and store result
    string result = calculation.evaluate_postfix(postfix);
    if (result == "")
        return;

    storage.store_var(var_name, result);
}
