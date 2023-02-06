#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
#pragma comment(lib, "urlmon.lib")
using namespace std;

/*
[
{
"r030":36,"txt":"Австралійський долар","rate":24.6198,"cc":"AUD","exchangedate":"23.12.2022"
 }
,{
"r030":124,"txt":"Канадський долар","rate":26.8482,"cc":"CAD","exchangedate":"23.12.2022"
 }
 ]
*/
int main()
{

	string srcURL = "https://bank.gov.ua/NBUStatService/v1/statdirectory/exchange?json";
	// the destination file 
	string destFile = "BankGov.txt";
	//// URLDownloadToFile returns S_OK on success 
	if (S_OK == URLDownloadToFileA(NULL, srcURL.c_str(), destFile.c_str(), 0, NULL))
	{
		cout << "Saved to " << destFile << "\n";
	}
	else cout << "Fail!\n";

	wstring response,data,temp;
	wstring currency, ListCurrency[65];
	int posS,i;
	wifstream Course("BankGov.txt");
	if (Course.is_open()) // -> открыт 
		do
		{
			Course >> temp;
			response += temp;
		} while (!Course.eof());
	else
		cout << "Cannot open file!" << endl;
	i = posS = 0;
	// заполнение доступными валютами
	while (posS<=response.size())
	{
		posS = response.find(L"\"cc\":",posS+6);
		for (int j = posS+6; response[j] != L'"'; j++)
		{
			ListCurrency[i] += response[j];
		}
		i++;
	}
	cout << "Enter the required currency. Exemple PLN USD EUR. Enter \"list\" to show list of available currencies" << endl;


	while (true)
	{
		wcin >> currency;
		if (currency == L"end")
			break;
		else if (currency == L"list")
		{
			// показ доступных валют
			for (int i = 0; i < 62; i++)
			{
				wcout << ListCurrency[i] << L"\t";
				if (i % 10 == 0)
				{
					wcout << L"\n";
				}
			}
			cout << "\nEnter the required currency or \"end\" to finish.\n" << endl;
		}
		else
		{
			data = L"EUR rate: ";
			posS = response.rfind(currency);
			if (posS >= 0)
			{
				posS = response.rfind(L"\"rate\":", posS);
				for (int i = posS + 7; response[i] != L','; i++) // 9- кол-во символов в <country>
					data += response[i];
				data += L"\n";

				wcout << data << endl;
			}
			else
				cout << "Now find!" << endl;
			cout << "Enter the required currency or \"end\" to finish." << endl;
		}
	}
		
	
	Course.close();

}