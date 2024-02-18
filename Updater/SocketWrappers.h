#pragma once
#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#define BUFFERSIZE 4096

class Socket
{
private:
    SOCKET clientSocket;

    std::string serverAddr;
    int serverPort;

    int bufSize;

public:
    char *buffer;
    bool socketStatus;

    Socket(const std::string servAddress, const int servPort);

    ~Socket();

    bool ConnectToServer();

    bool SendRequest(std::string request);

    bool InitBuffer(const int bufferSize);

    bool KillBuffer(const int bufferSize);
};
