//
// Created by pgoua on 20/04/2024.
//

#include <string>
#include <ChannelExceptions.hpp>

ChannelGetException::ChannelGetException(const std::string& message) throw() : message(message) {}

ChannelGetException::~ChannelGetException() throw() {}

const char* ChannelGetException::what() const throw()
{
	return message.c_str();
}