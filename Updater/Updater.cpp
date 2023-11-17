/////////////////////////////////////////////////////////////////////////////
//Updater, v.1.0
/////////////////////////////////////////////////////////////////////////////
//Copyright (c) Kirill Belozerov, 2021-2023. All Rights Reserved
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//Notes: 
// add structure UpdateInfo: version, priority, size etc
// add dynamic massive AvailableUpdateList which keeps received information about updates
/////////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <string>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define _CRT_SECURE_NO_WARNINGS

//Information about this Program Build. 
#define VERSION "Version 1.0"
#define AUTHOR "Copyright (c) Kirill Belozerov, 2021-2023. All Rights Reserved"

#define UNDERCONSTR "Under construction"
#define DEBUG 1.0





const string SERVERADDR = "127.0.0.1"; // IP-����� �������
const int SERVERPORT = 8080; // ���� �������


int main() 
{
    setlocale(LC_ALL, "Rus");

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) 
    {
        cerr << "������ ������������� winsock" << endl;
        return 1;
    }

    // �������� ������
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) 
    {
        cerr << "������ �������� ������" << endl;
        WSACleanup();
        return 1;
    }

    // �������� ��������� ������ �������
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVERPORT);
    if (inet_pton(AF_INET, SERVERADDR.c_str(), &(serverAddress.sin_addr.s_addr)) <= 0) 
    {
        cerr << "������ ������ �������" << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // ����������� � �������
    if (connect(clientSocket, reinterpret_cast<const sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) 
    {
        cerr << "������ ����������� � �������" << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // ��������� ������ ���������� � ������
    int appNumber;
    string appVersion;

    cout << "������� ����� ����������: ";
    cin >> appNumber;

    cout << "������� ������ ���������� (�� 3 �����, ����������� ������): ";
    cin >> appVersion;

    // ������������ �������
    string request = to_string(appNumber) + " " + appVersion;

    // �������� �������
    if (send(clientSocket, request.c_str(), request.length(), 0) == SOCKET_ERROR) 
    {
        cerr << "������ �������� �������" << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // ��������� ������
    char buffer[10];
    memset(buffer, 0, sizeof(buffer));

    if (recv(clientSocket, buffer, sizeof(buffer), 0) == SOCKET_ERROR) 
    {
        cerr << "������ ��������� ������" << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // �������� ������
    closesocket(clientSocket);
    WSACleanup();

    // ����� ����������
    string response = std::string(buffer);
    bool hasUpdates = (response == "true");

    cout << "������� ����������: " << (hasUpdates ? "true" : "false") << endl;

    return 0;
}