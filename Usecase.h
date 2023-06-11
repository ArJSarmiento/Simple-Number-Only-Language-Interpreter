#pragma once
#include <iostream>
#include <string>
#include "Storage.h"
#include "Calculation.h"

class Usecase
{
	private:
		Storage *storage;
		Calculation *calculation;

	public:
		Usecase(Storage *storage, Calculation *calculation) : storage(storage), calculation(calculation) {}

		void BEG(string command);
		void PRINT(string command);
		void ASSIGN(string command);
		string CALCULATE(string command);
};
