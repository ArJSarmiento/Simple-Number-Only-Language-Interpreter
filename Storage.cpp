#include <string>
#include <unordered_map>
#include "Storage.h"

using namespace std;

bool Storage::var_exists(string var_name)
{
	return variables.count(var_name) > 0;
}

void Storage::store_var(string var_name, string expression)
{
	variables[var_name] = expression;
}

string Storage::get_var(string var_name)
{
	if (var_exists(var_name))
	{
		return variables[var_name];
	}
	return "";
}
