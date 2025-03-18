#include <string>
#include <UserExceptions.hpp>

UserCacheExceptionString::UserCacheExceptionString(const std::string& value, const std::string& message) throw() : value(value), message(message) {}
UserCacheExceptionString::~UserCacheExceptionString() throw() {}

const char* UserCacheExceptionString::what() const throw()
{
	return message.c_str();
}
const std::string UserCacheExceptionString::getValue() const throw()
{
	return value;
}