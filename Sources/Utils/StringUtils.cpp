#include "StringUtils.hpp"
#include <IrcLogger.hpp>
//#include <regex>
#include <iostream>
#include <stdexcept>

bool StringUtils::isAscii(const std::string str)
{
	for (std::string::size_type i = 0; i < str.size(); ++i)
	{
		const int character = static_cast<unsigned char>(str[i]);
		if (character < 0 || character > 127)
			return false;
	}
	return true;
}

bool StringUtils::isOnlyWhitespace(const std::string str)
{
	for (std::string::size_type i = 0; i < str.size(); ++i)
	{
		const char character = (str[i]);
		if (!std::isspace(character))
			return false;
	}
	return true;
}

bool StringUtils::isOnlyDigits(const std::string str)
{
	for (std::string::size_type i = 0; i < str.size(); ++i)
	{
		const char character = (str[i]);
		if (!std::isdigit(character))
			return false;
	}
	return true;
}

bool StringUtils::isAlpha(const std::string str)
{
	for (std::string::size_type i = 0; i < str.size(); ++i)
	{
		const char character = (str[i]);
		if (!std::isalpha(character))
			return false;
	}
	return true;
}

bool StringUtils::isAlphaNumeric(const std::string str)
{
	for (std::string::size_type i = 0; i < str.size(); ++i)
	{
		const char character = (str[i]);
		if (!std::isalnum(character))
			return false;
	}
	return true;
}

bool StringUtils::isPrintable(const std::string str)
{
	for (std::string::size_type i = 0; i < str.size(); ++i)
	{
		const char character = (str[i]);
		if (!std::isprint(character))
			return false;
	}
	return true;
}

#include <iostream>

void StringUtils::trim(std::string &str, const char *characters)
{
	size_t first = str.find_first_not_of(characters);
	size_t last = str.find_last_not_of(characters);
	if (first == std::string::npos)
		first = 0;
	if (last == std::string::npos)
		last = str.size();
	try
	{
		str = str.substr(first, (last - first + 1));
	}
	catch (const std::exception &exception)
	{
		IrcLogger *logger = IrcLogger::getLogger();
		IrcLogger::LogLevel error = IrcLogger::ERROR;
		logger->log(error, "An error occured during the trim() function");
		logger->log(error, "StringUtils.Cpp : trim() : std::string::substr() L.78");
		logger->log(error, "String : " + str);
		logger->log(error, exception.what());
	}
}

void StringUtils::toUpper(std::string &str)
{
	for (std::string::size_type i = 0; i < str.size(); ++i)
	{
		char &character = (str[i]);
		character = static_cast<char>(std::toupper(character));
	}
}

void StringUtils::toLower(std::string &str)
{
	for (std::string::size_type i = 0; i < str.size(); ++i)
	{
		char &character = (str[i]);
		character = static_cast<char>(std::tolower(character));
	}
}

void StringUtils::replaceAll(std::string &line, const std::string &key, const std::string &value)
{
	const std::size_t keyLen = key.length();
	const std::size_t valueLen = value.length();

	try
	{
		const std::size_t stringLen = line.length();
		for (std::size_t charIndex = 0; charIndex < stringLen; charIndex++)
		{
			size_t pos = line.find(key, charIndex);
			if (pos == std::string::npos)
				continue;
			line.erase(pos, keyLen);
			line.insert(pos, value);
			charIndex = pos + valueLen;
		}
	}
	catch (const std::exception &e)
	{
		std::string trace = "Exception throws during replace :";
		trace.append(e.what());
		throw std::out_of_range(trace);
	}
}

std::vector<std::string> StringUtils::split(const std::string &input, int c)
{

	std::vector<std::string> result;
	if (input.empty())
		return result;
	std::string trimmed = input;

	if (input.find(c) == std::string::npos)
	{
		StringUtils::trim(const_cast<std::string &>(input), "\r\n ");
		result.push_back(input);
		return result;
	}

	size_t i = input.find(c);
	size_t j = 0;
	std::string line;

	while (input[i])
	{
		i = input.find(c, j);
		if (i == std::string::npos)
		{
			try
			{
				std::string subbed = input.substr(j, input.size() - j);
				result.push_back(subbed);
				break;
			}
			catch (const std::exception &exception)
			{
				IrcLogger *logger = IrcLogger::getLogger();
				IrcLogger::LogLevel error = IrcLogger::ERROR;
				logger->log(error, "An error occured during the split() function");
				logger->log(error, "StringUtils.Cpp : split() : std::string::substr() L.160");
				logger->log(error, "String : " + input);
				logger->log(error, exception.what());
			}
		}
		try
		{
			line = input.substr(j, i - j);
			i++;
			j = i;
			result.push_back(line);
			line.clear();
		}
		catch (std::exception &exception)
		{
			IrcLogger *logger = IrcLogger::getLogger();
			IrcLogger::LogLevel error = IrcLogger::ERROR;
			logger->log(error, "An error occured during the split() function");
			logger->log(error, "StringUtils.Cpp : split() : std::string::substr() L.176");
			logger->log(error, "String : " + input);
			logger->log(error, exception.what());
		}
	}
	for (std::vector<std::string>::iterator it = result.begin(); it != result.end(); ++it)
	{
		StringUtils::trim(*it, "\r\n ");
	}

	return result;
}

std::string StringUtils::ltos(long value)
{
	std::string result;
	while (value)
	{
		result = static_cast<char>((value % 10) + '0') + result;
		value /= 10;
	}
	return result;
}

void StringUtils::keepOnlyUsefulChar(std::string &input, const std::string toKeep)
{
	for (std::string::iterator it = input.begin(); it != input.end(); ++it)
	{
		if (toKeep.find(*it) == std::string::npos)
			input.erase(it);
	}
}

static std::string generateCensuredWord(std::string word, char c)
{
	std::string censuredWord = "";
	std::string::iterator begin = word.begin();
	while (begin != word.end())
	{
		censuredWord += c;
		censuredWord += c;
		++begin;
	}
	return censuredWord;
}

std::vector<std::string> StringUtils::generateCensuredStrings(std::vector<std::string> words)
{
	std::vector<std::string> censuredWords;
	int index = 32;
	int limit = 126;
	std::vector<std::string>::iterator it = words.begin();
	while (it != words.end())
	{
		std::string UPPER_WORD = *it;
		StringUtils::toUpper(UPPER_WORD);
		while (index < limit)
		{
			char c = index;
			censuredWords.push_back(generateCensuredWord(UPPER_WORD, c));
			++index;
		}
		++it;
	}
	return censuredWords;
}

std::pair<bool, std::string> StringUtils::hasCensuredWord(std::string word, std::vector<std::string> censuredWords)
{
	std::vector<std::string>::iterator it = censuredWords.begin();
	while (it != censuredWords.end())
	{
		if (word.find(*it) != std::string::npos)
			return std::make_pair(true, *it);
		++it;
	}
	return std::make_pair(false, "");
}

std::string StringUtils::getMessage(std::vector<std::string> &args)
{
	std::string message;
	for (std::vector<std::string>::iterator itMsg = args.begin(); itMsg != args.end(); itMsg++)
	{
		try
		{
			if (itMsg == args.begin())
				*itMsg = (*itMsg).substr(1);
			message += *itMsg + " ";
		}
		catch (const std::exception &exception)
		{
			IrcLogger *logger = IrcLogger::getLogger();
			IrcLogger::LogLevel error = IrcLogger::ERROR;
			logger->log(error, "An error occured during the getMessage() function");
			logger->log(error, "StringUtils.Cpp : getMessage() : std::string::substr() L.273");
			logger->log(error, "String : " + *itMsg);
			logger->log(error, exception.what());
			break;
		}
	}
	return message;
}

void StringUtils::printvector(std::vector<std::string> vec)
{
	for (std::vector<std::string >::iterator it = vec.begin() ; it != vec.end() ; ++it)
	{
		std::cout << *it << std::endl;
	}
}

void StringUtils::printVvector(std::vector<std::vector<std::string> > vec)
{
	size_t i = 0;
	for (std::vector<std::vector<std::string> >::iterator it = vec.begin() ; it != vec.end() ; ++it)
	{
		std::cout << "vector " << i << ":" << std::endl;
		printvector(*it);
		std::cout << std::endl;
		i++;
	}
}
