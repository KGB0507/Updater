#include "SocketWrapper.h"


Socket::Socket(const std::string serverAddress, const int serverPort)
{
    this->serverAddressLiteral = serverAddress;
    this->serverPort = serverPort;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        socketStatus = false;
        throw ClientException(WSAGetLastError(), "Winsock initialization error");
    }

    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
    {
        socketStatus = false;
        throw ClientException(WSAGetLastError(), "Incorrect Winsock version");
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    socketId = clientSocket;
    if (clientSocket == INVALID_SOCKET)
    {
        socketStatus = false;
        throw ClientException(WSAGetLastError(), "Socket creation error");
    }
    /*
    struct sockaddr_in servAddrTemp;
    servAddrTemp.sin_family = AF_INET;
    servAddrTemp.sin_port = htons(serverPort); 
    auto servAddrLitTemp = serverAddressLiteral.c_str();
    servAddrTemp.sin_addr.s_addr = inet_pton(AF_INET, servAddrLitTemp, &servAddrLitTemp);
    serverAddressStruct = servAddrTemp;
    */
    ZeroMemory(&serverAddressStruct, sizeof(serverAddressStruct));
    serverAddressStruct.sin_family = AF_INET;
    //serverAddressStruct.sin_port = htons(serverPort);
    auto servAddrLiteralTemp = serverAddressLiteral.c_str();
    serverAddressStruct.sin_addr.s_addr = inet_addr(servAddrLiteralTemp);
    //serverAddressStruct.sin_addr.s_addr = inet_pton(AF_INET, servAddrLiteralTemp, &servAddrLiteralTemp);
    serverAddressStruct.sin_port = htons(serverPort);

    buffer = nullptr;
    bufSize = 0;

    socketStatus = true;
}

Socket::Socket()
{
    this->serverAddressLiteral = "127.0.0.1"; //standard server parameters
    this->serverPort = 8080;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        socketStatus = false;
        throw ClientException(WSAGetLastError(), "Winsock initialization error");
    }

    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
    {
        socketStatus = false;
        throw ClientException(WSAGetLastError(), "Invalid Winsock version");
    }

    clientSocket = NULL;

    ZeroMemory(&serverAddressStruct, sizeof(serverAddressStruct));

    //struct sockaddr_in servAddrTemp{};
    //serverAddressStruct = servAddrTemp;

    buffer = nullptr;
    bufSize = 0;

    socketStatus = false;
}

Socket::~Socket()
{
    closesocket(clientSocket);
    WSACleanup();
}

SOCKET Socket::GetClientSocket()
{
    return clientSocket;
}

char* Socket::GetBuffer()
{
    return buffer;
}

bool Socket::GetSocketStatus()
{
    return socketStatus;
}

bool Socket::ConnectToServer()
{
    if (connect(clientSocket, (sockaddr*)&serverAddressStruct, sizeof(serverAddressStruct)) < 0)
    {
        socketStatus = false;
        throw ClientException(WSAGetLastError(), "Connection to the Server error");
    }

    return true;
}

int Socket::SendRequestAndReceiveResponse(std::string request)
{
    int bytesRead;

    int sendStatus = send(clientSocket, request.c_str(), request.length(), 0);
    if (sendStatus == SOCKET_ERROR)
    {
        socketStatus = false;
        throw ClientException(WSAGetLastError(), "Request sending error");
    }

    bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesRead == SOCKET_ERROR)
    {
        socketStatus = false;
        throw ClientException(WSAGetLastError(), "Response receiving error");
    }

    return bytesRead;
}

bool Socket::InitBuffer(const int bufferSize)
{
    buffer = new char(bufferSize); //now we have 4096 butes in memory, all are 0

    if (buffer == NULL)
        return false;

    return true;
}

void Socket::KillBuffer(const int bufferSize)
{
    delete[] buffer;
}
