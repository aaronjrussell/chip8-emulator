#pragma once

#include <exception>
#include <string>

class FileNotFoundException : public std::exception
{
public:
	FileNotFoundException(std::string filename);
	char const* what() const;
	std::string getFilename();
private:
	std::string filename;
};

class RomTooLargeException : public std::exception
{
public:
	RomTooLargeException();
};
