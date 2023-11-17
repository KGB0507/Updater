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



const std::string SERVERADDR = "127.0.0.1"; // IP-адрес сервера
const int SERVERPORT = 8080; // Порт сервера



int main(int argc, char* argv[])
{
    // Параметры номера приложения и версии
    int appNumber;
    std::string appVersion;

    std::cout << "Введите номер приложения: ";
    std::cin >> appNumber;

    std::cout << "Введите версию приложения (до 3 чисел, разделенных точкой): ";
    std::cin >> appVersion;

    // Создание сокета
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Ошибка создания сокета" << std::endl;
        return 1;
    }

    // Создание структуры адреса сервера
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVERPORT);
    if (inet_pton(AF_INET, SERVERADDR.c_str(), &serverAddress.sin_addr) <= 0) {
        std::cerr << "Ошибка адреса сервера" << std::endl;
        return 1;
    }

    // Подключение к серверу
    if (connect(clientSocket, reinterpret_cast<const sockaddr*>(&serverAddress), sizeof(serverAddress)) == -1) {
        std::cerr << "Ошибка подключения к серверу" << std::endl;
        return 1;
    }

    // Формирование запроса
    std::string request = std::to_string(appNumber) + " " + appVersion;

    // Отправка запроса
    if (send(clientSocket, request.c_str(), request.length(), 0) == -1) {
        std::cerr << "Ошибка отправки запроса" << std::endl;
        return 1;
    }

    // Получение ответа
    char buffer[10];
    memset(buffer, 0, sizeof(buffer));

    if (recv(clientSocket, buffer, sizeof(buffer), 0) == -1) {
        std::cerr << "Ошибка получения ответа" << std::endl;
        return 1;
    }

    // Закрытие сокета
    close(clientSocket);

    // Вывод результата
    std::string response = std::string(buffer);
    bool hasUpdates = (response == "true");

    std::cout << "Наличие обновлений: " << (hasUpdates ? "true" : "false") << std::endl;

    return 0;
}
