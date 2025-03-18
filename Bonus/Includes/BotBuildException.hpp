//
// Created by pgoua on 08/05/2024.
//

#ifndef BOTBUILDEXCEPTION_HPP
#define BOTBUILDEXCEPTION_HPP

#include <exception>
#include <string>

class BotBuildException : public std::exception {
private:
	std::string message;
public:
	BotBuildException(const std::string &error) throw();
	const char *what() const throw();
	~BotBuildException() throw();
};


#endif //INC_42_IRC_BOTBUILDEXCEPTION_HPP
