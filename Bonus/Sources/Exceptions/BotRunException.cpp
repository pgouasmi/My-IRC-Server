//
// Created by pgoua on 08/05/2024.
//

#include "../../Includes/BotRunException.hpp"

BotRunException::BotRunException(const std::string &error) throw() : message(error) {}

const char *BotRunException::what() const throw()
{
	return this->message.c_str();
}

BotRunException::~BotRunException() throw() {}