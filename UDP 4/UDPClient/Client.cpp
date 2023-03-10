
#define WIN32_LEAN_AND_MEAN

#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>
#include <string>
using namespace std;

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 4096
//#define SERVER_IP "192.168.31.243"
//#define SERVER_IP "10.4.3.200"
//#define SERVER_IP "10.4.3.219"
#define SERVER_IP "192.168.31.243"
#define DEFAULT_PORT "8888"

SOCKET client_socket;
string name;
HANDLE h;
string sColor;


DWORD WINAPI Sender(void* param)
{
    while (true) {
        // cout << "Please insert your query for server: ";
        // ??????? ????????? ?????
        /*char query[DEFAULT_BUFLEN];
        cin.getline(query, DEFAULT_BUFLEN);
        send(client_socket, query, strlen(query), 0);*/

        // ?????????????? ??????? ????? ?????? ????????
         string query,message;
         getline(cin, query);
         message = sColor + name + query;
        
         send(client_socket, message.c_str(), message.size(), 0);
    }
}

DWORD WINAPI Receiver(void* param)
{
    while (true) {
        char response[DEFAULT_BUFLEN];
        int result = recv(client_socket, response, DEFAULT_BUFLEN, 0);
        response[result] = '\0';
        
        switch (response[0])
        {
            case '1':
                SetConsoleTextAttribute(h, 1);
                break;
            case '2':
                SetConsoleTextAttribute(h, 2);
                break;
            case '3':
                SetConsoleTextAttribute(h, 4);
                break;
            case '4':
                SetConsoleTextAttribute(h, 5);
                break;
            case '5':
                SetConsoleTextAttribute(h, 6);
                break;
            case '6':
                SetConsoleTextAttribute(h, 8);
                break;
            default:
                break;
        }
        
        // cout << "...\nYou have new response from server: " << response << "\n";
        cout << &(response[1]) << "\n";
        //cout << response << "\n";
        // cout << "Please insert your query for server: ";
        SetConsoleTextAttribute(h, 7);
    }
}

BOOL ExitHandler(DWORD whatHappening)
{
    switch (whatHappening)
    {
    case CTRL_C_EVENT: // closing console by ctrl + c
    case CTRL_BREAK_EVENT: // ctrl + break
    case CTRL_CLOSE_EVENT: // closing the console window by X button
      return(TRUE);
        break;
    default:
        return FALSE;
    }
}

int main()
{
    // ?????????? ???????? ???? ???????
    //SetConsoleCtrlHandler((PHANDLER_ROUTINE)ExitHandler, true);

    system("title Client");
    h = GetStdHandle(STD_OUTPUT_HANDLE);

    // initialize Winsock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }
    addrinfo hints = {};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // ????????? ????? ??????? ? ????
    addrinfo* result = nullptr;
    iResult = getaddrinfo(SERVER_IP, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 2;
    }

    addrinfo* ptr = nullptr;
    // ???????? ???????????? ? ??????, ???? ?? ???????
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        // ??????? ????? ?? ??????? ??????? ??? ??????????? ? ???????
        client_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

        if (client_socket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 3;
        }

        // connect to server
        iResult = connect(client_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(client_socket);
            client_socket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    CreateThread(0, 0, Receiver, 0, 0, 0);

    freeaddrinfo(result);

    if (client_socket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 5;
    }

    Sleep(1000);
    // ?????? ????
    cout << "What your name?\n";
     
    getline(cin, name);
    send(client_socket, name.c_str(), name.size(), 0);
    name += ": ";


    // ?????
    cout << "Color text:\n 1.Blue\n2.Green\n3.Red\n4.Purple\n5.Yellow\n6.Gray\n";
    getline(cin, sColor);

    //system("cls");

    CreateThread(0, 0, Sender, 0, 0, 0);

    Sleep(INFINITE);
}