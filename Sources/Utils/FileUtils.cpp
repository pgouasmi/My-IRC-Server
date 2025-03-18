#include <FileUtils.hpp>

#include <fstream>
#include <iostream>
#include <sys/stat.h>

bool FileUtils::fileNameIsValid(std::string fileName)
{
	const std::string illegalChars = "\\/.";
	for (size_t i = 0; i < illegalChars.length(); ++i) {
		if (fileName.find(illegalChars[i]) != std::string::npos)
			return false;
	}
	return true;
}

bool FileUtils::fileExists(std::string filename)
{
	std::ifstream file(filename.c_str());
	const bool exists = file.good() && file.is_open();
	file.close();
	return exists;
}

bool FileUtils::isDirectory(std::string filename)
{
	struct stat buff = {};
	if (stat(filename.c_str(), &buff) == 0)
		return buff.st_mode & S_IFDIR;
	// If stat fails, assume it's not a directory
	return false;
}

bool FileUtils::isSymbolicLink(std::string filename)
{
	struct stat buff = {};
	if (stat(filename.c_str(), &buff) == 0)
		return (buff.st_mode & S_IFLNK);
	// If stat fails, assume it's not a symbolic link
	return false;
}

size_t FileUtils::countLines(std::ifstream &fileFlux)
{
	size_t count = 0;
	std::string line;
	while (!fileFlux.eof()) {
		count++;
		if (!std::getline(fileFlux, line))
			break;
	}
	resetIfStream(fileFlux);
	return count;
}

void FileUtils::readFromFile(std::ifstream &fileFlux, std::string lines[], size_t linesToRead = 0)
{

	for (std::size_t j = 0; j < linesToRead; ++j) {
		std::getline(fileFlux, lines[j]);
	}
	resetIfStream(fileFlux);
}

void FileUtils::readFromFile(std::ifstream &fileFlux, std::list<std::string> &lines, size_t linesToRead = 0)
{
	std::string line;
	for (std::size_t j = 0; j < linesToRead; ++j) {
		std::getline(fileFlux, line);
		lines.push_back(line);
	}
	resetIfStream(fileFlux);
}

void FileUtils::writeToFile(std::ofstream &fileFlux, std::string lines[], size_t linesToWrite = 0)
{

	size_t i = 0;
	while (i < linesToWrite && lines[i].c_str()) {
		fileFlux << lines[i++];
		if (i < linesToWrite)
			fileFlux << std::endl;
	}
}

void FileUtils::printFile(std::ifstream &fileFlux)
{
	std::string line;
	while ((!fileFlux.eof()) && std::getline(fileFlux, line))
		std::cout << line << std::endl;
	resetIfStream(fileFlux);
}

void FileUtils::resetIfStream(std::ifstream &fileFlux)
{
	fileFlux.clear();
	fileFlux.seekg(0, std::ios::beg);
}