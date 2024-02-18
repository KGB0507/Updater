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
#include <Exception>
#include "SocketWrappers.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define _CRT_SECURE_NO_WARNINGS

//Information about this Program Build. 
#define VERSION "Version 1.0"
#define AUTHOR "Copyright (c) Kirill Belozerov, 2021-2024. All Rights Reserved"

#define UNDERCONSTR "Under construction"
#define DEBUG 1.0




struct UpdateInfo
{
    string version;
    int priority = 0;
    string releaseDate;
    double size = 0;
};

list<UpdateInfo> AvailableUpdateList; //List which keeps received information about updates


void PrintAvailableUpdateList()
{
    cout << "Available updates: " << endl;

    int numOfUpdates = AvailableUpdateList.size();

    for (auto i = AvailableUpdateList.begin(); i != AvailableUpdateList.end(); i++)
    {
        cout << i->version << "\t";
        cout << i->priority << "\t";
        cout << i->releaseDate << "\t";
        cout << i->size << endl;
    }
}



const string SERVERADDR = "127.0.0.1"; // Server IP-address
const int SERVERPORT = 8080; // Server Port


int main(int argc, char* argv[]) 
{
    setlocale(LC_ALL, "Rus");

    //Application properties reveived from the Command line
    string appNumber = argv[1];
    string appCurrentVersion = argv[2];

    bool socketStatus;

    Socket clientSocket = Socket(SERVERADDR, SERVERPORT);
    socketStatus = clientSocket.socketStatus;
    if (!socketStatus)
        return 1;
    
    socketStatus = clientSocket.ConnectToServer();
    if (!socketStatus)
        return 1;
    
    string request = appNumber + "\t" + appCurrentVersion;
    socketStatus = clientSocket.SendRequest(request);

    char* buffer = clientSocket.buffer;
    
    //Result handling
    string response = string(buffer);
    string delimiter = "/t";
    size_t pos = 0;
    string token;

    //response status
    pos = response.find(delimiter);
    token = response.substr(0, pos);
    int responseStatus = stoi(token);
    response.erase(0, pos + delimiter.length());

    //number of updates
    pos = response.find(delimiter);
    token = response.substr(0, pos);
    int numOfUpdates = stoi(token);
    response.erase(0, pos + delimiter.length());
    

    /*
    * struct UpdateInfo
    {
        string version;
        int priority;
        string releaseDate;
        double size;
    };

    list<UpdateInfo> AvailableUpdateList; //List which keeps received information about updates
    */

    UpdateInfo updInfo;

    while ((pos = response.find(delimiter)) != string::npos) 
    {
        //version
        pos = response.find(delimiter);
        token = response.substr(0, pos);
        updInfo.version = token;
        int numOfUpdates = stoi(token);
        response.erase(0, pos + delimiter.length());

        //priority
        pos = response.find(delimiter);
        token = response.substr(0, pos);
        updInfo.priority = stoi(token);
        response.erase(0, pos + delimiter.length());

        //date of development
        pos = response.find(delimiter);
        token = response.substr(0, pos);
        updInfo.releaseDate = token;
        response.erase(0, pos + delimiter.length());

        pos = response.find(delimiter);
        token = response.substr(0, pos);
        int size = stoi(token);
        response.erase(0, pos + delimiter.length());

        AvailableUpdateList.push_back(updInfo);
    }
    ////////////////////////////
    
    string selectedVersion;

    switch (responseStatus)
    {
    case 200:
    {
        cout << "Server response status: 200 OK" << endl;
        PrintAvailableUpdateList();
        cout << "Please select the version you want to install: ";
        cin >> selectedVersion;
        break;
    }
    case 300:
    {
        cout << "No updates available. Your version is up to date" << endl;
        break;
    }
    case 404:
    {
        cout << "Invalid update request: unknown application" << endl;
        break;
    }
    default:
    {
        break;
    }
    }

    return 0;
}

