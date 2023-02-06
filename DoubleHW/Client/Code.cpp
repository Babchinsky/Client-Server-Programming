#include <ws2tcpip.h> 
#include <windows.h>
#include <tchar.h>
#include <string>
#include "winsock2.h" 
#include "resource.h"
#pragma comment(lib, "ws2_32.lib") 
using namespace std;

void InitSocket(void);
void ConnectToServer(const char* ConnectToIP, short PORT);
bool SendData(char* BUFF);
void SendDataSMS(void);
bool ReceiveData(char* BUFF, short LocalSIZE);

HWND hDialog, hLabel1, hIP, hBCONNECT, hBUNCONNECT, hSI, hBSEND, hLabel2, hGI;
WSADATA WSAdata;
SOCKET Socket, AcceptSocket;
sockaddr_in SockAddr;
const short PORT = 8888;
const short GlobalSIZE = 256;
char EnterIP[GlobalSIZE] = {};
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
			hLabel1 = GetDlgItem(hWnd, LABEL1);
			hIP = GetDlgItem(hWnd, IP);
			hBCONNECT = GetDlgItem(hWnd, bCONNECT);
			hBUNCONNECT = GetDlgItem(hWnd, bUNCONNECT);
			hSI = GetDlgItem(hWnd, SEND_INFO);
			hBSEND = GetDlgItem(hWnd, bSEND);
			hLabel2 = GetDlgItem(hWnd, LABEL2);
			hGI = GetDlgItem(hWnd, GET_INFO);
			return(TRUE);
		}
		case WM_COMMAND:
		{
			{
				EnableWindow(hBSEND, FALSE);
				EnableWindow(hBCONNECT, FALSE);
				while (true)
				{
					ReceiveData(ReceiveSMS, GlobalSIZE);
					SetWindowText(hGI, LPWSTR(ReceiveSMS));
					Sleep(15000);
					SetWindowText(hGI, NULL);
					Sleep(15000);
				}
			}
			if (LOWORD(wp) == IP)
			{
				GetWindowText(hIP, (LPWSTR)EnterIP, GlobalSIZE);
				if (strlen(EnterIP) > NULL)
				{
					EnableWindow(hBCONNECT, TRUE);
				}
				else EnableWindow(hBCONNECT, FALSE);
			}
			if (LOWORD(wp) == bCONNECT)
			{
				EnableWindow(hBCONNECT, FALSE);
				EnableWindow(hBUNCONNECT, TRUE);
				InitSocket();
				ConnectToServer(EnterIP, PORT);
				EnableWindow(hIP, FALSE);
			}
			if (LOWORD(wp) == bUNCONNECT)
			{
				EnableWindow(hBUNCONNECT, FALSE);
				EnableWindow(hBCONNECT, TRUE);
				EnableWindow(hBSEND, FALSE);
				EnableWindow(hIP, TRUE);
				SetWindowText(hIP, NULL);
				SetWindowText(hSI, NULL);
				SetWindowText(hGI, NULL);
				closesocket(AcceptSocket);
				closesocket(Socket);
				WSACleanup();
			}
			if (LOWORD(wp) == SEND_INFO)
			{
				GetWindowText(hSI, (LPWSTR)SendSMS, GlobalSIZE);
				if (strlen(SendSMS) > NULL && IsWindowEnabled(hBCONNECT) == FALSE)
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
void ConnectToServer(const char* ConnectToIP, short PORT)
{
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_port = htons(PORT);
	inet_pton(AF_INET, ConnectToIP, &SockAddr.sin_addr);
	if (connect(Socket, (SOCKADDR*)&SockAddr, sizeof(SockAddr)) == SOCKET_ERROR)
	{
		MessageBox(NULL, TEXT("Error in ConncectToServer func"), TEXT("Information"), MB_OK | MB_ICONERROR);
		WSACleanup();
		EndDialog(NULL, NULL);
	}
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