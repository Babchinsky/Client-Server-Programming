#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
#pragma comment(lib, "urlmon.lib")
using namespace std;

int main()
{
	//setlocale(0, "ru");
	//SetConsoleOutputCP(1251);
	//SetConsoleCP(1251);

	// the URL to download from 
	//string srcURL = "https://bank.gov.ua/NBUStatService/v1/statdirectory/exchange?json";

	string srcURL = "https://api.privatbank.ua/p24api/pubinfo?json&exchange&coursid=5";
	// the destination file 
	string destFile = "Course.txt";
	//// URLDownloadToFile returns S_OK on success 
	if (S_OK == URLDownloadToFileA(NULL, srcURL.c_str(), destFile.c_str(), 0, NULL))
	{
		cout << "Saved to " << destFile << "\n";
	}
	else cout << "Fail!\n";

	wstring response,data;
	int posS;
	wifstream Course("Course.txt");
	if (Course.is_open()) // -> открыт 
		Course >> response;
	else
		cout << "Cannot open file!" << endl;
	//wcout << response << endl;

	//	[{"ccy":"EUR","base_ccy":"UAH","buy":"41.15000","sale":"42.15000"},
	//	{"ccy":"USD","base_ccy":"UAH","buy":"39.60000","sale":"40.10000"}]
	// EUR
	{
		data = L"EUR: buy: ";
		posS = response.find(L"EUR");
		posS = response.find(L"buy",posS);
		for (int i = posS + 6; response[i] != L'"'; i++) // 9- кол-во символов в <country>
			data += response[i];
		posS = response.find(L"sale", posS);
		data += L"\t sale: ";
		for (int i = posS + 7; response[i] != L'"'; i++) // 9- кол-во символов в <country>
			data += response[i];
		data += L"\n";
	}
	// USD
	{
		data += L"USD: buy: ";
		posS = response.find(L"USD");
		posS = response.find(L"buy", posS);
		for (int i = posS + 6; response[i] != L'"'; i++) // 9- кол-во символов в <country>
			data += response[i];
		posS = response.find(L"sale", posS);
		data += L"\t sale: ";
		for (int i = posS + 7; response[i] != L'"'; i++) // 9- кол-во символов в <country>
			data += response[i];
		data += L"\n";
	}
	wcout << data << endl;

	Course.close();

}