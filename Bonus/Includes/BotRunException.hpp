//
// Created by pgoua on 08/05/2024.
//

#ifndef BOTRUNEXCEPTION_HPP
#define BOTRUNEXCEPTION_HPP

#include <exception>
#include <string>


class BotRunException : public std::exception {
	private:
	std::string message;
public:
	BotRunException(const std::string &error) throw();
	const char *what() const throw();
	~BotRunException() throw();
};


#endif //INC_42_IRC_BOTRUNEXCEPTION_HPP
