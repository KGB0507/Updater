/////////////////////////////////////////////////////////////////////////////
//Updater, v.1.0
/////////////////////////////////////////////////////////////////////////////
//Copyright (c) Kirill Belozerov, 2021-2024. All Rights Reserved
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//Notes: 
/////////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <string>
#include <list>
#include "SocketWrapper.h"
#include "UpdateManager.h"
#include "ClientException.h"
#include "Settings.h"

#pragma comment(lib, "ws2_32.lib")


#define _CRT_SECURE_NO_WARNINGS

//Information about this Program Build. 
#define VERSION "Version 1.0"
#define AUTHOR "Copyright (c) Kirill Belozerov, 2021-2024. All Rights Reserved."

#define DEBUG 1.0

#ifdef DEBUG
#define UNDERCONSTR "Under construction"
#else
#define UNDERCONSTR ""
#endif 

#define STDBUFFERSIZE 4096




int main(int argc, char* argv[]) 
{
    //setlocale(LC_ALL, "Ru");

    std::cout << "Updater " << VERSION << std::endl;
    std::cout << AUTHOR << std::endl;
#ifdef DEBUG
    std::cout << UNDERCONSTR << std::endl << std::endl;
#endif

#ifndef DEBUG
    //Application properties received from the Command line
    std::string appNumber = argv[1];
    std::string appCurrentVersion = argv[2];
#else
    std::string appNumber = "101";
    std::string appCurrentVersion = "1.5.2";
#endif 

    bool socketStatus;

    try
    {
        Settings settings;

        std::string serverAddress = settings.GetServerAddr();
        int serverPort = settings.GetServerPort();
        std::string specifPath = settings.GetSpecifPath();

        Socket clientSocket = Socket(serverAddress, serverPort);
        //////////////
        socketStatus = clientSocket.ConnectToServer();

        clientSocket.InitBuffer(STDBUFFERSIZE);
        char* buffer = clientSocket.GetBuffer();

        std::string request = "RequestUpdate\t" + appNumber + "\t" + appCurrentVersion;
        socketStatus = clientSocket.SendRequestAndReceiveResponse(request);

        ServerResponse serverResponse = UpdateListHandler(buffer);

        std::string selectedVersion;
        PrintServerResponse(serverResponse, selectedVersion);
        
        DownloadSpecificationHandler(specifPath, clientSocket);
        /////////////
    }
    catch (ClientException ex)
    {
        std::cout << "Client error: " << ex.What() << std::endl;
    }
    catch (std::fstream::failure ex)
    {
        std::cout << "Client error: " << ex.what() << std::endl << ex.code() << std::endl;
    }
    catch (std::exception ex)
    {
        std::cout << "Client error: " << ex.what() << std::endl;
    }

    return 0;
}

