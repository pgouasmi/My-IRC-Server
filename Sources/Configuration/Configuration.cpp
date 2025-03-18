#include "Configuration.hpp"
#include "FileUtils.hpp"
#include "StringUtils.hpp"
#include "Colors.hpp"
#include "ConfigurationExceptions.hpp"
#include <iostream>
#include <IrcLogger.hpp>
Configuration* Configuration::instance = NULL;

Configuration::Configuration(const std::string &file) : file(file) {}

Configuration::~Configuration()
{
	std::list<ConfigurationSection *>::iterator sectionIterator = sections.begin();
	while (sectionIterator != sections.end())
	{
		delete *sectionIterator;
		sectionIterator++;
	}
	sections.clear();
}

void Configuration::load() throw(ConfigurationIOException)
{
	if (file.empty() || StringUtils::isOnlyWhitespace(file) || !StringUtils::isPrintable(file))
		throw ConfigurationIOException("Filename is invalid !");
	if (FileUtils::isDirectory(file))
		throw ConfigurationIOException("Filename is a directory !");

	std::ifstream configurationFileStream(file.c_str());

	if (!configurationFileStream.is_open())
		throw ConfigurationIOException(file + " could not be opened");

	size_t linesCount = FileUtils::countLines(configurationFileStream);
	std::list<std::string> lines;
	FileUtils::readFromFile(configurationFileStream, lines, linesCount);
	configurationFileStream.close();

	std::list<std::string>::iterator linesIterator = lines.begin();
	while (linesIterator != lines.end())
	{
		std::string line = *linesIterator;
		if (line.empty() || StringUtils::isOnlyWhitespace(line) || !StringUtils::isPrintable(line))
		{
			++linesIterator;
			continue;
		}

		if (line[0] == '[' && line[line.size() - 1] == ']')
		{
			ConfigurationSection *section = new ConfigurationSection(line.substr(1, line.size() - 2));
			sections.push_back(section);
			++linesIterator;
			while (linesIterator != lines.end())
			{
				line = *linesIterator;
				if (line.empty() || StringUtils::isOnlyWhitespace(line) || !StringUtils::isPrintable(line))
				{
					++linesIterator;
					continue;
				}
				if (line[0] == '[' && line[line.size() - 1] == ']')
					break;
				size_t equalPos = line.find('=');
				if (equalPos != std::string::npos)
				{
					std::string key = line.substr(0, equalPos -1);
					std::string value = line.substr(equalPos + 2);
					if (value[0] && value[value.size() - 1] == '"')
						value = value.substr(1, value.size() - 2);
					section->addEntry(key, value);
				}
				++linesIterator;
			}
		}
	}
}

void Configuration::addSection(ConfigurationSection *section)
{
	sections.push_back(section);
}

void Configuration::printSections()
{

	Colors yellow(Colors::FG_LYELLOW);
	Colors green(Colors::FG_GREEN);
	Colors reset(Colors::FG_DEFAULT);
	std::cout << yellow << "CONFIGURATION FILE: " << green << file << reset << std::endl;
	std::cout << "                    " << green << sections.size() << yellow << " SECTIONS WAS FOUNDED" << std::endl;
	std::list<ConfigurationSection *>::iterator sectionIterator = sections.begin();
	while (sectionIterator != sections.end())
	{
		std::cout << yellow << "                    SECTION: " << green << (*sectionIterator)->getName() << reset << std::endl;
		std::cout << yellow << "                     ENTRIES: " << green << (*sectionIterator)->getValues().size() << yellow << " WAS LOADED" << reset << std::endl;
		std::map<std::string, std::string>::iterator entryIterator = (*sectionIterator)->getValues().begin();
		while (entryIterator != (*sectionIterator)->getValues().end())
		{
			std::cout << "                      " << yellow << entryIterator->first << ": " << green <<'[' << entryIterator->second << ']' << std::endl;
			entryIterator++;
		}
		sectionIterator++;
	}
	std::cout << reset << std::endl;
}

ConfigurationSection *Configuration::getSection(const std::string &name)
{
	std::list<ConfigurationSection *>::iterator sectionIterator = sections.begin();
	while (sectionIterator != sections.end())
	{
		if ((*sectionIterator)->getName() == name)
			return *sectionIterator;
		sectionIterator++;
	}
	IrcLogger::getLogger()->log(IrcLogger::ERROR, "Section: " + name + " is not present in: " + file);
	return NULL;
}

void Configuration::setCensoredWords(std::vector<std::string> censoredWords)
{
	this->_censoredWords = censoredWords;
}


std::vector<std::string> Configuration::getCensoredWords()
{
	return this->_censoredWords;
}


Configuration* Configuration::getInstance(const std::string& file)
{
	return instance == NULL ? instance = new Configuration(file) : instance;
}