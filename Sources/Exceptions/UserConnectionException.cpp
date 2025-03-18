#include <string>
#include <UserExceptions.hpp>

UserConnectionException::UserConnectionException(const std::string& message) throw() : message(message) {}
UserConnectionException::UserConnectionException(const UserConnectionException& other) throw() : message(other.message) {}
UserConnectionException::~UserConnectionException() throw() {}

const char* UserConnectionException::what() const throw()
{
	return message.c_str();
}