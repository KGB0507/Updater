/////////////////////////////////////////////////////////////////////////////
//Updater, v.1.0
/////////////////////////////////////////////////////////////////////////////
//Copyright (c) Kirill Belozerov, 2021-2023. All Rights Reserved
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//Notes: 
/////////////////////////////////////////////////////////////////////////////


#define _CRT_SECURE_NO_WARNINGS

//Information about this Program Build. 
#define VERSION "Version 1.0"
#define AUTHOR "Copyright (c) Kirill Belozerov, 2021-2023. All Rights Reserved"

#define UNDERCONSTR "Under construction"
#define DEBUG 1.0

#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>



const std::string SERVERADDR = "127.0.0.1"; // IP-����� �������
const int SERVERPORT = 8080; // ���� �������



int main(int argc, char* argv[])
{
    // ��������� ������ ���������� � ������
    int appNumber;
    std::string appVersion;

    std::cout << "������� ����� ����������: ";
    std::cin >> appNumber;

    std::cout << "������� ������ ���������� (�� 3 �����, ����������� ������): ";
    std::cin >> appVersion;

    // �������� ������
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "������ �������� ������" << std::endl;
        return 1;
    }

    // �������� ��������� ������ �������
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVERPORT);
    if (inet_pton(AF_INET, SERVERADDR.c_str(), &serverAddress.sin_addr) <= 0) {
        std::cerr << "������ ������ �������" << std::endl;
        return 1;
    }

    // ����������� � �������
    if (connect(clientSocket, reinterpret_cast<const sockaddr*>(&serverAddress), sizeof(serverAddress)) == -1) {
        std::cerr << "������ ����������� � �������" << std::endl;
        return 1;
    }

    // ������������ �������
    std::string request = std::to_string(appNumber) + " " + appVersion;

    // �������� �������
    if (send(clientSocket, request.c_str(), request.length(), 0) == -1) {
        std::cerr << "������ �������� �������" << std::endl;
        return 1;
    }

    // ��������� ������
    char buffer[10];
    memset(buffer, 0, sizeof(buffer));

    if (recv(clientSocket, buffer, sizeof(buffer), 0) == -1) {
        std::cerr << "������ ��������� ������" << std::endl;
        return 1;
    }

    // �������� ������
    close(clientSocket);

    // ����� ����������
    std::string response = std::string(buffer);
    bool hasUpdates = (response == "true");

    std::cout << "������� ����������: " << (hasUpdates ? "true" : "false") << std::endl;

    return 0;
}
