#include <string>
#include <unordered_map>
#include <limits>
#include "Utils.h"
#include "Storage.h"

using namespace std;

bool Storage::var_exists(string var_name)
{
	try
	{
		variables.at(var_name);
	}
	catch (const std::exception &e)
	{
		return false;
	}

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
