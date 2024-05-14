/////////////////////////////////////////////////////////////////////////////
//Updater, v.1.0
/////////////////////////////////////////////////////////////////////////////
//Copyright (c) Kirill Belozerov, 2021-2024. All Rights Reserved
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//Notes: 
// Create a settings.ini file where properties like server address and port will be stored
/////////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <string>
#include <list>
#include "SocketWrapper.h"
#include "UpdateManager.h"

#pragma comment(lib, "ws2_32.lib")


#define _CRT_SECURE_NO_WARNINGS

//Information about this Program Build. 
#define VERSION "Version 1.0"
#define AUTHOR "Copyright (c) Kirill Belozerov, 2021-2024. All Rights Reserved"

#define UNDERCONSTR "Under construction"
#define DEBUG 1.0

#define STDBUFFERSIZE 4096



const std::string SERVERADDR = "127.0.0.1"; // Server IP-address
const int SERVERPORT = 8080; // Server Port
const std::string SPECIFPATH = "app.spec";


int main(int argc, char* argv[]) 
{
    setlocale(LC_ALL, "Rus");
#ifndef DEBUG
    //Application properties received from the Command line
    std::string appNumber = argv[1];
    std::string appCurrentVersion = argv[2];
#else
    std::string appNumber = "101";
    std::string appCurrentVersion = "1.5.2";
#endif 

    bool socketStatus;
    Socket clientSocket = Socket(SERVERADDR, SERVERPORT);

    try
    {
        socketStatus = clientSocket.GetClientSocket();
        if (!socketStatus)
            throw new std::exception();
    }
    catch(std::exception ex)
    {
        std::cout << ex.what();
    }
    
    try
    {
        socketStatus = clientSocket.ConnectToServer();
        if (!socketStatus)
            return 1;
    }
    catch (std::exception ex)
    {
        std::cout << ex.what();
    }

    int bufferSize = 4096;
    clientSocket.InitBuffer(STDBUFFERSIZE);
    char* buffer = clientSocket.GetBuffer();

    std::string request = "RequestUpdate\t" + appNumber + "\t" + appCurrentVersion;
    socketStatus = clientSocket.SendRequest(request);

    ServerResponse serverResponse = UpdateListHandler(buffer);

    std::string selectedVersion;
    PrintServerResponse(serverResponse, selectedVersion);
    /////////////

    DownloadSpecificationHandler(SPECIFPATH, clientSocket);



    /////////////
    return 0;
}

