#pragma once
#include <iostream>
#include <string>
#include "Storage.h"
#include "Calculation.h"

class Usecase
{
	Storage storage;
	Calculation calculation;

public:
	void BEG(string command);
	void PRINT(string command);
	void ASSIGN(string command);
};
