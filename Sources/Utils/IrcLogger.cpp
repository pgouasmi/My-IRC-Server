#include "IrcLogger.hpp"
#include "Colors.hpp"
#include "TimeUtils.hpp"
#include <iostream>

IrcLogger* IrcLogger::instance = NULL;

IrcLogger::IrcLogger() : logFile(NULL), logLevel(TRACE)
{
	this->logFunctions[TRACE] = &IrcLogger::trace;
	this->logFunctions[DEBUG] = &IrcLogger::debug;
	this->logFunctions[INFO] = &IrcLogger::info;
	this->logFunctions[WARN] = &IrcLogger::warn;
	this->logFunctions[ERROR] = &IrcLogger::error;
}
IrcLogger::~IrcLogger()
{
	this->close();
}

void IrcLogger::open(const std::string& file)
{
	this->logFile = new std::ofstream(file.c_str());
}

void IrcLogger::setLogLevel(LogLevel level)
{
	this->logLevel = level;
}

IrcLogger::LogLevel IrcLogger::getLogLevelFromString(const std::string& value)
{
	if (value == "TRACE")
		return TRACE;
	if (value == "DEBUG")
		return DEBUG;
	if (value == "INFO")
		return INFO;
	if (value == "WARN")
		return WARN;
	if (value == "ERROR")
		return ERROR;
	log(IrcLogger::ERROR, "Invalid log level, defaulting to TRACE");
	return TRACE;
}


void IrcLogger::log(LogLevel level, const std::string& message)
{
	std::map<LogLevel, void (IrcLogger::*)(const std::string&)> ::iterator iterator = logFunctions.begin();
	while (iterator != logFunctions.end())
	{
		if (level < logLevel)
		{
			++iterator;
			continue;
		}

		if (iterator->first == level)
		{
			(this->*(iterator->second))(message);
			break;
		}
		++iterator;
	}
}

static const Colors RESET = Colors(Colors::RESET);
static const Colors GRAY = Colors(Colors::FG_LGRAY);
static const Colors RED = Colors(Colors::FG_LRED);
static const Colors BLUE = Colors(Colors::FG_BLUE);
static const Colors GREEN = Colors(Colors::FG_GREEN);
static const Colors YELLOW = Colors(Colors::FG_LYELLOW);

void IrcLogger::trace(const std::string& message)
{
	std::cout << GRAY << TimeUtils::getCurrentTime() << "[TRACE] " << message  << RESET << std::endl;
	*logFile << TimeUtils::getCurrentTime() << "[TRACE] " << message << std::endl;
}

void IrcLogger::debug(const std::string& message)
{
	std::cout << BLUE << TimeUtils::getCurrentTime() << "[DEBUG] " << message << RESET << std::endl;
	*logFile << TimeUtils::getCurrentTime() << "[DEBUG] " << message << std::endl;
}

void IrcLogger::info(const std::string& message)
{
	std::cout << GREEN << TimeUtils::getCurrentTime() << "[INFO] " << message << RESET << std::endl;
	*logFile << TimeUtils::getCurrentTime() << "[INFO] " << message << std::endl;
}

void IrcLogger::warn(const std::string& message)
{
	std::cout << YELLOW << TimeUtils::getCurrentTime() << "[WARN] " << message << RESET << std::endl;
	*logFile << TimeUtils::getCurrentTime() << "[WARN] " << message << std::endl;
}

void IrcLogger::error(const std::string& message)
{
	std::cout << RED << TimeUtils::getCurrentTime() << "[ERROR] " << message << RESET << std::endl;
	*logFile << TimeUtils::getCurrentTime() << "[ERROR] " << message << std::endl;
}

void IrcLogger::close()
{
	if (logFile != NULL)
	{
		logFile->close();
		delete logFile;
		logFile = NULL;
	}
}

IrcLogger* IrcLogger::getLogger()
{
	return instance == NULL ? instance = new IrcLogger() : instance;
}