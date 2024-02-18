#include "SocketWrappers.h"

Socket::Socket(const std::string servAddress, const int servPort)
{
    serverAddr = servAddress;
    serverPort = servPort;

    try
    {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
            throw std::exception();
    }
    catch (std::exception& ex)
    {
        socketStatus = false;
        std::cout << "Winsocket initialization error: " << ex.what() << std::endl;
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

    socketStatus = true;
}

Socket::~Socket()
{
    closesocket(clientSocket);
    WSACleanup();
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

bool Socket::SendRequest(std::string request)
{
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
        int recvStatus = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (recvStatus == SOCKET_ERROR)
            throw std::exception();
    }
    catch (std::exception& ex)
    {
        std::cout << "Receiving response error: " << ex.what() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return false;
    }

    return true;
}

bool Socket::InitBuffer(const int bufferSize)
{
    buffer = new char(bufferSize);
    memset(buffer, 0, sizeof(buffer)); //now we have 4096 butes in memory, all are 0
}

bool Socket::KillBuffer(const int bufferSize)
{
    delete[] buffer;
}
