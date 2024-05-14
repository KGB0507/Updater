#pragma once
#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <vector>
#include <Exception>
#include <regex>
#include <thread>
#include "SocketWrapper.h"


struct UpdateInfo
{
    std::string version;
    int priority = 0;
    std::string releaseDate;
    double sizeInKBytes = 0;
};

struct ServerResponse
{
    int status;
    int numOfUpdates;
    std::list<UpdateInfo> availableUpdateList;
};


void DownloadSpecificationHandler(const std::string pathToSpecification, Socket &clientSocket);

ServerResponse UpdateListHandler(std::string buffer);

void PrintServerResponse(ServerResponse &serverResponse, std::string &selectedVersion);

void PrintAvailableUpdateList(std::list<UpdateInfo> availableUpdateList);

//bool DownloadFile(Socket &clientSocket, const char* buffer, const std::string &savePath);
