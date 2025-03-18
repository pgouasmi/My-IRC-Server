#include <string>
#include <ChannelExceptions.hpp>

ChannelCacheException::ChannelCacheException(size_t channelId, const std::string& message) throw() : channelId(channelId), message(message) {}
ChannelCacheException::ChannelCacheException(const ChannelCacheException& other) throw() : channelId(other.channelId), message(other.message) {}
ChannelCacheException::~ChannelCacheException() throw() {}

const char* ChannelCacheException::what() const throw()
{
	return message.c_str();
}