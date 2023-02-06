#pragma once
#include <string>
#include "Exception.h"

class ArrayValueCalculator
{
	int sum;


public:
	ArrayValueCalculator() : sum(0) {}

	int doCalc(const std::string array[4][4], size_t size)
	{
		if (size != 4)
		{
			throw ArrayDataException();
		}

		int sum = 0;
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				for (size_t k = 0; k < array[i][j].length(); k++)
				{
					if (array[i][j][k] < '0' || array[i][j][k] > '9')
					{
						std::string excpt = "Cannot convert array[" + std::to_string(i) + "][" + std::to_string(j) + "] = \"" + array[i][j] + "\"\n";
						throw ArrayDataException(excpt);
					}
				}
				sum = std::atoi(array[i][j].c_str());
			}
		}
		this->sum = sum;
		return sum;
	}

	template <class T>
	int doCalc(const T array[4][4], size_t size)
	{
		if (size != 4)
		{
			throw ArrayDataException();
		}

		int sum = 0;
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				try 
				{
					sum += int(array[i][j]);
				}
				catch (...)
				{
					std::string excpt = "Cannot convert array[" + std::to_string(i) + "][" + std::to_string(j) + "]\n";
					throw ArrayDataException(excpt);
				}
			}
		}
		this->sum = sum;
		return sum;
	}
};