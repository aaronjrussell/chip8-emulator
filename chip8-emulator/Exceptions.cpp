#include "Exceptions.h"

FileNotFoundException::FileNotFoundException(std::string filename) :
	filename{filename}
{}

char const* FileNotFoundException::what() const
{
	return "File not found!";
}

std::string FileNotFoundException::getFilename()
{
	return filename;
}

RomTooLargeException::RomTooLargeException() : std::exception("ROM too large to load")
{}
