#pragma once
#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

class Socket
{
private:
    SOCKET clientSocket;
    WSADATA wsaData;

    std::string serverAddr;
    int serverPort;

    int bufSize;

    char *buffer;
    bool socketStatus;

public:
    Socket(const std::string servAddress, const int servPort);

    ~Socket();

    SOCKET GetClientSocket();

    char* GetBuffer();

    bool GetSocketStatus();

    bool ConnectToServer();

    int SendRequest(std::string request);

    bool InitBuffer(const int bufferSize);

    void KillBuffer(const int bufferSize);
};
