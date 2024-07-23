#include "Settings.h"

Settings::Settings()
{
    settingsFile.open(SETTINGSFILEPATH);
    if (!settingsFile.is_open())
    {
        throw std::fstream::failure("File 'settings.cfg' opening error");
    }
    else
    {
        std::string line;
        std::string token;
        std::vector<std::string> tokens;

        int i = 0;
        int propertiesMatched = 0;

        while (std::getline(settingsFile, line))
        {
            auto start = line.begin();
            auto end = line.end();

            while (start != end)
            {
                auto tab = std::find(start, end, '\t');
                tokens.emplace_back(start, tab);
                start = tab != end ? tab + 1 : end;
            }

            if (tokens[i] == "SERVERADDR")
            {
                serverAddr = tokens[i + 1];
                if (serverAddr.empty())
                    throw ClientException(SettingsInitError, "File 'settings.cfg' corrupted: incorrect server address detected");

                propertiesMatched++;
                i += 2;
            }
            else if (tokens[i] == "SERVERPORT")
            {
                serverPort = std::stoi(tokens[i + 1]);
                if (serverPort < 1000 || serverPort > 10000)
                    throw ClientException(SettingsInitError, "File 'settings.cfg' corrupted: incorrect server port detected");

                propertiesMatched++;
                i += 2;
            }
            else if (tokens[i] == "SPECIFPATH")
            {
                specifPath = tokens[i + 1];
                if (specifPath.empty())
                    throw ClientException(SettingsInitError, "File 'settings.cfg' corrupted: incorrect specification path detected");

                propertiesMatched++;
                i += 2;
            }
            else
                continue;
        }

        if (propertiesMatched < NUMOFPROPERTIES)
            throw ClientException("File 'settings.cfg' corrupted: not all required properties are found");
        else
            std::cout << "Client Settings initialization was successful." << std::endl;
    }
}

Settings::~Settings()
{
    settingsFile.close();
}

std::string Settings::GetServerAddr()
{
    return serverAddr;
}

int Settings::GetServerPort()
{
    return serverPort;
}

std::string Settings::GetSpecifPath()
{
    return specifPath;
}