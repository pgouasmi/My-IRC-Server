#include "IRCPredicate.hpp"
#include <iostream>
#include <string>

#include "User.hpp"

ChannelPredicate::ChannelPredicate(size_t channelId) : channelId(channelId) {}
bool ChannelPredicate::operator () (Channel *channel)
{
	return channel->getUniqueId() == this->channelId;
}

bool ChannelPredicate::operator == (size_t channelId)
{
	return channelId == this->channelId;
}

ChannelPredicateName::ChannelPredicateName(const std::string &channelName) : channelName(channelName) {}
bool ChannelPredicateName::operator () (Channel *channel)
{
	return channel->getName() == this->channelName;
}

bool ChannelPredicateName::operator == (const std::string &channelName)
{
	return channelName == this->channelName;
}

UserPredicate::UserPredicate(size_t userId) : userId(userId) {}

bool UserPredicate::operator () (User *user)
{
	return user->getUniqueId() == this->userId;
}

bool UserPredicate::operator == (size_t userId)
{
	return userId == this->userId;
}

UserPredicateFD::UserPredicateFD(int fd) : socketFD(fd) {}

bool UserPredicateFD::operator () (User *user)
{
	return user->getUserSocketFd() == this->socketFD;
}

bool UserPredicateFD::operator == (int userId)
{
	return userId == this->socketFD;
}

UserPredicateNickname::UserPredicateNickname(const std::string &value) : nickname(value) {}

bool UserPredicateNickname::operator () (User *user)
{
	return user->getNickname() == this->nickname;
}

bool UserPredicateNickname::operator == (const std::string &value)
{
	return value == this->nickname;
}

UserPredicateUsername::UserPredicateUsername(const std::string &value) : username(value) {}

bool UserPredicateUsername::operator () (User *user)
{
	return user->getUserName() == this->username;
}

bool UserPredicateUsername::operator == (const std::string &value)
{
	return value == this->username;
}
