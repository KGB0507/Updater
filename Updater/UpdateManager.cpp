#include "UpdateManager.h"

////application	Clock Application
//#version	1.5
//#priority	2
//#releaseDate	01.01.2023
//#sizeInKBytes	6144
//
//
/// specif.txt	0
/// config / settings.ini	0
/// music / CalmStandard.mp3	0
/// music / criticalError.mp3	0
/// music / error.mp3	0
/// music / RelaxStandard.mp3	0
/// music / shutdown.mp3	0
/// music / startup.mp3	0
/// updater / updater.exe	0
/// Clock.exe	0



void DownloadSpecificationHandler(const std::string pathToSpecification, Socket &clientSocket)
{
    try
    {
        std::ifstream specif(pathToSpecification);
        if (!specif)
            throw std::exception("Specification file was not opened");

        clientSocket.SendRequest("Download\tapp.spec");

        std::string line;
        int bytesSum = 0;
        while (std::getline(specif, line))
        {
            if (!line.empty() || line[0] == '/' && line[1] == '/')
                continue;

            int fileIteration;
            std::smatch match;
            if (std::regex_search(line, match, std::regex("\\d+"))) 
                fileIteration = std::stoi(match.str());

            std::regex regex("\\d+|\t");
            line = std::regex_replace(line, regex, "");

            std::ofstream file(line, std::ios::binary);
            if (!file)
                throw std::exception("File blank for receiving from server was not opened");

            std::string request = "Download\t" + line;

            int bytesRead = clientSocket.SendRequest(request);
            //int bytesRead;
            //bytesRead = recv(clientSocket.GetClientSocket(), clientSocket.GetBuffer(), sizeof(clientSocket.GetBuffer()), 0);
            file.write(clientSocket.GetBuffer(), bytesRead);
            bytesSum += bytesRead;
                
            if (!file)
                throw std::exception("Problem while writing received file");
        }

        specif.close();

        std::cout << "Updating finished. Total downloaded " << bytesSum / 1024 << " KB.";
    }
    catch (std::exception& ex)
    {
        std::cerr << "Failed to open file: " << pathToSpecification << ex.what() << std::endl;
    }
}

ServerResponse UpdateListHandler(std::string buffer)
{
    ServerResponse serverResponse;

    std::string response = std::string(buffer);
    std::string delimiter = "/t";
    size_t pos = 0;
    std::string token;

    pos = response.find(delimiter);
    token = response.substr(0, pos);
    serverResponse.status = stoi(token);
    response.erase(0, pos + delimiter.length());

    pos = response.find(delimiter);
    token = response.substr(0, pos);
    serverResponse.numOfUpdates = stoi(token);
    response.erase(0, pos + delimiter.length());

    UpdateInfo updInfo;
    std::list<UpdateInfo> availableUpdateList;

    while ((pos = response.find(delimiter)) != std::string::npos)
    {
        //version
        pos = response.find(delimiter);
        token = response.substr(0, pos);
        updInfo.version = token;
        int numOfUpdates = stoi(token);
        response.erase(0, pos + delimiter.length());

        //update priority
        pos = response.find(delimiter);
        token = response.substr(0, pos);
        updInfo.priority = stoi(token);
        response.erase(0, pos + delimiter.length());

        //date of release
        pos = response.find(delimiter);
        token = response.substr(0, pos);
        updInfo.releaseDate = token;
        response.erase(0, pos + delimiter.length());

        //size of package
        pos = response.find(delimiter);
        token = response.substr(0, pos);
        int size = stoi(token);
        response.erase(0, pos + delimiter.length());

        availableUpdateList.push_back(updInfo);
    }

    serverResponse.availableUpdateList = availableUpdateList;
    
    return serverResponse;
}

void PrintServerResponse(ServerResponse &serverResponse, std::string &selectedVersion)
{
    switch (serverResponse.status)
    {
    case 200:
    {
        std::cout << "Server response status: 200 OK." << std::endl;
        PrintAvailableUpdateList(serverResponse.availableUpdateList);
        std::cout << "Please enter the version you want to install: ";
        std::cin >> selectedVersion;
        break;
    }
    case 300:
    {
        std::cout << "Server response status: 300." << std::endl;
        std::cout << "No updates available. Your version is up to date." << std::endl;
        break;
    }
    case 404:
    {
        std::cout << "Server response status: 404 NOT FOUND." << std::endl;
        std::cout << "Invalid update request: unknown application requested." << std::endl;
        break;
    }
    default:
    {
        std::cout << "Server response status: " << serverResponse.status << "." << std::endl;
        std::cout << "Invalid update request: unknown response status. Try to reconnect to the server." << std::endl;
        break;
    }
    }
}

void PrintAvailableUpdateList(std::list<UpdateInfo> availableUpdateList)
{
    std::cout << "Available updates: " << std::endl;

    int numOfUpdates = availableUpdateList.size();

    for (auto i = availableUpdateList.begin(); i != availableUpdateList.end(); i++)
    {
        std::cout << i->version << "\t";
        std::cout << i->priority << "\t";
        std::cout << i->releaseDate << "\t";
        std::cout << i->sizeInKBytes << std::endl;
    }
}
/*
bool DownloadFile(Socket &clientSocket, const char* buffer, const std::string &savePath)
{
    std::ofstream file(savePath, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to create file: " << savePath << std::endl;
        return false;
    }

    int bytesRead;
    while ((bytesRead = recv(clientSocket.GetClientSocket(), clientSocket.GetBuffer(), sizeof(clientSocket.GetBuffer()), 0)) > 0)
    {
        file.write(buffer, bytesRead);
    }

    file.close();
    return true;
}
*/