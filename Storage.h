#pragma once
#include <string>
#include <unordered_map>
#include <limits>
#include "Utils.h"

using namespace std;

class Storage
{
	unordered_map<string, string> variables;

public:
	bool var_exists(string command);
	void store_var(string var_name, string expression);
	string get_var(string command);
};
