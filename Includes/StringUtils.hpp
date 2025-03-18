#ifndef STRINGUTILS_H
#define STRINGUTILS_H
#include <string>
#include <vector>

class StringUtils
{
public:
	static bool isAscii(const std::string str);
	static bool isPrintable(const std::string str);
	static bool isOnlyWhitespace(const std::string str);
	static bool isOnlyDigits(const std::string str);
	static bool isAlpha(const std::string str);
	static bool isAlphaNumeric(const std::string str);
	static void trim(std::string &str, const char *characters);
	static void toUpper(std::string &str);
	static void toLower(std::string &str);
	static void replaceAll(std::string &line, const std::string &key, const std::string &value);
	static std::string ltos(long value);
	static std::vector<std::string>split(const std::string &input, int c);
	static void keepOnlyUsefulChar(std::string &input, const std::string toKeep);
	static std::vector<std::string> generateCensuredStrings(std::vector<std::string> words);
	static std::pair<bool, std::string> hasCensuredWord(std::string word, std::vector<std::string> censuredWords);
	static std::string getMessage(std::vector<std::string>& args);
	static void printvector(std::vector<std::string> vec);
	static void printVvector(std::vector<std::vector<std::string> > vec);
};

#endif //STRINGUTILS_H