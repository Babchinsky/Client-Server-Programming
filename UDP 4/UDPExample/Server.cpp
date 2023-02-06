#include <winsock2.h>
#include <iostream>
#include <vector>
#include <string>
using namespace std;


// NEW
#include <map>

typedef map<SOCKET, string> MyMap;


#define MAX_CLIENTS 10
#define DEFAULT_BUFLEN 4096

#pragma comment(lib, "ws2_32.lib") // Winsock library
#pragma warning(disable:4996) // ��������� �������������� _WINSOCK_DEPRECATED_NO_WARNINGS

SOCKET server_socket;

vector<string> history;

int main() {
	// NEW

	map<SOCKET, string> client_name_and_IP;


	system("title Server");

	puts("Start server... DONE.");
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		printf("Failed. Error Code: %d", WSAGetLastError());
		return 1;
	}

	// create a socket
	if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		printf("Could not create socket: %d", WSAGetLastError());
		return 2;
	}
	// puts("Create socket... DONE.");

	// prepare the sockaddr_in structure
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(8888);

	// bind socket
	if (bind(server_socket, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
		printf("Bind failed with error code: %d", WSAGetLastError());
		return 3;
	}

	// puts("Bind socket... DONE.");

	// ������� �������� ����������
	listen(server_socket, MAX_CLIENTS);

	// ������� � �������� ����������
	puts("Server is waiting for incoming connections...\nPlease, start one or more client-side app.");

	// ������ ������ ��������� ������, ��� ����� ������
	// ����� ������������ �������
	// fd means "file descriptors"
	fd_set readfds; // https://docs.microsoft.com/en-us/windows/win32/api/winsock/ns-winsock-fd_set
	SOCKET client_socket[MAX_CLIENTS] = {};

	while (true) {
		// �������� ����� fdset
		FD_ZERO(&readfds);

		// �������� ������� ����� � fdset
		FD_SET(server_socket, &readfds);

		// �������� �������� ������ � fdset
		for (int i = 0; i < MAX_CLIENTS; i++) 
		{
			SOCKET s = client_socket[i];
			if (s > 0) {
				FD_SET(s, &readfds);
			}
		}

		// ��������� ���������� �� ����� �� �������, ����-��� ����� NULL, ������� ����� ����������
		if (select(0, &readfds, NULL, NULL, NULL) == SOCKET_ERROR) {
			printf("select function call failed with error code : %d", WSAGetLastError());
			return 4;
		}

		// ���� ���-�� ��������� �� ������-������, �� ��� �������� ����������
		SOCKET new_socket; // ����� ���������� �����
		sockaddr_in address;
		int addrlen = sizeof(sockaddr_in);
		if (FD_ISSET(server_socket, &readfds)) {
			string message;
			if ((new_socket = accept(server_socket, (sockaddr*)&address, &addrlen)) < 0) {
				perror("accept function error");
				return 5;
			}
			//Sleep(20000);
			for (int i = 0; i < history.size(); i++)
			{
				//cout << history[i] << "\n";
				send(new_socket, history[i].c_str(), history[i].size(), 0);
				Sleep(10);
			}

			message = " List of conversation participants: ";
			send(new_socket, message.c_str(), message.size(), 0);
			string name;
			for (map<SOCKET, string>::iterator it = client_name_and_IP.begin(); it != client_name_and_IP.end(); it++)
			{
				name = " " + it->second + "\n";
				send(new_socket, name.c_str(), name.size(), 0);
			}

			// ������������� ��������� ������� � ������ ������ - ������������ � �������� �������� � ���������
			printf("New connection, socket fd is %d, ip is: %s, port: %d\n", new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));


			// �������� ������������� ��������� ��� ����������������� 
			char client_name[DEFAULT_BUFLEN];

			int client_message_length = recv(new_socket, client_name, DEFAULT_BUFLEN, 0);
			client_name[client_message_length] = '\0';
			message = " ";
			message += client_name;
			message += " has joined the conversation\n";

			for (int i = 0; i < MAX_CLIENTS; i++) {
				if (client_socket[i] != 0) {
					send(client_socket[i], message.c_str(), message.size(), 0);
				}
			}

			client_name_and_IP.insert(map<SOCKET, string>::value_type(new_socket, client_name));


			// �������� ����� ����� � ������ �������
			for (int i = 0; i < MAX_CLIENTS; i++) {
				if (client_socket[i] == 0) {
					client_socket[i] = new_socket;
					printf("Adding to list of sockets at index %d\n", i);
					break;
				}
			}



		}

		// ���� �����-�� �� ���������� ������� ���������� ���-��
		for (int i = 0; i < MAX_CLIENTS; i++)
		{
			SOCKET s = client_socket[i];
			// ���� ������ ������������ � ������� ������
			if (FD_ISSET(s, &readfds))
			{
				// �������� ��������� �������
				getpeername(s, (sockaddr*)&address, (int*)&addrlen);

				// ���������, ���� �� ��� �� ��������, � ����� ���������� �������� ���������
				// recv �� �������� ������� ���������� � ����� ������ (� �� ����� ��� printf %s ������������, ��� �� ����)

				char client_message[DEFAULT_BUFLEN];

				int client_message_length = recv(s, client_message, DEFAULT_BUFLEN, 0);
				client_message[client_message_length] = '\0';

				string check_exit = client_message;
				int i = check_exit.find("off#");
				if (i > 0)
				{
					// �� �������
					cout << "Client #" << i << " is off\n";
					client_socket[i] = 0;
					// �������� 
					string name;
					// � ����� ����� ������������� ��� ���� �����
					for (map<SOCKET, string>::iterator it = client_name_and_IP.begin(); it != client_name_and_IP.end(); it++)
					{
						// ���� ����� ���� ���� � ������
						if (it->first == s)
						{
							name = it->second;
							break;
						}
					}
					string message = " ";
					message += name;
					message += " has leave the conversation\n";

					for (int i = 0; i < MAX_CLIENTS; i++) {
						if (client_socket[i] != 0) {
							send(client_socket[i], message.c_str(), message.size(), 0);
						}
					}

				}
				// -------------------------------------
				else
				{
					string temp = client_message;
					//temp += "\n";
					history.push_back(temp);

					for (int i = 0; i < MAX_CLIENTS; i++) {
						if (client_socket[i] != 0) {
							send(client_socket[i], client_message, client_message_length, 0);
						}

					}
				}
				

			}
		}
	}

	WSACleanup();
}