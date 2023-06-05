#include <iostream>
#include <stack>
#include <string>
#include <algorithm>
#include <regex>
#include "Utils.h"

using namespace std;

/*
	This function removes all spaces in a string
	Input: string
	Output: string
*/
string remove_space(string str)
{
	str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());
	return str;
}

bool is_operator(char c)
{ // Check if current character is a mathematical operator
	switch (c)
	{
	case '+':
	case '-':
	case '*':
	case '/':
	case '%':
		return true;
	}
	return false;
}

bool is_variable(string c)
{												 // Check if the string is in variable name syntax
	regex var("\\(*-?[A-Za-z][A-Za-z0-9]*\\)*"); // letter{letter|digit} -> in IBNF
	if (regex_match(c, var))
		return true;
	else
		return false;
}

bool is_digit(string c)
{													  // Check if string is in digit syntax
	regex digit("\\(*-?[0-9][0-9]*(\\.[0-9]+)?\\)*"); // [-]digit{digit}[.{digit}] -> in IBNF
	if (regex_match(c, digit))
		return true;
	else
		return false;
}

int analyze_command(string command)
{
	regex beg("BEG [^ ]+");	   // "BEG (any characters)" to be a BEG command
	regex disp("PRINT [^ ]+"); // "PRINT (any characters)" to be a PRINT command
	if (regex_match(command, beg))
		return 1;
	else if (regex_match(command, disp))
		return 2;
	else if (command == "EXIT!")
		return 3;
	for (int i = 0; i < command.length(); i++)
	{
		if (is_operator(command[i]))
			return 4; // If the string contains an operator first, then it is a calculation
		if (command[i] == '=')
			return 5; // If the string contains an equals sign first, then it is an assignment
	}
	return 0; // INVALID COMMAND
}

bool check_syntax(string command, int type)
{
	regex var("\\(*-?[A-Za-z][A-Za-z0-9]*\\)*");	  // letter{letter|digit} -> in IBNF
	regex digit("\\(*-?[0-9][0-9]*(\\.[0-9]+)?\\)*"); // [-]digit{digit}[.{digit}] -> in IBNF
	string temp;
	int parenthesis = 0;
	if (type == 1)
	{ // BEG command syntax check
		temp = command.erase(0, 4);
		if (regex_match(temp, var))
			return true;
		else
		{
			// DISPLAY APPROPRIATE ERROR COMMAND PLEASEEEE
			cout << "ERROR" << endl;
			return false;
		}
	}
	else if (type == 2)
	{ // PRINT command syntax check
		temp = command.erase(0, 6);
		if (regex_match(temp, var) || regex_match(temp, digit))
			return true;
		else
		{
			// DISPLAY APPROPRIATE ERROR COMMAND PLEASEEEE
			cout << "ERROR" << endl;
			return false;
		}
	}
	else if (type == 4)
	{ // Check Calculation if it fits the expression syntax
		for (int i = 0; i < command.length(); i++)
		{
			if (parenthesis < 0)
			{
				// DISPLAY APPROPRIATE ERROR COMMAND PLEASEEEE
				cout << " Parenthesis ERROR" << endl;
				return false;
			}
			else if (command[i] == '(')
			{
				parenthesis++;
				temp += command[i];
			}
			else if (command[i] == ')')
			{
				parenthesis--;
				temp += command[i];
			}
			else if (is_operator(command[i]))
			{ // Serves as flag to check the temp
				if (temp.size() == 0)
				{ // No valid characters for part
					// DISPLAY APPROPRIATE ERROR COMMAND PLEASEEEE
					cout << "ERROR" << endl;
					return false;
				}
				if (!(regex_match(temp, var) || regex_match(temp, digit)))
				{ // Not in variable or digit syntax
					// DISPLAY APPROPRIATE ERROR COMMAND PLEASEEEE
					cout << "ERROR" << endl;
					return false;
				}
				temp.erase();
			}
			else if (command[i] == ' ')
				continue; // Ignores spaces
			else
				temp += command[i]; // Add the next character to the part to be checked
		}

		if (parenthesis != 0)
		{
			// DISPLAY APPROPRIATE ERROR COMMAND PLEASEEEE
			cout << "Parenthesis ERROR" << endl;
			return false;
		}

		// Same as above, but for the last part before end of string
		if (temp.size() == 0)
		{ // No valid characters for part
			// DISPLAY APPROPRIATE ERROR COMMAND PLEASEEEE
			cout << "ERROR" << endl;
			return false;
		}
		if (!(regex_match(temp, var) || regex_match(temp, digit)))
		{ // Not in variable or digit syntax
			// DISPLAY APPROPRIATE ERROR COMMAND PLEASEEEE
			cout << "ERROR" << endl;
			return false;
		}
		temp.erase();
		return true;
	}
	else if (type == 5)
	{ // Check Assignment if it fits the expression syntax
		for (int i = 0, equals = 0; i < command.size(); i++)
		{
			if (parenthesis < 0)
			{
				// DISPLAY APPROPRIATE ERROR COMMAND PLEASEEEE
				cout << "ERROR" << endl;
				return false;
			}
			else if (command[i] == '(')
			{
				parenthesis++;
				temp += command[i];
			}
			else if (command[i] == ')')
			{
				parenthesis--;
				temp += command[i];
			}
			else if (command[i] == '=')
			{ // Flag to check assigned variable syntax
				equals++;
				if (equals > 1)
				{ // Error if more than 1 equals sign
					// DISPLAY APPROPRIATE ERROR COMMAND PLEASEEEE
					cout << "ERROR" << endl;
					return false;
				}
				if (!regex_match(temp, var))
				{ // Error if not in variable name syntax
					// DISPLAY APPROPRIATE ERROR COMMAND PLEASEEEE
					cout << "ERROR" << endl;
					return false;
				}
				temp.erase();
			}
			else if (is_operator(command[i]))
			{ // Flag to check part of the expression for syntax
				if (temp.size() == 0 || equals == 0)
				{ // No part captured, meaning repeating operator error or no equals sign found
					// DISPLAY APPROPRIATE ERROR COMMAND PLEASEEEE
					cout << "ERROR" << endl;
					return false;
				}
				if (!(regex_match(temp, var) || regex_match(temp, digit)))
				{ // Not in variable or digit syntax
					// DISPLAY APPROPRIATE ERROR COMMAND PLEASEEEE
					cout << "ERROR" << endl;
					return false;
				}
				temp.erase();
			}
			else if (command[i] == ' ')
				continue; // Ignores spaces
			else
				temp += command[i];
		}

		if (parenthesis != 0)
		{
			// DISPLAY APPROPRIATE ERROR COMMAND PLEASEEEE
			cout << "ERROR" << endl;
			return false;
		}

		// Same as above, but for the last part before end of string
		if (temp.size() == 0)
		{ // No valid characters for part
			// DISPLAY APPROPRIATE ERROR COMMAND PLEASEEEE
			cout << "ERROR" << endl;
			return false;
		}
		if (!(regex_match(temp, var) || regex_match(temp, digit)))
		{ // Not in variable or digit syntax
			// DISPLAY APPROPRIATE ERROR COMMAND PLEASEEEE
			cout << "ERROR" << endl;
			return false;
		}
		temp.erase();
		return true;
	}
	else
		return false;
}