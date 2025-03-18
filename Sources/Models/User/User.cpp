#include "User.hpp"
#include "UserProperties.hpp"
#include "Channel.hpp"

User::User() : currentChannel(NULL) {}

std::string User::getUserName() const
{
	return userName;
}

std::string User::getNickname() const
{
	return nickname;
}

std::string User::getIpAddr() const
{
	return ipAddr;
}

size_t User::getLastPingTimestamp() const
{
	return this->lastPingTimestamp;
}

size_t User::getUniqueId() const
{
	return uniqueId;
}

UserProperties User::getProperties() const
{
	return *properties;
}

void User::setName(const std::string& name)
{
	this->userName = name;
}

void User::setNickname(const std::string& nickname)
{
	this->nickname = nickname;
}

void User::setIpAddr(const std::string& ipAddr)
{
	this->ipAddr = ipAddr;
}

void User::setUniqueId(const size_t& uniqueId)
{
	this->uniqueId = uniqueId;
}

void User::setLastPingTimestamp(size_t timestamp)
{
	this->lastPingTimestamp = timestamp;
}

void User::setProperties(const UserProperties& properties)
{
	*this->properties = properties;
}

void User::setUserSocketId(int userSocketFd)
{
	this->userSocketFd = userSocketFd;
}

int User::getUserSocketFd() const
{
	return userSocketFd;
}

void User::setRealName(const std::string& realName)
{
	this->realName = realName;
}

std::string User::getRealName() const
{
	return realName;
}

Channel *User::getCurrentChannel() const
{
	return this->currentChannel;
}

std::vector<Channel *> User::getChannelList() const
{
	return this->channelList;
}

void User::addChannelToList(Channel *newChannel)
{
	this->channelList.push_back(newChannel);
}

void User::addToBuffer(const std::string &incomingBuffer)
{
	std::string temp = incomingBuffer;
//	StringUtils::trim(temp, "\n");
	if (temp.find(4) != std::string::npos)
		temp.erase(temp.find(4));
	if (incomingBuffer.find("/r/n") != std::string::npos)
		this->receivedBuffer += incomingBuffer;
	else
		this->receivedBuffer += temp;
}

std::string &User::getReceivedBuffer()
{
	return this->receivedBuffer;
}

#include <iostream>
int User::isBufferValid() const
{
//	if (receivedBuffer.find("\r\n", receivedBuffer.size() - 2) != std::string::npos)
//		return OK;
//	if (receivedBuffer.find("\r\n") != std::string::npos)
//		return KO;
//	return WAITING;
	return OK;
}

void User::clearBuffer()
{
	this->receivedBuffer.clear();
}



