#include <string>
#include <unordered_map>
#include <limits>
#include "Utils.h"
#include "Storage.h"

using namespace std;

bool Storage::var_exists(string command)
{
	if (variables.find(command) == variables.end())
		return false; // Variable not assigned yet
	return true;
}

void Storage::store_var(string var_name, string expression)
{
	variables[var_name] = expression;
}

string Storage::get_var(string var_name)
{
	return variables.at(var_name);
}
