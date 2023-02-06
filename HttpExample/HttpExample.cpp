#pragma comment (lib, "Ws2_32.lib")
#include <Winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <string>
using namespace std;

int main()
{
    setlocale(0, "ru");

    //1. инициализация "Ws2_32.dll" для текущего процесса
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(2, 2);

    int err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {

        cout << "WSAStartup failed with error: " << err << endl;
        return 1;
    }  

    //инициализация структуры, для указания ip адреса и порта сервера с которым мы хотим соединиться
   
    char hostname[255] = "api.openweathermap.org";
    
    addrinfo* result = NULL;    
    
    addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    int iResult = getaddrinfo(hostname, "http", &hints, &result);
    if (iResult != 0) {
        cout << "getaddrinfo failed with error: " << iResult << endl;
        WSACleanup();
        return 3;
    }     

    SOCKET connectSocket = INVALID_SOCKET;
    addrinfo* ptr = NULL;

    //Пробуем присоединиться к полученному адресу
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        //2. создание клиентского сокета
        connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (connectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

       //3. Соединяемся с сервером
        iResult = connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(connectSocket);
            connectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    //4. HTTP Request

    string uri = "/data/2.5/weather?q=Odessa&appid=75f6e64d49db78658d09cb5ab201e483&mode=JSON";

    string request = "GET " + uri + " HTTP/1.1\n"; 
    request += "Host: " + string(hostname) + "\n";
    request += "Accept: */*\n";
    request += "Accept-Encoding: gzip, deflate, br\n";   
    request += "Connection: close\n";   
    request += "\n";

    //отправка сообщения
    if (send(connectSocket, request.c_str(), request.length(), 0) == SOCKET_ERROR) {
        cout << "send failed: " << WSAGetLastError() << endl;
        closesocket(connectSocket);
        WSACleanup();
        return 5;
    }
    cout << "send data" << endl;

    //5. HTTP Response

    string response;

    const size_t BUFFERSIZE = 1024;
    char resBuf[BUFFERSIZE];

    int respLength;

    do {
        respLength = recv(connectSocket, resBuf, BUFFERSIZE, 0);
        if (respLength > 0) {
            response += string(resBuf).substr(0, respLength);           
        }
        else {
            cout << "recv failed: " << WSAGetLastError() << endl;
            closesocket(connectSocket);
            WSACleanup();
            return 6;
        }

    } while (respLength == BUFFERSIZE);

    /*cout << response << endl;*/

    string szCountry, szCityName, szTimezone, szID, szLat, szLon, szTemp, szTempMin, szTempMax, szSunrise, szSunset;
    int iPos;

    // Country
    {
        szCountry = "Country: ";
        iPos = response.find("country");
        for (int i = iPos + 10; response[i] != '"'; i++)
            szCountry += response[i];
        szCountry += '\n';

        cout << szCountry;
    }

    // City
    {
        szCityName = "City: ";
        iPos = response.find("name");
        for (int i = iPos + 7; response[i] != '"'; i++)
            szCityName += response[i];
        szCityName += '\n';

        cout << szCityName;
    }

       // Timezone
    {
        szTimezone = "Timezone: ";
        iPos = response.find("timezone");
        for (int i = iPos + 10; response[i] != ','; i++)
            szTimezone += response[i];
        szTimezone += '\n';

        cout << szTimezone;
    }

     // ID
    {
        szID = "ID: ";
        iPos = response.find("id", iPos);
        for (int i = iPos + 4; response[i] != ','; i++)
            szID += response[i];
        szID += '\n';

        cout << szID;
    }

    // X
    {
        szLon = "Coord: X: ";
        iPos = response.find("lon");
        for (int i = iPos + 5; response[i] != ','; i++)
            szLon += response[i];
        szLon += '\n';

        cout << szLon;
    }

        // Y
    {
        szLat = "Coord: Y: ";
        iPos = response.find("lat");
        for (int i = iPos + 5; response[i] != '}'; i++)
            szLat += response[i];
        szLat += '\n';

        cout << szLat;
    }

    char buf[8];
    string temp;
    double a;


         // Temperature Min
    {
        szTempMin = "Temperature MIN: ";
        iPos = response.find("temp_min");
        for (int i = iPos; response[i] != ','; i++)
            temp += response[i];

        a = atof(temp.c_str());
        a -= 273.15;
        sprintf_s(buf, "%0.2f", a);

        szTempMin += buf;
        szTempMin += '\n';
        cout << szTempMin;
    }

    // Temperature Max
    {
        szTempMax = "Temperature MAX: ";
        iPos = response.find("temp_max");
        for (int i = iPos; response[i] != ','; i++)
            temp += response[i];

        a = atof(temp.c_str());
        a -= 273.15;
        sprintf_s(buf, "%0.2f", a);

        szTempMax += buf;
        szTempMax += '\n';
        cout << szTempMax;
    }





    //отключает отправку и получение сообщений сокетом
    iResult = shutdown(connectSocket, SD_BOTH);
    if (iResult == SOCKET_ERROR) {
        cout << "shutdown failed: " << WSAGetLastError() << endl;
        closesocket(connectSocket);
        WSACleanup();
        return 7;
    }

    closesocket(connectSocket);
    WSACleanup();
}