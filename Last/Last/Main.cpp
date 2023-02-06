#include <iostream>
#include "ArrayValueCalculator.h"
#include "Exception.h"

using namespace std;

int main()
{
	ArrayValueCalculator calc;
	int array[4][4] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
	int array2[3][4] = { 1,2,3,4,5,6,7,8,9,10,11,12};


	string arrayS[4][4] = { "1A", "2", "3","4","5","6","7","8","9","10","11","12","13","14","15","16" };

	try
	{
		cout << calc.doCalc(array, 4) << endl;
		cout << calc.doCalc(arrayS, 4) << endl;
	}
	catch (ArrayDataException ex)
	{
		cout << ex.GetException() << endl;
	}
}