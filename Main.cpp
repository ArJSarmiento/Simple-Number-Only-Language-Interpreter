#include <iostream>
#include <string>
#include "Utils.h"
#include "Usecase.h"
#include "Calculation.h"

using namespace std;

/*
	Program Description: Simple Number-Only Language (SNOL).
	SNOL is a simplified custom language that only involves integer and real values, operations, and expressions.
	It consists of four modules: Main, Calculation, Utilities, and Storage
	Main: Serves as the main part of the program where the I/O part is done
	Calculation (Calculation.cpp) Functions that calculate the value of an expression
	Utilities (Utils.cpp) : Consists of utility functions where important parts involving syntax and grammar checking is done
	Storage (Storage.cpp) : Class file that handles with variable storage and manipulation

	Commands:
		0 = Command unrecognized or invalid syntax
		1 = BEG command, for storing a value to a variable
		2 = PRINT command, for displaying the value of a variable
		3 = EXIT! command, for exiting the program
		4 = Calculation is done without storage, but syntax checking is important
		5 = Assignment is done to a variable
*/

int main()
{
	string command;	  // command stores the user input
	int type = 0;	  // type stores the command type of the input
	Usecase use_case; // Object that performs variable
	cout << "The SNOL environment is now active, you may proceed with giving your commands." << endl;
	while (1)
	{
		cout << "\nCommand: ";
		getline(cin, command);
		type = analyze_command(command); // Checks and returns the command type of the command
		switch (type)
		{
		case 0:
			/*
				analyze_command did not understand the command
			*/
			cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl;
			break;
		case 1:
			// BEG Command
			if (check_syntax(command, type))
				use_case.BEG(command);
			break;
		case 2:
			// PRINT Command
			if (check_syntax(command, type))
				use_case.PRINT(command);
			break;
		case 3:
			cout << "\nInterpreter is now terminated..." << endl;
			return 0;
			break;
		case 4:
			if (!check_syntax(command, type))
				cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl;
			break;
		case 5:
			// Assignment
			if (check_syntax(command, type))
				use_case.ASSIGN(command);
			break;
		}
	}
}
