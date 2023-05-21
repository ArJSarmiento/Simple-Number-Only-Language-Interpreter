#pragma once
#include <iostream>
#include <string>
#include "Storage.h"

class Usecase
{
	Storage storage;

public:
	void BEG(string command);
	void PRINT(string command);
	void ASSIGN(string command);
};
