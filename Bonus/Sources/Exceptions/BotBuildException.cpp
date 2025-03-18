//
// Created by pgoua on 08/05/2024.
//

#include "../../Includes/BotBuildException.hpp"

BotBuildException::BotBuildException(const std::string &error) throw() : message(error) {}

const char *BotBuildException::what() const throw()
{
	return this->message.c_str();
}

BotBuildException::~BotBuildException() throw() {}
