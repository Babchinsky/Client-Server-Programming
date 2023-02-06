#include <ws2tcpip.h> 
#include <windows.h>
#include <tchar.h>
#include <string>
#include "winsock2.h" 
#include "resource.h"
#pragma comment(lib, "ws2_32.lib") 
using namespace std;

void InitSocket(void);
void Bind(short PORT);
void Listen(void);
void StartHosting(short PORT);
bool SendData(char* BUFF);
void SendDataSMS(void);
bool ReceiveData(char* BUFF, short LocalSIZE);

HWND hDialog, hSI, hBSEND, hLabel, hGI, hBSTART, hBSTOP;
WSADATA WSAdata;
SOCKET Socket, AcceptSocket;
sockaddr_in SockAddr;
string MyIP = "192.168.56.1";
const short PORT = 8888;
const short GlobalSIZE = 256;
char SendSMS[GlobalSIZE] = {};
char ReceiveSMS[GlobalSIZE] = {};

BOOL CALLBACK DP(HWND hWnd, UINT sms, WPARAM wp, LPARAM lp);

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int CmdShow)
{
	return DialogBox(hInst, MAKEINTRESOURCE(DIALOG), NULL, (DLGPROC)DP);
}

BOOL CALLBACK DP(HWND hWnd, UINT sms, WPARAM wp, LPARAM lp)
{
	switch (sms)
	{
		case WM_INITDIALOG:
		{
			hDialog = GetDlgItem(hWnd, DIALOG);
			hSI = GetDlgItem(hWnd, SEND_INFO);
			hBSEND = GetDlgItem(hWnd, bSEND);
			hLabel = GetDlgItem(hWnd, LABEL);
			hGI = GetDlgItem(hWnd, GET_INFO);
			hBSTART = GetDlgItem(hWnd, bSTART);
			hBSTOP = GetDlgItem(hWnd, bSTOP);
			return(TRUE);
		}
		case WM_COMMAND:
		{
			{
				EnableWindow(hBSEND, FALSE);
				while (true)
				{
					ReceiveData(ReceiveSMS, GlobalSIZE);
					SetWindowText(hGI, LPWSTR(ReceiveSMS));
					Sleep(15000);
					SetWindowText(hGI, NULL);
					Sleep(15000);
				}
			}
			if (LOWORD(wp) == bSTART)
			{
				EnableWindow(hBSTART, FALSE);
				EnableWindow(hBSTOP, TRUE);
				InitSocket();
				StartHosting(PORT);
			}
			if (LOWORD(wp) == bSTOP)
			{
				EnableWindow(hBSTOP, FALSE);
				EnableWindow(hBSTART, TRUE);
				EnableWindow(hBSEND, FALSE);
				SetWindowText(hSI, NULL);
				SetWindowText(hGI, NULL);
				closesocket(AcceptSocket);
				closesocket(Socket);
				WSACleanup();
			}
			if (LOWORD(wp) == SEND_INFO)
			{
				GetWindowText(hSI, (LPWSTR)SendSMS, GlobalSIZE);
				if (strlen(SendSMS) > NULL && IsWindowEnabled(hBSTART) == FALSE)
				{
					EnableWindow(hBSEND, TRUE);
				}
				else EnableWindow(hBSEND, FALSE);
			}
			if (LOWORD(wp) == bSEND)
			{
				SendDataSMS();
				SetWindowText(hSI, NULL);
			}
			return(TRUE);
		}
		case WM_CLOSE:
		{
			closesocket(AcceptSocket);
			closesocket(Socket);
			WSACleanup();
			EndDialog(hWnd, NULL);
			return(TRUE);
		}
	}
	return(FALSE);
}

void InitSocket(void)
{
	if (WSAStartup(MAKEWORD(2, 2), &WSAdata) != NO_ERROR)
	{
		MessageBox(NULL, TEXT("Error in 1/2 InitSocket func"), TEXT("Information"), MB_OK | MB_ICONERROR);
		WSACleanup();
		EndDialog(NULL, NULL);
	}
	Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (Socket == INVALID_SOCKET)
	{
		MessageBox(NULL, TEXT("Error in 2/2 InitSocket func"), TEXT("Information"), MB_OK | MB_ICONERROR);
		WSACleanup();
		EndDialog(NULL, NULL);
	}
}
void Bind(short PORT)
{
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_port = htons(PORT);
	inet_pton(AF_INET, "0.0.0.0", &SockAddr.sin_addr);
	if (bind(Socket, (SOCKADDR*)&SockAddr, sizeof(SockAddr)) == SOCKET_ERROR)
	{
		MessageBox(NULL, TEXT("Error in Bind func"), TEXT("Information"), MB_OK | MB_ICONERROR);
		WSACleanup();
		EndDialog(NULL, NULL);
	}
}
void Listen(void)
{
	if (listen(Socket, 1) == SOCKET_ERROR)
	{
		MessageBox(NULL, TEXT("Error in Listen func"), TEXT("Information"), MB_OK | MB_ICONERROR);
		WSACleanup();
		EndDialog(NULL, NULL);
	}
	AcceptSocket = accept(Socket, NULL, NULL);
	while (AcceptSocket == SOCKET_ERROR)
	{
		AcceptSocket = accept(Socket, NULL, NULL);
	}
	Socket = AcceptSocket;
}
void StartHosting(short PORT)
{
	Bind(PORT);
	Listen();
}
bool SendData(char* BUFF)
{
	send(Socket, BUFF, strlen(BUFF), NULL);
	return(TRUE);
}
void SendDataSMS(void)
{
	SendData(SendSMS);
}
bool ReceiveData(char* BUFF, short LocalSIZE)
{
	LocalSIZE = GlobalSIZE;
	short i = recv(Socket, BUFF, LocalSIZE, NULL);
	BUFF[i] = '\0';
	return(true);
}