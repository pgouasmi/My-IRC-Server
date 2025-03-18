#include <string>
#include <ChannelExceptions.hpp>

ChannelBuildException::ChannelBuildException(const std::string& message) throw() : message(message) {}
ChannelBuildException::ChannelBuildException(const ChannelBuildException& other) throw() : message(other.message) {}
ChannelBuildException::~ChannelBuildException() throw() {}

const char* ChannelBuildException::what() const throw()
{
	return message.c_str();
}