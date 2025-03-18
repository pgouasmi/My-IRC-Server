#include <string>
#include <UserExceptions.hpp>

UserBuildException::UserBuildException(const std::string& message) throw() : message(message) {}
UserBuildException::UserBuildException(const UserBuildException& other) throw() : message(other.message) {}
UserBuildException::~UserBuildException() throw() {}

const char* UserBuildException::what() const throw()
{
	return message.c_str();
}