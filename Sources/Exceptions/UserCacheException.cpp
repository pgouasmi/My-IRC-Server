#include <string>
#include <UserExceptions.hpp>

UserCacheException::UserCacheException(size_t userId, const std::string& message) throw() : userId(userId), message(message) {}
UserCacheException::UserCacheException(const UserCacheException& other) throw() : userId(other.userId), message(other.message) {}
UserCacheException::~UserCacheException() throw() {}

const char* UserCacheException::what() const throw()
{
	return message.c_str();
}