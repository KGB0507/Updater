#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <Winsock2.h>
#include <Windows.h>
#include <iostream>
#include <string>
#include "ClientException.h"

class Socket
{
private:
    SOCKET clientSocket;
    WSADATA wsaData;
    sockaddr_in serverAddressStruct;
    int socketId;

    std::string serverAddressLiteral;
    int serverPort;

    int bufSize;

    char *buffer;
    bool socketStatus;

public:
    Socket(const std::string serverAddress, const int serverPort);

    Socket();

    ~Socket();

    SOCKET GetClientSocket();

    char* GetBuffer();

    bool GetSocketStatus();

    bool ConnectToServer();

    int SendRequestAndReceiveResponse(std::string request);

    bool InitBuffer(const int bufferSize);

    void KillBuffer(const int bufferSize);
};
