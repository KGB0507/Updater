#include "ClientException.h"

ClientException::ClientException(const int errorCode, const std::string errorMessage)
{
	this->errorCode = errorCode;
	this->errorMessage = errorMessage;
}

ClientException::ClientException(const std::string errorMessage)
{
	this->errorCode = 99; //unknown error code 
	this->errorMessage = errorMessage;
}

std::string ClientException::What()
{
	std::string fullErrorMessage = errorMessage + ": ";
	return fullErrorMessage += std::to_string(errorCode);
}
