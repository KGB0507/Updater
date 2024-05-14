#include "SocketWrapper.h"

Socket::Socket(const std::string servAddress, const int servPort)
{
    serverAddr = servAddress;
    serverPort = servPort;

    try
    {
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
            throw std::exception();
    }
    catch (std::exception& ex)
    {
        socketStatus = false;
        std::cout << "Winsocket initialization error: " << ex.what() << std::endl;
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET)
    {
        std::cout << "Socket creation error" << std::endl;
        WSACleanup();
        socketStatus = false;
    }

    sockaddr_in serverAddress{};
    try
    {
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(serverPort);
        if (inet_pton(AF_INET, serverAddr.c_str(), &(serverAddress.sin_addr.s_addr)) <= 0)
            throw std::exception();
    }
    catch (std::exception& ex)
    {
        std::cout << "Server address error: " << ex.what() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        socketStatus = false;
    }

    buffer = nullptr;
    bufSize = 0;

    socketStatus = true;
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
    try
    {
        if (connect(clientSocket, reinterpret_cast<const sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR)
            throw std::exception();
    }
    catch (std::exception& ex)
    {
        std::cout << "Connection to the Server error: " << ex.what() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return false;
    }

    return true;
}

int Socket::SendRequest(std::string request)
{
    int bytesRead;

    try
    {
        int sendStatus = send(clientSocket, request.c_str(), request.length(), 0);
        if (sendStatus == SOCKET_ERROR)
            throw std::exception();
    }
    catch (std::exception& ex)
    {
        std::cout << "Request sending error: " << ex.what() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return false;
    }

    try
    {
        bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead == SOCKET_ERROR)
            throw std::exception();
    }
    catch (std::exception& ex)
    {
        std::cout << "Response receiving error: " << ex.what() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return false;
    }

    return bytesRead;
}

bool Socket::InitBuffer(const int bufferSize)
{
    buffer = new char(bufferSize);

    if (buffer == NULL)
        return false;

    buffer = new char(bufferSize); //now we have 4096 butes in memory, all are 0

    return true;
}

void Socket::KillBuffer(const int bufferSize)
{
    delete[] buffer;
}
