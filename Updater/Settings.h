#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <exception>
#include "ClientException.h"

#define SETTINGSFILEPATH "./config/settings.cfg"
#define NUMOFPROPERTIES 3

class Settings
{
private:
	std::fstream settingsFile;

	std::string serverAddr;
	int serverPort;
	std::string specifPath;

public:
	Settings();

	~Settings();

	std::string GetServerAddr();

	int GetServerPort();

	std::string GetSpecifPath();
};