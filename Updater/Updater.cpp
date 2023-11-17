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





const string SERVERADDR = "127.0.0.1"; // IP-адрес сервера
const int SERVERPORT = 8080; // Порт сервера


int main() 
{
    setlocale(LC_ALL, "Rus");

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) 
    {
        cerr << "Ошибка инициализации winsock" << endl;
        return 1;
    }

    // Создание сокета
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) 
    {
        cerr << "Ошибка создания сокета" << endl;
        WSACleanup();
        return 1;
    }

    // Создание структуры адреса сервера
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVERPORT);
    if (inet_pton(AF_INET, SERVERADDR.c_str(), &(serverAddress.sin_addr.s_addr)) <= 0) 
    {
        cerr << "Ошибка адреса сервера" << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // Подключение к серверу
    if (connect(clientSocket, reinterpret_cast<const sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) 
    {
        cerr << "Ошибка подключения к серверу" << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // Параметры номера приложения и версии
    int appNumber;
    string appVersion;

    cout << "Введите номер приложения: ";
    cin >> appNumber;

    cout << "Введите версию приложения (до 3 чисел, разделенных точкой): ";
    cin >> appVersion;

    // Формирование запроса
    string request = to_string(appNumber) + " " + appVersion;

    // Отправка запроса
    if (send(clientSocket, request.c_str(), request.length(), 0) == SOCKET_ERROR) 
    {
        cerr << "Ошибка отправки запроса" << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // Получение ответа
    char buffer[10];
    memset(buffer, 0, sizeof(buffer));

    if (recv(clientSocket, buffer, sizeof(buffer), 0) == SOCKET_ERROR) 
    {
        cerr << "Ошибка получения ответа" << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // Закрытие сокета
    closesocket(clientSocket);
    WSACleanup();

    // Вывод результата
    string response = std::string(buffer);
    bool hasUpdates = (response == "true");

    cout << "Наличие обновлений: " << (hasUpdates ? "true" : "false") << endl;

    return 0;
}