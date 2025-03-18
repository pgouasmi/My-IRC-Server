#include <string>
#include <ServerExceptions.hpp>

ServerStartingException::ServerStartingException(const std::string& message) throw() : message(message) {}
ServerStartingException::ServerStartingException(const ServerStartingException& other) throw() : message(other.message) {}
ServerStartingException::~ServerStartingException() throw() {}

const char* ServerStartingException::what() const throw()
{
	return message.c_str();
}