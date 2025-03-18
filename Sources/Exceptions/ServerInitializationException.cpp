#include <string>
#include <ServerExceptions.hpp>

ServerInitializationException::ServerInitializationException(const std::string& message) throw() : message(message) {}
ServerInitializationException::ServerInitializationException(const ServerInitializationException& other) throw() : message(other.message) {}
ServerInitializationException::~ServerInitializationException() throw() {}

const char* ServerInitializationException::what() const throw()
{
	return message.c_str();
}